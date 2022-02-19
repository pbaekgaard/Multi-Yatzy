/*************************************************/
/*###############################################*/
/*#                                             #*/
/*#Name: --------------> Peter Bækgaard         #*/
/*#E-mail: ------------> pbakga21@student.aau.dk#*/
/*#Group Number: ------> Group B224             #*/
/*#Field of Study: ----> Software               #*/
/*#                                             #*/
/*###############################################*/
/*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#define PAIR 2
#define TOAK 3
#define FOAK 4
#define FH 3
#define YAHTZY 5

/*Enumeration types*/
enum modes {ones, twos, threes, fours, fives, sixes, bonus = 7, 
            onepair, twopairs, threeofakind, fourofakind, 
            smallstraight, largestraight, fullhouse, chance, yatzy};
enum sums {smallSum = 6, finalSum = 17};

/*Function Prototypes*/
void roll_multiple_dice(int, int*);
void scoreCalculator(enum modes, int*, int, int*);
void playGame(int);
void startMenu(void);
int findSingles(int*, int, int);
int findPairs(int*, int, int, int);
int findStraight(int*, int, int);
int fullHouse(int*, int);
int findChance(int*, int);
void chancefier(int, int*, int*, int*, int);
void finalScore(int*);
void roundDisplay(char*, int*, int, int);
void flushInput(void);

int main(void) {
    srand(time(NULL));
    startMenu(); /*Shows the PRESS ENTER TO START title screen*/
    return EXIT_SUCCESS;
}

void playGame(int speed) { /*The game function it self that prompts the user for inputs*/
    char n = '2';
    int throws;
    int* dice_result;
    int scoreboard[18];
    char* roundTitle[16] = {"Ones", "Twos", "Threes", "Fours", "Fives", "Sixes", 
                            "BONUS", "Pair", "Two Pairs", "Three of a Kind", 
                            "Four of a kind", "Small Straight", "Large Straight", 
                            "Full House", "Chance", "YATZY"};
    enum modes gamemode; /*Declare gamemode as type 'enum modes'*/

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("How many dice do you want to roll? (5-255)\n>");
    scanf("%s", &n);
    throws = atoi(&n);
        while (isdigit(n) == 0) { /*Make sure the user inputs a digit, and not a character.*/
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printf("Please write a valid number!\n>");
            scanf("%s",&n);
            throws = atoi(&n);
        }
        while (throws < 5 || throws > 255) { /*Makes sure the input is between 5 and 255. Too high of a number results in 0?*/
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printf("Your number should be between 5 and 255!\n>");
            scanf("%s",&n);
            throws = atoi(&n);
        }
            /*If the user has entered a number higher than 5,
            A for-loop is executed, with the enum values of the different gamemodes.
            The for-loop then runs the different modes with different dice rolls
            for each gamemode. */
        if (isdigit(n) && throws>=5) {
            for (gamemode = ones; gamemode <= yatzy; gamemode++) {
                dice_result = (int*) malloc(sizeof(int)*throws);
                roll_multiple_dice(throws, dice_result);
                scoreCalculator(gamemode, dice_result, throws, scoreboard);
                if(gamemode < 6) {
                    roundDisplay(roundTitle[gamemode], dice_result, throws, scoreboard[gamemode]);
                    sleep(speed);
                }
                else if (gamemode > 7){
                    roundDisplay(roundTitle[gamemode-1], dice_result, throws, scoreboard[gamemode]);
                    sleep(speed);
                }

                free(dice_result); /*Free the malloc array from the memory*/
            }
            finalScore(scoreboard);
        }
}

void roll_multiple_dice(int throws, int *dice_result) { /*Rolls the amount of dice specified by the user*/
    int i;
    for (i = 0; i < (int)throws; i++) {
        dice_result[i] = rand() % 6 + 1;
    }
}

