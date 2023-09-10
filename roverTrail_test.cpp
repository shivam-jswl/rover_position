//Testing the library roverTrails.h


#include "roverTrails.h"

int main(){

    Rovers robo;
    int x = robo.trail_finder();
    if(x==-1){
        cout<<"Error: Input file not found"<<endl;
        exit(0);
    }
    if(x==0){
        cout<<"Trail files generated .... Exiting"<<endl;
        exit(0);
    }
    return 0;
}
