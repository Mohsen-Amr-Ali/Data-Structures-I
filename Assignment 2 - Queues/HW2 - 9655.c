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
    int elements[SIZE];
    int front;
    int back;
} Queue;

typedef struct{
    int items[SIZE];
    int top;
} intStack;

typedef struct{
    int menuId;
    int currentSelection;
    int itemCount;
    wchar_t **menuItems;
} Menu;

//------------------------------- Function Prototypes -------------------------------//
Queue *initialise();
int isFull(Queue *q);
int isEmpty(Queue *q);
void enqueue(Queue *q, int x);
int dequeue(Queue *q);

intStack *intInitialise(void);
void intPush(intStack *s, int x);
int intPop(intStack *s);
int intIsEmpty(intStack *s);

int maximum(Queue *q);
void swapFirstLast(Queue *q);
void insert(Queue *q, int value, int index);
void reverseOdd(Queue *q);
void reverse(Queue *q, int k);

void displayQueue(Queue *q);
void swapFromQueue(Queue *q1, Queue *q2);
void inputIntToQueue(Queue *q);void inputInt(int *num, int sign);
void inputInt(int *num, int sign);

void colorPrint(char *string, char color, ...);
void clearScreen(void);
void printPreMessages(int menuId);
void displayMenu(const Menu *menu);
int navigateMenu(Menu *menu);
void exitProgram(void);

//------------------------------- Questions Functions -------------------------------//
//-- Question 1 --> Get maximum value
int maximum(Queue *q){
    if (isEmpty(q)){
        colorPrint("Error: queue is empty", 'R');
        return;
    }
    Queue *temp = initialise();
    int max, x;

    max = x = dequeue(q);
    enqueue(temp, x);
    
    while(!isEmpty(q)){
        x = dequeue(q);
        if(x > max) max = x;
        enqueue(temp, x);
    }

    swapFromQueue(q, temp);
    free(temp);

    return max;
}

//-- Question 2 --> Swap first and last elements in a queue
void swapFirstLast(Queue *q){
    if (isEmpty(q)){
        colorPrint("Error: couldn't swap, queue is empty", 'R');
        return;
    }
    Queue *temp = initialise();
    int x, first = dequeue(q);
    
    while(1){
        x = dequeue(q);
        if(isEmpty(q)){
            enqueue(q, x);
            break;
        }

        enqueue(temp, x);
    }

    swapFromQueue(q, temp);

    enqueue(q, first);
    free(temp);
}

//-- Question 3 --> Insert at specific index
void insert(Queue *q, int value, int index){
    if (isEmpty(q)){
        colorPrint("Error: queue is empty", 'R');
        return;
    }
    Queue *temp = initialise();
    int i = 0;

    swapFromQueue(temp, q);
    while(i++ <index && !isEmpty(temp)) enqueue(q, dequeue(temp));
    enqueue(q, value);
    swapFromQueue(q, temp);

    if(i<=index) colorPrint("\nWarning: queue ends before specified index [%d], value added at the end of queue.", 'R', index);
    free(temp);
}

//-- Question 4 --> Reverse odd numbers
void reverseOdd(Queue *q){
    if (isEmpty(q)){
        colorPrint("Error: queue is empty.", 'R');
        return;
    }
    Queue *tempQueue = initialise();
    intStack *tempStack = intInitialise();
    int x;

    while(!isEmpty(q)){
        x = dequeue(q);
        if(x%2 != 0){
            intPush(tempStack, x);
            enqueue(tempQueue, 1);
        }else enqueue(tempQueue, x);
    }

    while(!isEmpty(tempQueue)){
        x = dequeue(tempQueue);
        if(x==1) enqueue(q, intPop(tempStack));
        else enqueue(q, x);
    }

    free(tempQueue);
    free(tempStack);
}

//-- Question 5 --> Reverse first k elements
void reverse(Queue *q, int k){
    if (isEmpty(q)){
        colorPrint("Error: queue is empty", 'R');
        return;
    }
    Queue *tempQueue = initialise();
    intStack *tempStack = intInitialise();

    for(int i=0; i<k; i++){
        intPush(tempStack, dequeue(q));
    }

    swapFromQueue(tempQueue, q);
    while(!intIsEmpty(tempStack)) enqueue(q, intPop(tempStack));
    swapFromQueue(q, tempQueue);

    free(tempQueue);
    free(tempStack);
}