void scoreCalculator (enum modes gamemode, int* dice_result, int throws, int* scoreboard) {
    /*The scoreCalculator function is basically a large switch case
      It takes in the gamemode that is being played, the dice results,
      the amount of dice throws and the scoreboard as input.
      The function is resposible for running and getting the correct
      results for each of the different gamemodes (ones, twos, pairs, Full House etc.)*/
    switch (gamemode) {
        case ones:
            scoreboard[ones] = findSingles(dice_result, throws, 1); /*Adds the score of the roll, to the scoreboard*/
            break;
        case twos:
            scoreboard[twos] = 2 * findSingles(dice_result, throws, 2);
            break;
        case threes:
            scoreboard[threes] = 3 *findSingles(dice_result, throws, 3);
            break;
        case fours:
            scoreboard[fours] = 4 *findSingles(dice_result, throws, 4);
            break;
        case fives:
            scoreboard[fives] = 5 *findSingles(dice_result, throws, 5);
            break;
        case sixes:
            scoreboard[sixes] = 6 *findSingles(dice_result, throws, 6);
            scoreboard[smallSum] = scoreboard[ones]+scoreboard[twos]
                                 +scoreboard[threes]+scoreboard[fours]
                                 +scoreboard[fives]+scoreboard[sixes]; 
            break;
        case bonus:
            if ((scoreboard[0]+scoreboard[1]+scoreboard[2]+scoreboard[3]+scoreboard[4]+scoreboard[5]) >= 63) {
                scoreboard[bonus] = 50;
            }
            else {
                scoreboard[bonus] = 0;
            }
            break;
        case onepair:
            findPairs(dice_result, throws, PAIR, 1);
            scoreboard[onepair] = findPairs(dice_result, throws, PAIR, 1);
            break;
        case twopairs:
            scoreboard[twopairs] = findPairs(dice_result, throws, PAIR, 2);
            break;
        case threeofakind:
            scoreboard[threeofakind] = findPairs(dice_result, throws, TOAK, 1);
            break;
        case fourofakind:
            scoreboard[fourofakind] = findPairs(dice_result, throws, FOAK, 1);
            break;
        case smallstraight:
            scoreboard[smallstraight] = findStraight(dice_result, throws, 1);
            break;
        case largestraight:
            scoreboard[largestraight] = findStraight(dice_result, throws, 2);
            break;
        case fullhouse:
            scoreboard[fullhouse] = findPairs(dice_result, throws, FH, 2);
            break;
        case chance:
            scoreboard[chance] = findChance(dice_result, throws);
            break;
        case yatzy:
            if(findPairs(dice_result, throws, YAHTZY, 1) > 0) { 
                scoreboard[yatzy] = 50; }
            else{ 
                scoreboard[yatzy] = 0; }
            scoreboard[finalSum] = (scoreboard[smallSum] + scoreboard[bonus] + scoreboard[onepair]
                                 + scoreboard[twopairs] + scoreboard[threeofakind] + scoreboard[fourofakind] 
                                 + scoreboard[smallstraight] + scoreboard[largestraight] + scoreboard[fullhouse] 
                                 + scoreboard[chance] + scoreboard[yatzy]);
            break;
    }
}

int findSingles(int* dice_result, int throws, int eyes) {
    /*Finds how many dice with a given eye, has been rolled
    For example, if it checks for 6, and 3 is rolled, it returns 3*/
    int numDices = 0;
    int i;
    for (i = 0; i <= throws; i++) {
        if(numDices < 5 && dice_result[i] == eyes){
            numDices++;
        }
    }
    return numDices;
}

int findStraight(int* dice_result, int throws, int straightType) {
    /*Checks if the user has rolled a Small Straith or Large Straight*/
    if (straightType == 1) {
        if (findSingles(dice_result, throws, 1) >= 1 && 
            findSingles(dice_result, throws, 2) >= 1 && 
            findSingles(dice_result, throws, 3) >= 1 && 
            findSingles(dice_result, throws, 4) >= 1 && 
            findSingles(dice_result, throws, 5) >= 1) {
        return 15;
    }
    return 0;
    }
    if (straightType == 2) {
        if (findSingles(dice_result, throws, 2) >= 1 && 
            findSingles(dice_result, throws, 3) >= 1 && 
            findSingles(dice_result, throws, 4) >= 1 && 
            findSingles(dice_result, throws, 5) >= 1 && 
            findSingles(dice_result, throws, 6) >= 1) {
        return 20;
    }
    return 0;
    }
return EXIT_FAILURE;
}

int findPairs(int* dice_result, int throws,int pairType, int numPairs) {
    /*This function finds the number of pairs
    The function takes in the number of throws,
    the type of pairs to look for (1 pair, 2 pairs, Three Of a Kind etc.)
    And then returns the score to the scoreCalculator*/
    int i, pairScore, pairsFound, onesFound, twosFound, threesFound, foursFound, fivesFound, sixesFound;
    onesFound = twosFound = threesFound = foursFound = fivesFound = sixesFound = pairsFound = pairScore = 0;

    for (i = 0; i <= numPairs; i++) {

        if(sixesFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 6)>=pairType){
                pairScore = pairScore + (6 * pairType);
                sixesFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
        if(fivesFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 5)>=pairType){
                pairScore = pairScore + (5 * pairType);
                fivesFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
        if(foursFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 4)>=pairType){
                pairScore = pairScore + (4 * pairType);
                foursFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
        if(threesFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 3) >= pairType){
                pairScore = pairScore + (3 * pairType);
                threesFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
        if(twosFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 2)>=pairType){
                pairScore = pairScore + (2 * pairType);
                twosFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
        if(onesFound < 1 && (pairsFound != numPairs)) {
            if(findSingles(dice_result, throws, 1)>=pairType){
                pairScore = pairScore + (1 * pairType);
                onesFound = 1;
                pairsFound++;
                if(pairType == FH) {
                    pairType = PAIR;
                }
            }
        }
    }
    if (pairsFound == numPairs) {
        return pairScore;
    }
    return 0;
}

int findChance(int* dice_result, int throws) { /*Calls the chancefier function 6 using (6, 5, 4, 3, 2, 1)*/
    int dicesToFind;                            /*And accumulating the score which is returned to the scoreCalculator*/
    int score;
    int numEyes;

    dicesToFind = 5;
    score = 0;
    for (numEyes = 6; numEyes >= 1; numEyes--) {
        chancefier(numEyes, &dicesToFind, &score, dice_result, throws); /*Chancefier gets called 6 times corrosponding to the number of eyes of a die*/
    }
    return score;
}

