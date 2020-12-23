//add test functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getLength(char* file);
void storeData(int length, char* file,
               int* curStateList, char* inputList, int* nextStateList);
int getInputLength(char* file);
void storeInputData(int length2, char* file, char* inputOrder);
void getState(int length, int* curStateList, char* inputList, int* nextStateList,
              int length2, char* inputOrder);
int validInput(char input, char* inputList, int length);
void debugger(int length, int* curStateList, char* inputList, int* nextStateList,
              int length2, char* inputOrder);
int moveOne(int length, int* curStateList, char* inputList, int* nextStateList,
            char* inputOrder, int curState, int step);

int main(int argc, char *argv[]) {
    //if no arguments, print error message
    if (argc==1){
        printf("Error: please input 2 filenames\n");
        exit(0);
    }

    //if the first argument is -d, activate debugger
    int debug = (!strcmp(argv[1],"-d"));

    //if too few arguments were provided, print an error message
    //for debugger, must have 3 args, and otherwise must have 2
    if ((debug && argc<4)||(!debug && argc<3)){
        printf("Error: too few arguments\n");
        exit(0);
    }

    //if too many arguments, print error message
    //if debugger has a 4th arg or regular has a 3rd, there are too many
    if ((debug && argc>4)||(!debug && argc>3)){
        printf("Error: too many arguments\n");
        exit(0);
    }

    //if debugging mode, files are args 2 and 3
    //if not, files are args 1 and 2
    char* file1 = debug ? argv[2]: argv[1];
    char* file2 = debug ? argv[3]: argv[2];

    //read through def file and get length
    int length = getLength(file1);
    //initialize storage arrays to the size of length
    int curStateList[length];
    char inputList[length];
    int nextStateList[length];
    //read through def again and store the def data in arrays
    storeData(length, file1, curStateList, inputList, nextStateList);

    //read through input file and get length
    int length2 = getInputLength(file2);
    //initialize input storage array
    char inputOrder[length2];
    //store input data in array
    storeInputData(length2, file2, inputOrder);

    //if debugger mode, open debugger
    if (debug){
        debugger(length, curStateList, inputList, nextStateList, length2, inputOrder);
    }

    //otherwise, move through FSM and print final state
    else {getState(length, curStateList, inputList, nextStateList, length2, inputOrder);}

}

//reads through def file once and finds length of file
int getLength(char *file) {

    //open def file
    FILE* def = fopen(file, "r");

    //check for error. If error, terminate program
    if (!def) {
        printf("Error reading definition file\n");
        exit(0);
    }
    printf("processing FSM definition file %s\n", file);

    int length = 0;
    char* line;

    //read the file one line at a time, and increment length each time
    while (fscanf(def, "%s", line) != EOF) {
        length++;
    }
    printf("FSM has %d transitions\n", length);
    fclose(def);
    return length;
}

//stores the data from the FSM definition into 3 parallel arrays
void storeData(int length, char* file,
               int* curStateList, char* inputList, int* nextStateList) {

    //initialize 3 variables to accept data from each line of def file
    int var1;
    char var2;
    int var3;

    //read through def file again
    FILE* def = fopen(file, "r");

    //for each line, separate the data into the arrays
    for (int i = 0; i < length; i++) {
        //make sure all 3 variables are found before processing a line
        if (fscanf(def, "%d:%c>%d\n", &var1, &var2, &var3) == 3) {
            curStateList[i] = var1;
            inputList[i] = var2;
            nextStateList[i] = var3;
        }
        //if not 3 variables detected, there is a syntax error
        else{
            printf("Error in syntax of definition file\n");
            exit(0);
        }

    }
}

//returns the length of the input file
int getInputLength(char* file){
    //open file
    FILE* input = fopen(file, "r");

    //check for error. If error, terminate program
    if (!input) {
        printf("Error reading input file\n");
        exit(0);
    }
    printf("processing FSM inputs file %s\n", file);

    int length = 0;
    char line;

    //read the file one line at a time, and increment length each time
    while (fscanf(input, "%c\n", &line) > 0) {
        length++;
    }
    fclose(input);
    return length;
}

