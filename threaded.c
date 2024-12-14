#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

char rock = ' ';
char paper = '-';
char scizzors = '@';

const int width = 214;
const int height = 63;

typedef struct thread_data {
    char (*state)[height][width];  // Shared state array
    char (*nextState)[height][width];
    int row;                         // Row index to process
} getUpdatedRowParams;

void printState(char state[height][width]){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++)
			printf("%c", state[i][j]);
		printf("\n");
	}
}

void init_state(char (*state)[height][width]){

	int c;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			c = rand()% 3;

			if (c == 0) (*state)[i][j] = rock;
			else (*state)[i][j] = c == 1 ? paper : scizzors;

		}
	}
}

void* getUpdatedRow(void* ps) {
    getUpdatedRowParams* params = (getUpdatedRowParams*)ps;
 

    int numSciz, numRock, numPaper;

    for (int j = 0; j < width; j++) {
        numRock = numPaper = numSciz = 0;

        // Process neighboring cells (same as before)
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (params->row + x < height && params->row + x >= 0 && j + y < width && j + y >= 0) {
                    numRock  += (*params->state)[params->row + x][j + y] == rock ? 1 : 0;
                    numPaper += (*params->state)[params->row + x][j + y] == paper ? 1 : 0;
                    numSciz  += (*params->state)[params->row + x][j + y] == scizzors ? 1 : 0;
                }
            }
        }

        // Update the local nextState array for the current row
        if ((*params->state)[params->row][j] == rock && numPaper > 2) 
            (*params->nextState)[params->row][j] = paper;
        else if ((*params->state)[params->row][j] == paper && numSciz > 2) 
            (*params->nextState)[params->row][j] = scizzors;
        else if ((*params->state)[params->row][j] == scizzors && numRock > 2) 
            (*params->nextState)[params->row][j] = rock;
        else 
            (*params->nextState)[params->row][j] = (*params->state)[params->row][j];
    }


    return NULL;
}

void getNextState(char (*prevState)[height][width]) {
    pthread_t t[height];
    char nextState[height][width];
    getUpdatedRowParams params[height];  // Array of thread params

    for (int i = 0; i < height; i++) {
        params[i].state = prevState;
        params[i].nextState = &nextState;
        params[i].row = i;

        assert(!pthread_create(&t[i], NULL, getUpdatedRow, (void*)(&params[i])));
    }

    for (int i = 0; i < height; i++) {
        assert(!pthread_join(t[i], NULL));
    }

    for (int i = 0; i < height; i++)
    	for (int j = 0; j < width; j++)
    		(*prevState)[i][j] = nextState[i][j];
}


int main()
{
	srand(time(NULL));

	char state[height][width];
	

    init_state(&state);


    while(1){
		usleep(50000);

        printState(state);
		getNextState(&state);
	}

	return 0;
}