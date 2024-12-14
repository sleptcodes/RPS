#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#include <assert.h>

char rock = ' ';
char paper = '-';
char scizzors = '@';

const int width = 214;
const int height = 64;


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

void updateRow(char (*state)[height][width], char (*nextState)[height][width], int i){
	int numSciz, numRock, numPaper;

	for (int j = 0; j < width; j++){

		numRock = 0;
		numPaper = 0;
		numSciz = 0;

		for (int x = -1; x <= 1; x++){
			for (int y = -1; y <= 1; y++){


				if (i + x < height && 
					 i + x >= 0 && 
					 j + y < width && 
					 j + y >= 0){

					numRock  += (*state)[i + x][j + y] == rock     ? 1 : 0;
					numPaper += (*state)[i + x][j + y] == paper    ? 1 : 0;
					numSciz  += (*state)[i + x][j + y] == scizzors ? 1 : 0;

				}

			}
		}

		if ((*state)[i][j]  == rock && numPaper > 2) (*nextState)[i][j] = paper;
		else if ((*state)[i][j] == paper && numSciz > 2) (*nextState)[i][j] = scizzors;
		else if ((*state)[i][j] == scizzors && numRock > 2) (*nextState)[i][j] = rock;
		else (*nextState)[i][j] = (*state)[i][j];
	}
}

void getNextState(char (*prevState)[height][width]){

	char nextState[height][width];

	for (int i = 0; i < height; i++){
		updateRow(prevState, &nextState, i);
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