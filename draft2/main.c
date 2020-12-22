#include <stdio.h>

int getLength(char* file);
int storeData(int length, char* file,
               int* curStateList, char* inputList, int* nextStateList);
int getState(int length, char* file,
              int* curStateList, char* inputList, int* nextStateList);

int main(int argc, char *argv[]) {
    char *file1 = argv[1];
    char *file2 = argv[2];

    //read through def file and get length
    int length = getLength(file1);
    //if there was an error reading the file, terminate program
    if (length==0){return 0;}

    //initialize storage arrays to the size of length
    int curStateList[length];
    char inputList[length];
    int nextStateList[length];

    //read through def again and store the def data in arrays
    int data = storeData(length, file1, curStateList, inputList, nextStateList);
    //if failed, terminate program
    if (data==0){return 0;}

    //read through input file and print final state
    getState(length, file2, curStateList, inputList, nextStateList);

}

    //reads through def file once and finds length of file
    int getLength(char *file) {

        //open def file
        FILE *def = fopen(file, "r");

        //check for error
        if (!def) {
            printf("Error reading definition file");
            return 0;
        }
        printf("processing FSM definition file %s\n", file);


        int length = 0;
        char *line;

        //read the file one line at a time, and increment length each time
        while (fscanf(def, "%s", line) != EOF) {
            //printf("%s\n",line);
            length++;
        }
        printf("FSM has %d transitions\n", length);
        fclose(def);
        return length;
    }

    //stores the data from the FSM definition into 3 parallel arrays
    int storeData(int length, char* file,
                   int* curStateList, char* inputList, int* nextStateList) {

        //initialize 3 variables to accept data from each line of def file
        int var1;
        char var2;
        int var3;

        //read through def file again
        //for each line, separate the data into the arrays

        FILE *def = fopen(file, "r");
        //make sure all 3 variables are found before processing a line
        for (int i = 0; i < length; i++) {
            if (fscanf(def, "%d:%c>%d", &var1, &var2, &var3) == 3) {
                curStateList[i] = var1;
                inputList[i] = var2;
                nextStateList[i] = var3;
            }
            //if not 3 variables detected, there is a syntax error
            else{
                printf("Error in syntax of definition file");
                return 0;
            }

        }
        //if the function finished successfully, return 1
        return 1;
    }


    //reads input file and determines final state
    int getState(int length, char* file,
                  int* curStateList, char* inputList, int* nextStateList){

        FILE *input = fopen(file, "r");
        if (!input) {
            printf("Error reading input file");
            return 0;
        }
        printf("processing FSM inputs file %s\n", file);

        //initialize state and step # to 0
        int curState = 0;
        int nextState;
        int step = 0;
        char nextInput;

        //read one line (single char) of the input file
        while (fscanf(input, "%c", &nextInput) > 0) {

            //loop through the 3 arrays
            //when you find the index corresponding to
            //the current state and next input,
            //change the state to the value of nextState at that index

            for (int j = 0; j < length; j++) {
                if (curStateList[j] == curState && inputList[j] == nextInput) {
                    nextState = nextStateList[j];
                    printf("at step %d, "
                           "input %c transitions FSM from state %d to state %d\n",
                           step, nextInput, curState, nextState);
                    curState = nextState;   //the next state becomes the new state to work with
                    step++; //increment number of steps
                    break; //once you found a match, stop looping and read the next input
                }
            }

        }

        //success
        printf("after %d steps, state machine finished successfully at state %d",
               step, curState);
        return 1;
    }