void chancefier(int numEyes, int* dicesToFind, int* score, int* dice_result, int throws){ /*Calculates the score of the given number*/
    if(findSingles(dice_result, throws, numEyes) > 0) {                                   /*Fx. if it looks for 6s and there are 2, it gives a score of 12*/
        if (findSingles(dice_result, throws, numEyes) < *dicesToFind) {
            *dicesToFind = *dicesToFind - findSingles(dice_result, throws, numEyes);
            *score = *score + (findSingles(dice_result, throws, numEyes)*numEyes);
        }
        else {
            *score = *score + numEyes*(*dicesToFind);
            *dicesToFind = 0;
        }
    }
}

void startMenu(void) { /*A Title Screen*/
    int fastMode = 0;
    char begin;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("############################\n");
    printf("                            \n");
    printf("                            \n");
    printf("  WELCOME TO MY YATZY GAME  \n");
    printf("                            \n");
    printf("----------------------------\n");
    printf("                            \n");
    printf("   PRESS 'ENTER' TO BEGIN   \n");
    printf("   PRESS 'S' FOR SPEEDMODE   \n");
    printf("                            \n");
    printf("----------------------------\n");
    printf("                            \n");
    printf("                            \n");
    printf("############################\n");
        /*Continues to the game if the user presses 'ENTER'*/
    begin = getchar();
    while (!(begin == '\n' || begin == 's' || begin == 'S')) {
        begin = getchar();
    }
    if (begin == 's' || begin == 'S'){
        playGame(fastMode);
    }
    if (begin == '\n'){
        playGame(1);
    }
}

void finalScore(int* scoreboard) {
    char choice;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("############################\n\n\n");
    printf("      FINAL SCOREBOARD      \n\n");
    printf("----------------------------\n");
    printf("                            \n");
    printf(" GameType        |   Score  \n");
    printf("----------------------------\n");   /*Prints the scores for the different rounds in a fancy style*/
    printf(" Ones            |     %d   \n", scoreboard[ones]);
    printf(" Twos            |     %d   \n", scoreboard[twos]);
    printf(" Threes          |     %d   \n", scoreboard[threes]);
    printf(" Fours           |     %d   \n", scoreboard[fours]);
    printf(" Fives           |     %d   \n", scoreboard[fives]);
    printf(" Sixes           |     %d   \n", scoreboard[sixes]);
    printf("----------------------------\n");
    printf(" SUM             |     %d   \n", scoreboard[smallSum]);
    printf(" BONUS           |     %d   \n", scoreboard[bonus]);
    printf("----------------------------\n");
    printf(" One Pair        |     %d   \n", scoreboard[onepair]);
    printf(" Two Pairs       |     %d   \n", scoreboard[twopairs]);
    printf(" Three of A Kind |     %d   \n", scoreboard[threeofakind]);
    printf(" Four of A Kind  |     %d   \n", scoreboard[fourofakind]);
    printf(" Small Straight  |     %d   \n", scoreboard[smallstraight]);
    printf(" Large Straight  |     %d   \n", scoreboard[largestraight]);
    printf(" Full House      |     %d   \n", scoreboard[fullhouse]);
    printf(" Chance          |     %d   \n", scoreboard[chance]);
    printf(" YATZY           |     %d   \n", scoreboard[yatzy]);
    printf("----------------------------\n");
    printf(" SUM             |     %d   \n", scoreboard[finalSum]);
    printf("----------------------------\n\n");
    printf("############################\n\n\n");
    printf("  Play Again     Exit Game  \n");
    printf("   'ENTER'          'q'     \n\n");
    flushInput(); /*Flush the input stream to allow the user to answer the prompt*/
    choice = getchar();
    while (choice != '\n') {
        if(choice == 'q') { /*Exit the game if the user answers with 'q'*/
            exit(1);
        }
        choice = getchar();
    }
    if (choice == '\n'){
        startMenu(); /*Restart the game if the user presses 'ENTER' without any other input*/
    }

}

void roundDisplay(char* roundTitle, int* dice_result, int throws, int score){
    int i;
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("############################\n\n\n");
    printf("      GAME IN PROGRESS      \n\n");
    printf("----------------------------\n");
    printf("      %s                  \n", roundTitle); /*Printf the different gamemode titles*/
    printf("----------------------------\n\n");
    printf("      ");
        for (i = 0; i < throws; i++) { /*Printf the corrosponding dice Roll for the round.*/
        printf("%d ", dice_result[i]);
    }
    printf("\n\n");
    printf("----------------------------\n");
    printf("      YOU SCORED: %d                  \n", score); /*Printf the score corrosponding to the round played*/
    printf("----------------------------\n\n");
    printf("############################\n");
}

void flushInput(void) {/*Function to flush the input stream*/
    char flush;
    while((flush = getchar()) != '\n');
}