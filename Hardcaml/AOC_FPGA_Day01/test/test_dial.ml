(* CLANKER GENERATED FILE TO QUICK TEST; WILL WRITE PROPER TESTS LATER *)
open! Core
open! Hardcaml
open! Hardcaml_waveterm

(* Example input from problem description *)
let example_input =
  [ "L68"; "L30"; "R48"; "L5"; "R60"; "L55"; "L1"; "L99"; "R14"; "L82" ]

let parse_rotation line =
  let direction = if String.is_prefix line ~prefix:"L" then 0 else 1 in
  let distance =
    String.chop_prefix_exn line ~prefix:(if direction = 0 then "L" else "R")
    |> Int.of_string
  in
  (direction, distance)

let%expect_test "Example from problem description - should output 3" =
  let module Sim = Cyclesim.With_interface(Dial.I)(Dial.O) in
  let sim = Sim.create (Dial.create (Scope.create ~flatten_design:true ())) in
  let inputs = Cyclesim.inputs sim in
  let outputs = Cyclesim.outputs sim in

  let cycle () =
    Cyclesim.cycle sim
  in

  (* Reset *)
  inputs.clear := Bits.vdd;
  inputs.start := Bits.gnd;
  inputs.direction := Bits.gnd;
  inputs.distance := Bits.of_int_trunc ~width:16 0;
  cycle ();
  inputs.clear := Bits.gnd;
  cycle ();

  (* Process rotations *)
  let rotations = List.map example_input ~f:parse_rotation in
  List.iteri rotations ~f:(fun i (dir, dist) ->
    let dir_str = if dir = 0 then "L" else "R" in

    inputs.direction := Bits.of_int_trunc ~width:1 dir;
    inputs.distance := Bits.of_int_trunc ~width:16 dist;
    inputs.start := Bits.vdd;
    cycle ();
    inputs.start := Bits.gnd;

    let rec wait_done count =
      let pos = Bits.to_int_trunc !(outputs.current_pos) in
      if count <= 5 then
        printf "  Cycle %d: pos=%d, done=%d\n" count pos (Bits.to_int_trunc !(outputs.done_));
      if count > (dist + 10) then
        failwith (sprintf "Timeout on rotation %d" (i+1))
      else if Bits.to_int_trunc !(outputs.done_) = 1 then begin
        let zeros = Bits.to_int_trunc !(outputs.zero_count) in
        printf "After %s%-3d: Position = %2d, Total Zeros = %d (took %d cycles)\n"
          dir_str dist pos zeros count;
        cycle ()
      end else begin
        cycle ();
        wait_done (count + 1)
      end
    in
    wait_done 0
  );

  let final_answer = Bits.to_int_trunc !(outputs.zero_count) in
  printf "Final answer: %d\n" final_answer;
  [%expect {|
      Cycle 0: pos=50, done=0
      Cycle 1: pos=49, done=0
      Cycle 2: pos=48, done=0
      Cycle 3: pos=47, done=0
      Cycle 4: pos=46, done=0
      Cycle 5: pos=45, done=0
    After L68 : Position = 83, Total Zeros = 0 (took 67 cycles)
      Cycle 0: pos=82, done=0
      Cycle 1: pos=81, done=0
      Cycle 2: pos=80, done=0
      Cycle 3: pos=79, done=0
      Cycle 4: pos=78, done=0
      Cycle 5: pos=77, done=0
    After L30 : Position = 53, Total Zeros = 0 (took 29 cycles)
      Cycle 0: pos=52, done=0
      Cycle 1: pos=53, done=0
      Cycle 2: pos=54, done=0
      Cycle 3: pos=55, done=0
      Cycle 4: pos=56, done=0
      Cycle 5: pos=57, done=0
    After R48 : Position = 99, Total Zeros = 0 (took 47 cycles)
      Cycle 0: pos=0, done=0
      Cycle 1: pos=99, done=0
      Cycle 2: pos=98, done=0
      Cycle 3: pos=97, done=0
      Cycle 4: pos=96, done=1
    After L5  : Position = 96, Total Zeros = 1 (took 4 cycles)
      Cycle 0: pos=95, done=0
      Cycle 1: pos=96, done=0
      Cycle 2: pos=97, done=0
      Cycle 3: pos=98, done=0
      Cycle 4: pos=99, done=0
      Cycle 5: pos=0, done=0
    After R60 : Position = 54, Total Zeros = 1 (took 59 cycles)
      Cycle 0: pos=55, done=0
      Cycle 1: pos=54, done=0
      Cycle 2: pos=53, done=0
      Cycle 3: pos=52, done=0
      Cycle 4: pos=51, done=0
      Cycle 5: pos=50, done=0
    After L55 : Position =  1, Total Zeros = 1 (took 54 cycles)
      Cycle 0: pos=0, done=1
    After L1  : Position =  0, Total Zeros = 2 (took 0 cycles)
      Cycle 0: pos=99, done=0
      Cycle 1: pos=98, done=0
      Cycle 2: pos=97, done=0
      Cycle 3: pos=96, done=0
      Cycle 4: pos=95, done=0
      Cycle 5: pos=94, done=0
    After L99 : Position =  1, Total Zeros = 2 (took 98 cycles)
      Cycle 0: pos=0, done=0
      Cycle 1: pos=1, done=0
      Cycle 2: pos=2, done=0
      Cycle 3: pos=3, done=0
      Cycle 4: pos=4, done=0
      Cycle 5: pos=5, done=0
    After R14 : Position = 13, Total Zeros = 3 (took 13 cycles)
      Cycle 0: pos=14, done=0
      Cycle 1: pos=13, done=0
      Cycle 2: pos=12, done=0
      Cycle 3: pos=11, done=0
      Cycle 4: pos=10, done=0
      Cycle 5: pos=9, done=0
    After L82 : Position = 33, Total Zeros = 3 (took 81 cycles)
    Final answer: 3
    |}]
