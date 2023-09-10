/*
Class "Rover" with function "trail_finder()", reads "Test_input" file determine new position after given command,
each positions/Trails is stored in Trail_of_robo_*.txt file for each robot, and final position of each robot is stored in 
"Test_output_final_pos.txt" in same order as in "Test_input.txt".

compiled using g++

*/

#include<iostream>
#include<fstream>
#include<string.h>
#include<sstream>
#include<math.h>
using namespace std;


class Rovers{

    //For only access within class
    private:
        unsigned int x;
        unsigned int y;
        char orientation;
    
        /*Function mat_mul_3*3 finds the multiplication of a 2D Tranformation matrix*/
        void mat_mul_3x3(float final_mat[3][3], float mat_2[3][3]){
            float store[3][3] = {1, 0, 0,
                                0, 1, 0,
                                0, 0, 1};
            for(int i = 0; i<2; i++){
                for(int j =0; j<3; j++){
                    store[i][j] = (final_mat[i][0] * mat_2[0][j]) + (final_mat[i][1] * mat_2[1][j]) + (final_mat[i][2] * mat_2[2][j]);
                }
            }
            for(int i = 0; i<2; i++){
                for(int j =0; j<3; j++){
                    final_mat[i][j] = store[i][j];
                }
            }
        }

        /*function mat_mul_3*1 find the position of the robot with respect to origin
          of platue by multiplying given transformation matrix to transpose({0.0,1}), i.e., position of robot
          with respect to itself*/
        void mat_mul_3x1(float final_mat[3][3], float mat_2[3][1]){
            float store[3][1] = {0,0,1};
            for(int i=0;i<3;i++){
                store[i][0]=(final_mat[i][2]*mat_2[2][0]);
            }
            for(int i = 0; i<3;i++){
                mat_2[i][0] = store[i][0];
            }
        }   

        /* determine the direction of the robot from tranformation matrix and return respective
            cardinal compass point*/
        char angular_orientation(float final_mat[3][3]){
            if(final_mat[0][0]!= 0){
                if(acos(final_mat[0][0])== 0)
                    return 'N';
                else
                    return 'S';
            }
            else{
                if(asin(final_mat[1][0])>0)
                    return 'W';
                else
                    return 'E';
            }
        }

    //For access outside class as well
    public:
        /* funtion to find the trail of each robot and store in a Trail_of_robo_*.txt file, * -> nth, robot
            also stores the final position and orientation of each robot in Test_output_final_pos.txt */
        int trail_finder(){
            char rob_pos_x[10], rob_pos_y[10], rob_direction;
            int i, file_count=1;
            string plat_size_string, rob_pos_string, cmd_string;
            stringstream rob_pos_ss;
            float final_transform_matrix[3][3]={1, 0, 0,
                                                0, 1, 0,
                                                0, 0, 1},
                                    left_mat[3][3]={0, -1, 0,                 //rotaion about z-axis in left (taking counter
                                                    1, 0, 0,                  //clock rotation as +ve)
                                                    0, 0, 1},                 //angle is +90 degree 

                                    right_mat[3][3]={0, 1, 0,                 //rotaion about z-axis in right (taking counter
                                                    -1, 0, 0,                //clock rotation as +ve)
                                                    0, 0, 1},                //angle is -90 degree

                                    trans_mov[3][3]={1, 0, 0,
                                                    0, 1, 1,                   //1 unit movement in +y axis wrt robot
                                                    0, 0, 1},
                                    trail_pos[3][1] = {0, 0, 1};
            ifstream input_file("Test_input.txt");                                      //input file
            ofstream output_file_final_pos("Test_output_final_pos.txt");                //output file 

            if(!input_file)                                                           //Checking file exists or not
                // ERROR: Input file not found
                return -1;

            getline(input_file, plat_size_string);                  //Reading size of plateau

            while(1){                                               //Runs until eof
                if(!getline(input_file, rob_pos_string) | !getline(input_file, cmd_string)){   //reading position and command string
                    input_file.close();                                                        //and checking for eof
                    output_file_final_pos.close();
                    //End of file.... Exiting
                    return 0;
                }

                string trail_file_name = "Trail_of_robo_"+to_string(file_count++)+".txt";
                ofstream trail_file(trail_file_name);

                rob_pos_ss << rob_pos_string;
                rob_pos_ss>>rob_pos_x>>rob_pos_y>>rob_direction;
                x=stoi(rob_pos_x);
                y=stoi(rob_pos_y);


                //intial transformation, translation and orientation
                final_transform_matrix[0][0] = 1;
                final_transform_matrix[1][0] = 0;
                final_transform_matrix[2][0] = 0;

                final_transform_matrix[0][1] = 0;
                final_transform_matrix[1][1] = 1;
                final_transform_matrix[2][1] = 0;

                final_transform_matrix[0][2] = x;
                final_transform_matrix[1][2] = y;
                final_transform_matrix[2][2] = 1;
                
                trail_file <<x<<" "<<y<<" "<<rob_direction<<endl;

                if(rob_direction == 'E')
                    mat_mul_3x3(final_transform_matrix, right_mat);
                else if(rob_direction == 'W')
                    mat_mul_3x3(final_transform_matrix, left_mat);
                else if(rob_direction == 'S'){
                    float rot_2_mat[3][3] = {-1, 0, 0,
                                            0, -1, 0,
                                            0, 0, 1};
                    mat_mul_3x3(final_transform_matrix, rot_2_mat);
                    
                }

                i=0;
                while(cmd_string[i]!= '\0'){

                    //Eular transformation, post multiplication

                    if(cmd_string[i] == 'M'){
                        mat_mul_3x3(final_transform_matrix, trans_mov);                 //Updates final_transform_matrix after M cmd
                        i++;
                        mat_mul_3x1(final_transform_matrix, trail_pos);
                        rob_direction = angular_orientation(final_transform_matrix);
                        trail_file << trail_pos[0][0]<<" "<<trail_pos[1][0]<<" "<<rob_direction<<endl;
                    }
                    if(cmd_string[i] == 'L'){
                        mat_mul_3x3(final_transform_matrix, left_mat);                 //Updates final_transform_matrix after L cmd
                        i++;
                        mat_mul_3x1(final_transform_matrix, trail_pos);
                        rob_direction = angular_orientation(final_transform_matrix);
                        trail_file << trail_pos[0][0]<<" "<<trail_pos[1][0]<<" "<<rob_direction<<endl;
                    }
                    if(cmd_string[i] == 'R'){
                        mat_mul_3x3(final_transform_matrix, right_mat);                 //Updates final_transform_matrix after R cmd
                        i++;
                        mat_mul_3x1(final_transform_matrix, trail_pos);
                        rob_direction = angular_orientation(final_transform_matrix);
                        trail_file << trail_pos[0][0]<<" "<<trail_pos[1][0]<<" "<<rob_direction<<endl;
                    }
                }
                trail_file.close();
                output_file_final_pos<<trail_pos[0][0] <<" "<< trail_pos[1][0] <<" "<<rob_direction<<endl;
            }
        }

};