//stores data from input file into array
void storeInputData(int length2, char* file, char* inputOrder){
    FILE* input = fopen(file, "r");
    char line;

    //for each line, scan the character and add it into the array
    for (int i = 0; i < length2; i++) {
        fscanf(input, "%c\n", &line);
        inputOrder[i] = line;
    }

}

//reads input file and determines final state
void getState(int length, int* curStateList, char* inputList, int* nextStateList,
               int length2, char* inputOrder){

    //initialize state and step # to 0
    int curState = 0;
    int step = 0;

    //until you've reached the end of the input file,
    //move forward one state at a time
    while (step < length2){
        curState = moveOne(length, curStateList, inputList, nextStateList,
                           inputOrder, curState, step);
        step++; //increment step

    }
    //success
    printf("after %d steps, state machine finished successfully at state %d\n",
           step, curState);

}

//checks if input char is in the list of possible inputs
int validInput(char input, char* inputList, int length){
    //if the input isn't in the list of possible inputs,
    //it is invalid
    for (int k = 0; k < length; k++){
        if (inputList[k]==input){
            return 1; //valid: input is in the list
        }
    }
    return 0; //invalid: input was not in the list
}

//activated when in debugger mode
void debugger(int length, int* curStateList, char* inputList, int* nextStateList,
              int length2, char* inputOrder) {

    //initialize state and step # to 0
    int curState = 0;
    int step = 0;
    char inputChar;
    char enter;

    //until you've reached the end of the input file
    while (step < length2) {
            //ask for user input
            printf("FSM debugger>");
            scanf("%c%c", &inputChar, &enter); //allows for char, newline

            //if the user typed p, print current state and definition
            if (inputChar == 'p') {
                printf("The FSM is currently in state %d\n", curState);
                printf("FSM has %d transitions\n",length);
                for (int i = 0; i < length; i++){
                    printf("transition %d: state %d with input %c "
                           "goes to state %d\n",
                           i, curStateList[i], inputList[i], nextStateList[i]);
                }
            }

            //if the user typed n, move forward one state
            else if (inputChar == 'n') {
                curState = moveOne(length, curStateList, inputList, nextStateList,
                inputOrder, curState, step);
                step++;
            }

            //if the user typed anything else, print this prompt
            else{
                printf("Error: invalid input. Enter p to print current state "
                       "or n to move one step forward.\n");
            }

    }

    //end of inputs
    printf("after %d steps, state machine finished successfully at state %d\n",
           step, curState);
    exit(0);

}

//moves the FSM forward one state and returns the new state
int moveOne(int length, int* curStateList, char* inputList, int* nextStateList,
                  char* inputOrder, int curState, int step){

    //get the next input from the inputs array
    char nextInput = inputOrder[step];

    //check if it is a valid input based on the definition file
    if (!validInput(nextInput, inputList, length)){
        printf("Error: %c is invalid input\n",nextInput);
        exit(0);
    }

    //loop through the 3 def arrays
    //when you find the index corresponding to
    //the current state and next input,
    //change the state to the value of nextState at that index
    for (int j = 0; j < length; j++) {
        if (curStateList[j] == curState && inputList[j] == nextInput) {
            int nextState = nextStateList[j];
            printf("at step %d, "
                   "input %c transitions FSM from state %d to state %d\n",
                   step, nextInput, curState, nextState);
            curState = nextState;   //the next state becomes the new state to work with
            break; //once you found a match, stop looping and read the next input
        }
        //if you got to the end of the loop and haven't found a match,
        //you've reached a dead end
        if (j == length - 1) {
            printf("Error detecting state-input match for state:%d input:%c\n",
                   curState, nextInput);
            exit(0);
        }

    }
    return curState;

}