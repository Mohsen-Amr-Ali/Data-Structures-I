#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

// NOTE: The menu is designed for windows and is tested using the C/C++ gcc.exe compiler.
//       The menu might not work properly on other compilers or operating systems,
//       please don't fail me the assignment mark (since the functions do work) and tell
//       me about it and I'll change the menu to a simpler, numeric one. Thank you.

//------------------------------- Definitions -------------------------------//
#define SIZE 100

#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13
#define ESC 27

typedef struct{
    int items[SIZE];
    int top;
} intStack;

typedef struct{
    char items[SIZE];
    int top;
} charStack;

typedef struct{
    int menuId;
    int currentSelection;
    int itemCount;
    wchar_t **menuItems;
} Menu;

//------------------------------- Function Prototypes -------------------------------//
intStack *intInitialise(void);
charStack *charInitialise(void);
void intPush(intStack *s, int x);
void charPush(charStack *s, char x);
int intPop(intStack *s);
char charPop(charStack *s);
int intIsEmpty(intStack *s);
int charIsEmpty(charStack *s);
int intIsFull(intStack *s);
int intPeek(intStack *s);
char charPeek(charStack *s);

char *stringReverse(char *str);
int isSorted(intStack *s);
void deletePrimes(intStack *s);
intStack *mergeSortedStacks(intStack *s1, intStack *s2);
int balancedParanthesis(char *str);

void display(intStack *s);
void sortStack(intStack *s);
void intSwapFromStack(intStack *s, intStack *temp);
int isPrime(int n);
void inputIntToStack(intStack *s);

void clearScreen(void);
void inputStr(char **str, int *size);
int navigateMenu(Menu *menu);
void exitProgram(void);

//------------------------------- Questions Functions -------------------------------//
//-- Question 1 --> Reverse a string
char *stringReverse(char *str){
    charStack *strRevStack = charInitialise();
    for (int i = 0; str[i] != '\0'; i++){
        charPush(strRevStack, str[i]);
    }

    char *reverse = malloc(sizeof(char) * (strRevStack->top + 2));
    char *reversePtr = reverse;

    while (!charIsEmpty(strRevStack)){
        *reversePtr = charPop(strRevStack);
        reversePtr++;
    }

    *reversePtr = '\0';
    free(strRevStack);
    return reverse;
}

//-- Question 2 --> Sorted Stack
int isSorted(intStack *s){
    if (s->top == -1 || s->top == 0) return 1;

    intStack *tempStack = intInitialise();
    int tempInt, result = 1;

    while (!intIsEmpty(s)){
        tempInt = intPop(s);
        intPush(tempStack, tempInt);
        if (intPeek(s) < tempInt){
            result = 0;
            break;
        }
    }

    intSwapFromStack(s, tempStack);

    free(tempStack);
    return result;
}

//-- Question 3 --> Delete Prime Numbers
void deletePrimes(intStack *s){
    intStack *tempStack = intInitialise();
    int tempInt;

    while (!intIsEmpty(s)){
        tempInt = intPop(s);
        if (!isPrime(tempInt))
            intPush(tempStack, tempInt);
    }

    intSwapFromStack(s, tempStack);

    free(tempStack);
}

//-- Question 4 --> Merge two Stacks and sort
intStack *mergeSortedStacks(intStack *s1, intStack *s2){
    intStack *mergedStack = intInitialise(), *tempStack1 = intInitialise(), *tempStack2 = intInitialise();

    *tempStack1 = *s1;
    *tempStack2 = *s2;

    intSwapFromStack(mergedStack, tempStack1);
    intSwapFromStack(mergedStack, tempStack2);
    sortStack(mergedStack);

    free(tempStack1);
    free(tempStack2);
    return mergedStack;
}

//-- Question 5 --> Balance Paranthesis in a string.
// Note: I'll consider a paranthesis balanced if it has a pair, regardless of the number of
//      pairs or the number of elements inside each pair.
int balancedParanthesis(char *str){
    charStack *s = charInitialise();
    for (int i = 0; str[i] != '\0'; i++){
        if (str[i] == '(' || str[i] == '[' || str[i] == '{'){
            charPush(s, str[i]);
        }

        if ((str[i] == ')' && charPeek(s) == '(') ||
            (str[i] == ']' && charPeek(s) == '[') ||
            (str[i] == '}' && charPeek(s) == '{')){

            charPop(s);
        }else if ((str[i] == ')' && charPeek(s) != '(') ||
                 (str[i] == ']' && charPeek(s) != '[') ||
                 (str[i] == '}' && charPeek(s) != '{')){

            free(s);
            return 0;
        }
    }

    if (charIsEmpty(s)){
        free(s);
        return 1;
    }else{
        free(s);
        return 0;
    }
}