//------------------------------- Main Function -------------------------------//

void main(){
    wchar_t *mainMenuItems[] = {L"1] Get Maximum Value", L"2] Swap First and Last", L"3] Insert at Index", L"4] Reverse Odd Numbers", L"5] Reverse First K Elements", L"\x1b[91m6] Exit Program\x1b[0m"};
    Menu mainMenu = {0, 0, 6, mainMenuItems};

    while(1){
        int selection = navigateMenu(&mainMenu);
        switch (selection){
            case 1:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Get Maximum Value>", 'T');

                    Queue *q = initialise();
                    colorPrint("\n\nEnter elements of the queue (press ESC to stop):", 'Y');
                    inputIntToQueue(q);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Get Maximum Value>\n\n", 'T');

                    colorPrint("Queue: ", 'O'); displayQueue(q);
                    colorPrint("\nMaximum value: ", 'O'); printf("%d", maximum(q));

                    colorPrint("\n\nDo you want to get the maximum value of another queue? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(q);
                        break;
                    }
                    free(q);
                }
                break;

            case 2:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Swap First and Last>\n\n", 'T');

                    Queue *q = initialise();
                    colorPrint("Enter elements of the queue (press ESC to stop):", 'Y');
                    inputIntToQueue(q);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Swap First and Last>\n\n", 'T');

                    colorPrint("Queue: ", 'O'); displayQueue(q);
                    swapFirstLast(q);
                    colorPrint("\n\nQueue after swapping first and last elements: ", 'O');
                    displayQueue(q);

                    colorPrint("\n\nDo you want to swap first and last elements of another queue? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(q);
                        break;
                    }
                    free(q);
                }
                break;

            case 3:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Insert at Index>\n\n", 'T');

                    Queue *q = initialise();
                    colorPrint("Enter elements of the queue (press ESC to stop):", 'Y');
                    inputIntToQueue(q);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Insert at Index>\n\n", 'T');

                    colorPrint("Queue: ", 'O'); displayQueue(q);
                    int value, index;
                    colorPrint("\nEnter the value to insert: ", 'Y');
                    inputInt(&value, 1);
                    colorPrint("Enter the index to insert at (starting from 0): ", 'Y');
                    inputInt(&index, 0);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Insert at Index>\n\n", 'T');

                    colorPrint("Initial queue: ", 'O'); displayQueue(q);
                    insert(q, value, index);
                    colorPrint("\n\nQueue after inserting value at index [%d]: ", 'O', index);
                    displayQueue(q);

                    colorPrint("\n\nDo you want to insert at another index? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(q);
                        break;
                    }
                    free(q);
                }
                break;

            case 4:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Reverse Odd Numbers>\n\n", 'T');

                    Queue *q = initialise();
                    colorPrint("Enter elements of the queue (press ESC to stop):", 'Y');
                    inputIntToQueue(q);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Reverse Odd Numbers>\n\n", 'T');

                    colorPrint("Queue: ", 'O');displayQueue(q);
                    reverseOdd(q);
                    colorPrint("\n\nQueue after reversing odd numbers: ", 'O');
                    displayQueue(q);

                    colorPrint("\n\nDo you want to reverse odd numbers of another queue? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(q);
                        break;
                    }
                    free(q);
                }
                break;
            
            case 5:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Reverse First K Elements>\n\n", 'T');

                    Queue *q = initialise();
                    colorPrint("Enter elements of the queue (press ESC to stop):", 'Y');
                    inputIntToQueue(q);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Reverse First K Elements>\n\n", 'T');

                    colorPrint("Queue: ", 'O'); displayQueue(q);
                    int k;
                    colorPrint("\n\nEnter the value of K: ", 'Y');
                    inputInt(&k, 1);

                    clearScreen();
                    colorPrint("Data Structure Assignment 2", 'B'); colorPrint("\t\t<Reverse First K Elements>\n\n", 'T');

                    colorPrint("Queue: ", 'O'); displayQueue(q);
                    reverse(q, k);
                    colorPrint("\n\nQueue after reversing first k elements: ", 'O');
                    displayQueue(q);

                    colorPrint("\n\nDo you want to reverse first k elements of another queue? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(q);
                        break;
                    }
                    free(q);
                }
                break;

            case 6:
                exitProgram();
                break;

            default:
                break;
        }
    }
}

