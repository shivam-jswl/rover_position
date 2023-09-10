<h1>Rover_Position</h1>
<h2>N rover's trails and final postions detection</h2>
<h4>Author: Shivam Jaiswal <br>
Connect: https://shivamjaiswal.free.nf
</h4>


<h2> What it does </h2>
Determines the trails and positions of 'n' mobile robots, given their initial positions, movement directions in the four orthogonal directions (North, West, East, South), and the size of the rectangular environment specified in a file named "Test_input.txt,"




<h2> File explanation </h2>


<b>Rovers.cpp</b> -> program containing codes

<b>Test_input.txt</b> -> input file in the given format

<b>Test_output_final_pos.txt</b> -> output file generated/overwritten during execution of the program
                             It contains final position and orientation of all robots in same order as given in Test_input.txt
                             each position is in new line

<b>Trail_of_robo_*.txt</b> ->  Trails of each individual robot taken during time of travelling.
                        Each robot has its own trail file.
                        each position and orientation is in new line
                        generated/overwritten during execution of the program
