#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct node {
    char actVal;
    char dispVal;
    struct node *next;
} STRING;

STRING* createNode(char val) {
    STRING *newNode = (STRING*)malloc(sizeof(STRING));
    newNode->actVal = val;
    if (isalnum(val)) {
        newNode->dispVal = '_';
    } else {
        newNode->dispVal = val;
    }
    newNode->next = NULL;
    return newNode;
}

void randomAnswer(char ans[]) {
    char *easy[14] = {"Home/Alone", "The/Wolf/of/Wallstreet", "Wolverine", "Cars", "Kung/Fu/Panda", "Dunkirk", "Barbie",
                     "Oppenheimer", "The/Dark/Knight/Rises", "Hamilton", "Top/Gun:/Maverick","The/Terminator","Pacific/Rim",
                     "The/Godfather"};
    char *medium[15] = {"Star/Trek:/Into/Darkness", "How/To/Train/Your/Dragon", "The/Good,/the/Bad/and/the/Ugly",
                       "Whiplash", "Baby/Driver", "Forrest/Gump", "Need/for/Speed", "Blue/Beetle", "Schindler's/List",
                       "Blade/Runner/2049", "Ford/v/Ferrari","Rush","Pirates/of/the/Caribbean:/The/Curse/of/the/Black/Pearl",
                       "Hustle","The/Polar/Express"};
    char *hard[11] = {"2001:/A/Space/Odyssey", "Nightmare/Before/Christmas", "A/Fistfull/of/Dollars",
                     "Nightcrawler", "Once/Upon/a/Time/in/Hollywood", "Secret/Society/of/Second-Born/Royals",
                     "300", "The/Shawshank/Redemption", "The/Grown/Ups", "Pet/Sematary", "21/Jumpstreet"};
    srand(time(0)); // set seed for random number generation
    int num;
    num = (rand() % 3) + 1;
    if (num == 1) {
        printf("Difficulty is EASY\n");
        strcpy(ans, easy[(rand() % 11)]);
    } else if (num == 2) {
        printf("Difficulty is MEDIUM\n");
        strcpy(ans, medium[(rand() % 11)]);
    } else {
        printf("Difficulty is HARD\n");
        strcpy(ans, hard[(rand() % 11)]);
    }
}

void convertAnswer(STRING **answer, char *ans) {
    *answer = createNode(ans[0]);
    STRING *current = *answer;
    for (int i = 1; i < strlen(ans); i++) {
        current->next = createNode(ans[i]);
        current = current->next;
    }
}

void checkLetterGuess(STRING *answer, char val, char wrong[], char correct[], int *wrongPos, int *correctPos, char *ans) {
    if (*wrongPos >= 6) {
        printf("Game Over! The answer is: %s\n", ans);
        return;
    }
    int notIn = 1;
    if (strchr(wrong, val) == NULL) {
        val = tolower(val);
        while (answer != NULL) {
            char answerChar = tolower(answer->actVal);
            if (strchr(correct, val) != NULL) {
                printf("Character has already been guessed\n");
                notIn = 0;
                break;
            }
            if (answerChar == val) {
                answer->dispVal = answer->actVal;
                notIn = 0;
            }
            answer = answer->next;
        }
        correct[(*correctPos)++] = val;
        if (notIn) {
            wrong[(*wrongPos)++] = val;
        }
    } else {
        printf("Character has already been guessed\n");
    }
}

int isComplete(STRING *answer) {
    STRING *temp = answer;
    while (temp != NULL) {
        if (temp->dispVal == '_') {
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

int isAnsCorrect(STRING *answer, char guess[]) {
    STRING *temp = answer;
    int guessIndex = 0;
    while (temp != NULL && guess[guessIndex] != '\0') {
        char answerChar = tolower(temp->actVal);
        char guessChar = tolower(guess[guessIndex]);
        if (isalnum(answerChar) || !isalnum(guessChar)) {
            if (answerChar != guessChar) {
                return 0;
            }
            guessIndex++;
        }
        temp = temp->next;
    }
    return guess[guessIndex] == '\0';
}

void displayHangman(STRING *answer, char wrong[]) {
    if (answer == NULL) {
        printf("List is empty\n");
    } else {
        STRING *temp = answer;
        while (temp != NULL) {
            printf("%c", temp->dispVal);
            temp = temp->next;
        }
        printf("\n");
        for (int i = 0; i < strlen(wrong); i++) {
            printf("%c ", wrong[i]);
        }
        printf("\n");
    }
}

int main() {
    char cont = '1';
    while (cont == '1') {
        int wrongPos = 0;
        int correctPos = 0;
        char wrong[6];
        char correct[26];
        memset(wrong, 0, sizeof(wrong));
        memset(correct, 0, sizeof(correct));
        printf("Do you wish to play Hangman? Yes(1) or No(0): ");
        scanf(" %c", &cont);
        getchar();
        if (cont == '0') {
            exit(0);
        } else if (cont == '1') {
            char ans[100];
            int modeChoice;
            printf("Single(1) or double player(2): ");
            scanf("%d", &modeChoice);
            STRING *answer = NULL;
            if (modeChoice == 2) {
                printf("Enter the answer(type \"/\" for spaces): ");
                scanf("%s", ans);
                system("clear");
            } else {
                randomAnswer(ans);
            }
            convertAnswer(&answer, ans);
            while (isComplete(answer) == 0) {
                displayHangman(answer, wrong);
                char guessChoice;
                printf("Guess a character(0) or the answer(1): ");
                scanf(" %c", &guessChoice);
                getchar();
                if (guessChoice == '0') {
                    char guess;
                    printf("Enter character: ");
                    scanf(" %c", &guess);
                    getchar();
                    printf("\n");
                    checkLetterGuess(answer, guess, wrong, correct, &wrongPos, &correctPos, ans);
                    if (wrongPos >= 6) {
                        printf("Game Over! The answer is \"%s\"\n", ans);
                        break;
                    }
                } else if (guessChoice == '1') {
                    char guessAns[100];
                    printf("Enter your guess: ");
                    scanf("%s", guessAns);
                    if (isAnsCorrect(answer, guessAns)) {
                        printf("Correct guess. You won the game! The answer is \"%s\"\n", ans);
                    } else {
                        printf("Wrong guess. The correct answer is \"%s\"\n", ans);
                    }
                    wrongPos = 0;
                    correctPos = 0;
                    break;
                } else {
                    printf("Choose a proper input\n");
                }
                if (isComplete(answer) == 1) {
                    printf("Completed the message \"%s\"\n", ans);
                    break;
                }
            }
            printf("\n");
        } else {
            printf("Choose a proper input\n");
        }
    }
    return 0;
}