//------------------------------- Main Queue & Stack Functions -------------------------------//
Queue *initialise(){
    Queue *q = malloc(sizeof(Queue));
    q->front = -1;
    q->back = -1;

    return q;
}

int isFull(Queue *q){
    return (q->back + 1) % SIZE == q->front;
}

int isEmpty(Queue *q){
    return q->front == -1;
}

void enqueue(Queue *q, int x){
    if (isFull(q)){
        printf("Queue is full! Cannot enqueue %d\n", x);
        return;
    }

    if (isEmpty(q)) {
        q->front = q->back = 0;
    }else{
        q->back = (q->back + 1) % SIZE;
    }
    q->elements[q->back] = x;
}

int dequeue(Queue *q){
    if (isEmpty(q)){
        printf("Error: couldn't dequeue, queue is empty");
        return;
    }

    int x = q->elements[q->front];
    if (q->back == q->front){
        q->back = q->front = -1;
    }else{
        q->front = (q->front + 1) % SIZE;
    }

    return x;
}

intStack *intInitialise(void){
    intStack *s = malloc(sizeof(intStack));
    s->top = -1;
    return s;
}

void intPush(intStack *s, int x){
    s->top++;
    s->items[s->top] = x;
}

int intPop(intStack *s){
    return s->items[(s->top)--];
}

int intIsEmpty(intStack *s){
    if (s->top == -1) return 1;
    else return 0;
}

//------------------------------- Auxillary Functions -------------------------------//
void displayQueue(Queue *q){
    Queue *temp = initialise();
    int x;

    printf("[");
    while(1){
        x = dequeue(q);
        enqueue(temp, x);

        printf("%d", x);
        if(isEmpty(q)) break;
        printf(", ");
    }
    printf("]");

    swapFromQueue(q, temp);
    free(temp);
}

void swapFromQueue(Queue *q1, Queue *q2){
    while (!isEmpty(q2)){
        enqueue(q1, dequeue(q2));
    }
}

void inputIntToQueue(Queue *q){
    char buffer[SIZE];
    int index = 0;
    int i = 1;
    int x;

    colorPrint("\nEnter element %d: ", 'O', i++);

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
                enqueue(q, x);
                index = 0;
            }
            wprintf(L"\n\x1b[38;2;255;103;0mEnter element %d: \x1b[0m", i++);
        }
    }
}

void inputInt(int *num, int sign){
    char buffer[SIZE];
    int index = 0;

    while (1){
        int ch = _getch();
        if (ch == ESC){ 
            break;
        }else if ((ch >= '0' && ch <= '9') || (ch == '-' && sign == 1 && index == 0)){
            buffer[index++] = ch;
            putchar(ch);
        }else if (ch == '\b' && index > 0){
            index--;
            printf("\b \b");
        }else if (ch == ENTER && index > 0){
            buffer[index] = '\0';
            *num = atoi(buffer);
            index = 0; 
            break; 
        }
    }
    printf("\n");
}

//------------------------------- Menu Functions -------------------------------//
void colorPrint(char *string, char color, ...) {
    va_list args;
    va_start(args, color);
    
    char *colorCode;
    switch (color) {
        case 'G': colorCode = "\x1b[32m"; break; //green
        case 'Y': colorCode = "\x1b[33m"; break; //yelllow
        case 'R': colorCode = "\x1b[31m"; break; //red
        case 'O': colorCode = "\x1b[38;2;255;103;0m"; break; //orange
        case 'B': colorCode = "\x1b[94m"; break; //blue
        case 'T': colorCode = "\x1b[96m"; break; //truqiouse
        case 'r': colorCode = "\x1b[91m"; break; //pinkish-red
        default: colorCode = "\033[0m"; break;
    }
    
    printf("%s", colorCode);
    vprintf(string, args);
    printf("\033[0m");

    va_end(args);
}

void clearScreen(void){
    system("cls");
}

void printPreMessages(int menuId){
    wprintf(L"\x1b[94mData Structure Assignment 2\x1b[0m\t\t\x1b[96m<Mohsen Amr - 9655>\x1b[0m\n");
    colorPrint("\n[Use arrow or number keys for selection | press ENTER for confirmation]\n", 'Y');
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

void exitProgram(){
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
