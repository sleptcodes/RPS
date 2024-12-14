#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>

char rock = ' ';
char paper = '-';
char scizzors = '@';

struct sigaction sigact;

const int width = 214;
const int height = 63;
char state[height][width];
char nextState[height][width];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void signal_handler(int sig){
    if (sig == SIGINT) exit(0);
}

void init_signals(){
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, (struct sigaction *)NULL);
}




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

void* getUpdatedRow(void* row) {

    pthread_mutex_lock(&lock);

    int i = *(int*)row;
    *(int*)row += 1;

    pthread_mutex_unlock(&lock);

    char localNextRow[width];  

    int numSciz, numRock, numPaper;

    for (int j = 0; j < width; j++) {
        numRock = numPaper = numSciz = 0;

        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (i + x < height && i + x >= 0 && j + y < width && j + y >= 0) {
                    numRock  += (state)[i + x][j + y] == rock ? 1 : 0;
                    numPaper += (state)[i + x][j + y] == paper ? 1 : 0;
                    numSciz  += (state)[i + x][j + y] == scizzors ? 1 : 0;
                }
            }
        }

        if ((state)[i][j] == rock && numPaper > 2) 
            localNextRow[j] = paper;
        else if ((state)[i][j] == paper && numSciz > 2) 
            localNextRow[j] = scizzors;
        else if ((state)[i][j] == scizzors && numRock > 2) 
            localNextRow[j] = rock;
        else 
            localNextRow[j] = (state)[i][j];
    }
   
    for (int j = 0; j < width; j++) {
        (nextState)[i][j] = localNextRow[j];
    }

    return NULL;
}

void updateState() {
    pthread_t t[height];
    int temp = 0;

    for (int i = 0; i < height; i++) {

        assert( !pthread_create(&t[i], NULL, getUpdatedRow, (void*) (&temp)) );
        
    }

    for (int i = 0; i < height; i++) {
        assert(!pthread_join(t[i], NULL));
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++)
            state[i][j] = nextState[i][j];
    }
}



int main()
{
    srand(time(NULL));
    init_signals();


    init_state(&state);

    while (1){
        usleep(50000);

        printState(state);
        updateState();
    }

    return 0;
}