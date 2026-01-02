open! Core
open! Hardcaml
open! Signal

(* Inputs
  - 'a is a polymorphic type var.
  - [@bits] annotation provided by ppx_hardcaml to specify bit width of signal. default is 1.
*)
module I = struct
  type 'a t =
    { clock : 'a
    ; clear : 'a
    ; direction : 'a
    ; distance : 'a [@bits 16]
    ; start : 'a
    }
  [@@deriving hardcaml]
end

(* Outputs *)
module O = struct
  type 'a t =
    { current_pos : 'a [@bits 7]
    ; zero_count : 'a [@bits 16]
    ; done_ : 'a
    }
  [@@deriving hardcaml]
end

(* States
  - sexp_of : for debugging, converts state to s-expression
  - compare ~localize : for comparing states; -localize is an Oxcaml optimization. Enables use to use (==:) operator for ex.
  - enumerate : to get a list of all possible states
*)
module States = struct
  type t =
    | Idle
    | Processing
  [@@deriving sexp_of, compare ~localize, enumerate]
end

(* Inspiration from: https://github.com/janestreet/hardcaml_template_project *)
let create scope (i : _ I.t) : _ O.t =
  (* spec - tells reg when to clk and rst *)
  let spec = Reg_spec.create ~clock:i.clock ~clear:i.clear () in
  (* Always DSL - kinda like always @(posedge clock) in verilog *)
  let open Always in
  let sm = State_machine.create (module States) spec in

  (* Variables regs - clicks remaining, curr pos, 0 count, direction latch, initialized flag *)
  (* Use let%hw_var for better signal naming in waveforms *)
  let%hw_var clicks_remain = Variable.reg spec ~width:16 in
  let%hw_var curr_pos = Variable.reg spec ~width:7 in
  let%hw_var zero_count = Variable.reg spec ~width:16 in

  (* Latch direction so it can't change mid-rotation *)
  let%hw_var dir = Variable.reg spec ~width:1 in

  (* Track initialization to set position to 50 only once *)
  let%hw_var initialized = Variable.reg spec ~width:1 in

  let%hw_var done_ = Variable.wire ~default:gnd () in

  compile
    [ sm.switch
        [ ( Idle
          , [ when_
                i.start
                [ (* One-time initialization: dial starts at 50, count starts at 0 *)
                  when_
                    (initialized.value ==:. 0)
                    [ curr_pos <-- of_int_trunc ~width:7 50
                    ; zero_count <-- zero 16
                    ; initialized <-- vdd
                    ]
                ; (* Latch direction and distance for this rotation *)
                  dir <-- i.direction
                ; clicks_remain <-- i.distance
                ; (* Handle zero-distance rotations *)
                  if_
                    (i.distance ==:. 0)
                    [ (* For Part 1: count if final position (no movement) is 0 *)
                      when_ (curr_pos.value ==:. 0) [ zero_count <-- zero_count.value +:. 1 ]
                    ; done_ <-- vdd
                    ; sm.set_next Idle
                    ]
                    [ sm.set_next Processing ]
                ]
            ] )
        ; ( Processing
          , [ ( let next_pos =
                  mux2 dir.value
                    (*
                    * Kinda like a ternary operator
                    * - right: if pos == 99 set to 0 else pos + 1
                    * - left: if pos == 0 set to 99 else pos - 1
                    *)
                    (* Right: 99 wraps to 0, otherwise +1 *)
                    (mux2 (curr_pos.value ==:. 99) (zero 7) (curr_pos.value +:. 1))
                    (* Left: 0 wraps to 99, otherwise -1 *)
                    (mux2 (curr_pos.value ==:. 0) (of_int_trunc ~width:7 99) (curr_pos.value -:. 1))
                in
                when_
                  (clicks_remain.value >:. 0)
                  [ (* Part 1: Only count when the final click of rotation lands on 0 *)
                    when_
                      ((clicks_remain.value ==:. 1) &&: (next_pos ==:. 0))
                      [ zero_count <-- zero_count.value +:. 1 ]
                  ; clicks_remain <-- clicks_remain.value -:. 1
                  ; curr_pos <-- next_pos
                  ; when_
                      (clicks_remain.value ==:. 1)
                      [ done_ <-- vdd; sm.set_next Idle ]
                  ]
              )
            ] )
        ]
    ];

  { O.current_pos = curr_pos.value
  ; zero_count = zero_count.value
  ; done_ = done_.value
  }
;;
