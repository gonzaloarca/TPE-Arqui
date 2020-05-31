#include <graphics_engine.h>
#define INPUT_BUFFER_SIZE 100

static int windowIdx;

void runShell( int window ){
    windowIdx = window;
    while(1){
        char inputBuffer[INPUT_BUFFER_SIZE];
        getInput( inputBuffer, INPUT_BUFFER_SIZE );
        //parse();
    }
}

// static void parse(){
    
// }