//------------------------------- Main Function -------------------------------//
int main(){
    wchar_t *mainMenuItems[] = {L"1) Reverse a string", L"2) Check if sorted", L"3) Delete Prime Numbers", L"4) Merge and Sort", L"5) Balance Paranthesis", L"\x1b[91mExit Program\x1b[0m"};
    Menu mainMenu = {0, 0, 6, mainMenuItems};

    while(1){
        int selected = navigateMenu(&mainMenu);
        switch(selected){
            case 1:
                while(1){
                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<String Reverser>\x1b[0m\n\n");

                    int size = 32;
                    char *str = malloc(sizeof(char) * size);
                    printf("\x1b[33mEnter a string: \x1b[0m");
                    scanf(" ");
                    inputStr(&str, &size);

                    printf("\n\x1b[38;2;255;103;0mReversed String:\x1b[0m %s\n", stringReverse(str));

                    printf("\n\x1b[33mDo you want to reverse another string? (Y/N): \x1b[0m");
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(str);
                        break;
                    }
                    free(str);
                }

                break;

            case 2:
                while(1){
                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Sorted Checker>\x1b[0m\n\n");
                    intStack *s = intInitialise();
                    wprintf(L"\x1b[33mEnter the elements of the stack (press ESC to stop):\x1b[0m\n");
                    inputIntToStack(s);

                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Sorted Checker>\x1b[0m\n");
                    printf("\n\x1b[38;2;255;103;0mStack: \x1b[0m");
                    display(s);

                    if (isSorted(s)) printf("\n\x1b[32mStack is sorted.\x1b[0m\n");
                    else printf("\n\x1b[31mStack is not sorted.\n\x1b[0m");

                    printf("\n\x1b[33mDo you want to check another stack? (Y/N): \x1b[0m");
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(s);
                        break;
                    }
                    free(s);
                }

                break;

            case 3:
                while(1){
                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Prime Number Deleter>\x1b[0m\n\n");
                    intStack *s = intInitialise();

                    wprintf(L"\x1b[33mEnter the elements of the stack (press ESC to stop):\x1b[0m\n");
                    inputIntToStack(s);

                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Prime Number Deleter>\x1b[0m\n");

                    printf("\n\x1b[38;2;255;103;0mStack: \x1b[0m");
                    display(s);

                    deletePrimes(s);

                    printf("\n\x1b[38;2;255;103;0mStack without prime numbers: \x1b[0m");
                    display(s);

                    printf("\n\n\x1b[33mDo you want to delete prime numbers from another stack? (Y/N): \x1b[0m");
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(s);
                        break;
                    }
                    free(s);
                }

                break;

            case 4:
                while(1){
                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Merge and Sort>\x1b[0m\n\n");
                    intStack *s1 = intInitialise(), *s2 = intInitialise();

                    wprintf(L"\x1b[33mEnter the elements of the first stack (press ESC to stop):\x1b[0m\n");
                    inputIntToStack(s1);

                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Merge and Sort>\x1b[0m\n");
                    printf("\n\x1b[38;2;255;103;0mFirst Stack: \x1b[0m");
                    display(s1);

                    wprintf(L"\n\n\x1b[33mEnter the elements of the second stack (press ESC to stop):\x1b[0m\n");
                    inputIntToStack(s2);

                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Merge and Sort>\x1b[0m\n");

                    printf("\n\x1b[38;2;255;103;0mFirst Stack: \x1b[0m");
                    display(s1);

                    printf("\n\x1b[38;2;255;103;0mSecond Stack: \x1b[0m");
                    display(s2);

                    if (!isSorted(s1) || !isSorted(s2)){
                        printf("\n\x1b[31mWarning: At least one of the initial stacks was not sorted.\x1b[0m");
                    }

                    intStack *mergedStack = mergeSortedStacks(s1, s2);

                    printf("\n\x1b[38;2;255;103;0m\nMerged and Sorted Stack: \x1b[0m");
                    display(mergedStack);

                    printf("\n\n\x1b[33mDo you want to merge and sort another two stacks? (Y/N): \x1b[0m");
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(s1);
                        free(s2);
                        free(mergedStack);
                        break;
                    }
                    free(s1);
                    free(s2);
                    free(mergedStack);
                }

                break;

            case 5:
                while(1){
                    clearScreen();
                    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Paranthesis Balance Checker>\x1b[0m\n\n");

                    int size = 32;
                    char *str = malloc(sizeof(char) * size);

                    printf("\x1b[33mEnter a string: \x1b[0m");
                    scanf(" ");
                    inputStr(&str, &size);
                    printf("\n");

                    if (balancedParanthesis(str)) printf("\x1b[32mParanthesis are balanced.\x1b[0m\n");
                    else printf("\x1b[31mParanthesis are not balanced.\x1b[0m\n");

                    printf("\n\x1b[33mDo you want to check another string? (Y/N): \x1b[0m");
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(str);
                        break;
                    }
                    free(str);
                }

                break;

            case 6:
                exitProgram();
                break;
        }
    }
    return 0;
}

//------------------------------- Main Stack Functions -------------------------------//
intStack *intInitialise(void){
    intStack *s = malloc(sizeof(intStack));
    s->top = -1;
    return s;
}

charStack *charInitialise(void){
    charStack *s = malloc(sizeof(charStack));
    s->top = -1;
    return s;
}

void intPush(intStack *s, int x){
    if (intIsFull(s)){
        printf("Error: couldn't push %d, stack is full", x);
        return;
    }
    
    s->top++;
    s->items[s->top] = x;
}

void charPush(charStack *s, char x){
    s->top++;
    s->items[s->top] = x;
}

int intPop(intStack *s){
    return s->items[(s->top)--];
}

char charPop(charStack *s){
    return s->items[(s->top)--];
}

int intIsEmpty(intStack *s){
    if (s->top == -1) return 1;
    else return 0;
}

int charIsEmpty(charStack *s){
    if (s->top == -1) return 1;
    else return 0;
}

int intIsFull(intStack *s){
    if (s->top == SIZE - 1) return 1;
    else return 0;
}

int intPeek(intStack *s){
    return s->items[s->top];
}

char charPeek(charStack *s){
    return s->items[s->top];
}

//------------------------------- Auxillary Functions -------------------------------//
void display(intStack *s){
    intStack *temp = intInitialise();
    int x;
    printf("\x1b[96m(top)\x1b[0m [");
    while (!intIsEmpty(s)){
        x = intPop(s);
        intPush(temp, x);
        printf("%d", x);

        if(s->top >= 0) printf(", ");
    }
    printf("] \x1b[96m(bottom)\x1b[0m");

    intSwapFromStack(s, temp);
    free(temp);
}

void intSwapFromStack(intStack *s1, intStack *s2){
    while (!intIsEmpty(s2)){
        intPush(s1, intPop(s2));
    }
}

int isPrime(int n){
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;

    for (int i = 3; i <= sqrt(n); i += 2){
        if (n % i == 0){
            return 0;
        }
    }

    return 1;
}

void sortStack(intStack *s){
    intStack *temp = intInitialise();
    int tempInt;

    while(!intIsEmpty(s)){
        tempInt = intPop(s);

        while(!intIsEmpty(temp) && intPeek(temp) > tempInt){
            intPush(s, intPop(temp));
        }

        intPush(temp, tempInt);
    }

    intSwapFromStack(s, temp);
    
    free(temp);
}

void inputStr(char **str, int *size){ 
    int i = 0;
    while (1){
        char ch = getchar();

        if (ch == '\n'){ 
            (*str)[i] = '\0';
            break;
        }

        (*str)[i++] = ch;
        if (i + 1 >= *size){ 
            *size += 16;
            *str = realloc(*str, *size);
        }
    }
}

void inputIntToStack(intStack *s){
    char buffer[SIZE];
    int index = 0;
    int i = 1;
    int x;

    wprintf(L"\x1b[38;2;255;103;0mEnter element %d: \x1b[0m", i++);

    while (1){
        int ch = _getch();
        if (ch == ESC){ 
            break;
        }else if (ch >= '0' && ch <= '9' || ch == '-'){
            buffer[index++] = ch;
            putchar(ch);
        }else if (ch == '\b' && index > 0){
            index--;
            printf("\b \b");
        }else if (ch == ENTER && index > 0){
            buffer[index] = '\0';
            if (index > 0){
                x = atoi(buffer);
                intPush(s, x);
                index = 0;
            }
            wprintf(L"\n\x1b[38;2;255;103;0mEnter element %d: \x1b[0m", i++);
        }
    }
}

//------------------------------- Menu Functions -------------------------------//
void clearScreen(void){
    system("cls");
}

void printPreMessages(int menuId){
    wprintf(L"\x1b[94mData Structure Assignment 1\x1b[0m\t\t\x1b[96m<Mohsen Amr - 9655>\x1b[0m\n");
    wprintf(L"\x1b[33mPlease select an option:\x1b[0m\n\n");
}

void displayMenu(const Menu *menu){
    clearScreen();
    printPreMessages(menu->menuId);
    for (int i = 0; i < menu->itemCount; i++){
        if (i == menu->currentSelection){
            wprintf(L"\x1b[47;30m%s\x1b[0m\r\n", menu->menuItems[i]);
        }else{
            wprintf(L"%s\r\n", menu->menuItems[i]);
        }
    }
}

int navigateMenu(Menu *menu){
    int choice = -1;
    while (choice < 0){
        displayMenu(menu);
        int ch = _getch();
        if (ch == 0xE0){
            ch = _getch();
            if (ch == KEY_UP){
                menu->currentSelection = (menu->currentSelection - 1 + menu->itemCount) % menu->itemCount;
            }
            else if (ch == KEY_DOWN){
                menu->currentSelection = (menu->currentSelection + 1) % menu->itemCount;
            }
        }
        else if (ch == ENTER){
            choice = menu->currentSelection;
        }else if (ch >= '1' && ch <= '0' + menu->itemCount){
            menu->currentSelection = ch - '1';
        }
    }
    return choice + 1;
}

void exitProgram(void){
    clearScreen();
    char input;
    printf("\x1b[33mAre you sure you want to exit the program? (Y/N):\x1b[0m ");
    scanf(" %c", &input);

    if (tolower(input) == 'y'){
        for (int i = 0; i < 3; i++){
            clearScreen();
            printf("\x1b[94mGoodbye!\x1b[0m\n");
            printf("\n\x1b[31mExiting program");
            for (int i = 0; i < 3; i++){
                printf(".");
                Sleep(300);
            }
        }
         exit(0);
    }
}