(* CLANKER GENERATED FILE TO QUICK TEST; WILL WRITE PROPER TESTS LATER *)

open! Core
open! Hardcaml
open! Hardcaml_waveterm

(* Parse input file - returns list of (direction, distance) tuples *)
let parse_input filename =
  In_channel.read_lines filename
  |> List.map ~f:(fun line ->
       let direction = if String.is_prefix line ~prefix:"L" then 0 else 1 in
       let distance =
         String.chop_prefix_exn line ~prefix:(if direction = 0 then "L" else "R")
         |> Int.of_string
       in
       (direction, distance))

let () =
  (* Create the simulator *)
  let module Sim = Cyclesim.With_interface(Dial.I)(Dial.O) in
  let sim = Sim.create (Dial.create (Scope.create ~flatten_design:true ())) in
  let inputs = Cyclesim.inputs sim in
  let outputs = Cyclesim.outputs sim in

  (* Optional: Create waveform for debugging *)
  let _waveform, sim = Waveform.create sim in

  (* Helper to run one clock cycle *)
  let cycle () =
    Cyclesim.cycle sim
  in

  (* Parse input file *)
  let input_file = "src/input.txt" in
  printf "Parsing input from: %s\n" input_file;
  let rotations = parse_input input_file in
  printf "Found %d rotations\n\n" (List.length rotations);

  (* Initialize - reset the design *)
  inputs.clear := Bits.vdd;
  inputs.start := Bits.gnd;
  inputs.direction := Bits.gnd;
  inputs.distance := Bits.of_int_trunc ~width:16 0;
  cycle ();
  inputs.clear := Bits.gnd;
  cycle ();

  (* Process each rotation *)
  List.iteri rotations ~f:(fun i (dir, dist) ->
    let dir_str = if dir = 0 then "L" else "R" in

    (* Set inputs *)
    inputs.direction := Bits.of_int_trunc ~width:1 dir;
    inputs.distance := Bits.of_int_trunc ~width:16 dist;
    inputs.start := Bits.vdd;
    cycle ();
    inputs.start := Bits.gnd;

    (* Wait for done signal *)
    let max_cycles = dist + 100 in
    let rec wait_done count =
      if count > max_cycles then
        failwith (sprintf "Timeout on rotation %d: %s%d" (i+1) dir_str dist)
      else if Bits.to_int_trunc !(outputs.done_) = 1 then begin
        let pos = Bits.to_int_trunc !(outputs.current_pos) in
        let zeros = Bits.to_int_trunc !(outputs.zero_count) in
        if (i + 1) <= 10 || zeros <> 0 then
          printf "Rotation %4d: %s%-4d -> Position: %2d, Total Zeros: %d\n"
            (i+1) dir_str dist pos zeros;
        cycle ()
      end else begin
        cycle ();
        wait_done (count + 1)
      end
    in
    wait_done 0
  );

  let final_answer = Bits.to_int_trunc !(outputs.zero_count) in
  printf "\n==========================================\n";
  printf "FINAL ANSWER: %d\n" final_answer;
  printf "==========================================\n";

  (* Optionally save waveform for debugging (commented by default) *)
  (* Waveform.expect ~display_width:100 ~display_height:50 waveform; *)
