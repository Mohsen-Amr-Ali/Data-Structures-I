#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <stdarg.h>

//------------------------------- Definitions -------------------------------//
#define EMPTY_STACK -999999.0

typedef struct node{
    float data;
    struct node *next;
} node;

typedef struct{
    node *top;
} Stack;

typedef struct charNode{
    char data;
    struct charNode *next;
} charNode;

typedef struct{
    charNode *top;
} charStack;

typedef struct{
    int menuId;
    int currentSelection;
    int itemCount;
    wchar_t **menuItems;
} Menu;

//------------------------------- Function Prototypes -------------------------------//
// Main Stack Functions
Stack *initialize();
float pop(Stack *s);
void push(Stack *s, float value);
float peek(Stack *s);
int isEmpty(Stack *s);

// Main charStack Functions
charStack *charInitialize();
char charPop(charStack *s);
void charPush(charStack *s, char value);
char charPeek(charStack *s);
int charIsEmpty(charStack *s);

// Queston Functions
char *infixTopostfix(char *infix);
int precedence(char c);
float evaluatePostfix(char *postfix);

// Utility Functions
void inputStr(char **str, int *size);
int strSplit(char delimiter, char *mainStr, char ***strArray);

// Menu and UI Functions
void clearScreen(void);
void colorPrint(char *string, char color, ...);

//------------------------------- Main Function -------------------------------//
int main() {
    char continueCalculation = 'y';
    
    do{
        clearScreen();
        colorPrint("<Mohsen Amr - 9655>", 'T');
        colorPrint("\nData Structures Assignment 4", 'B');
        colorPrint("\t\t[Infix to Postfix]\n\n", 'B');
        colorPrint("Valid expressions include numbers, operators (+,-,*,/,^,%), and parentheses.\n", 'Y');
        
        // Get infix expression from user
        char *infix;
        int size = 32;
        infix = malloc(size);
        if (infix == NULL) {
            colorPrint("Memory allocation failed!\n", 'R');
            return 1;
        }
        
        colorPrint("Enter infix expression: ", 'O');
        inputStr(&infix, &size);
        
        // Convert to postfix
        char *postfix = infixTopostfix(infix);
        float value = evaluatePostfix(postfix);

        colorPrint("\nPostfix expression: ", 'G');
        colorPrint("%s\n", 'T', postfix);

        colorPrint("\nResult: ", 'G');
        colorPrint("%.2f\n", 'T', value);

        // Clean up
        free(infix);
        free(postfix);
        
        // Ask if user wants to calculate another expression
        colorPrint("\nCalculate another expression? (y/n): ", 'O');
        continueCalculation = _getch();
        
        if (tolower(continueCalculation) != 'y') {
            break;
        }
    }while(1);

    colorPrint("\n\nThank you for using the calculator!\n", 'G');
    colorPrint("Press any key to exit...", 'r');
    _getch();
    
    
    return 0;
}

//------------------------------- Main Stack Functions -------------------------------//
Stack *initialize(){
    Stack *s = malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

void push (Stack *s, float x){
    node *n = malloc(sizeof(node));
    n->data = x;
    if (isEmpty(s)){
        n->next = NULL;
        s->top = n;
    }else{
        n->next = s->top;
        s->top = n;
    }
}

float pop(Stack *s){
    if(!isEmpty(s)){
        float x = s->top->data;
        node *temp = s->top;
        s->top = s->top->next;
        free(temp);
        return x;
    } else return EMPTY_STACK;
}

float peek(Stack *s){
    if(!isEmpty(s)) return s->top->data;
    else return EMPTY_STACK;
}

int isEmpty(Stack *s){
    return (s->top == NULL);
}

//------------------------------- Char Stack Main Functions -------------------------------//
charStack *charInitialize(){
    charStack *s = malloc(sizeof(charStack));
    s->top = NULL;
    return s;
}

void charPush(charStack *s, char x){
    charNode *n = malloc(sizeof(charNode));
    n->data = x;
    if (charIsEmpty(s)){
        n->next = NULL;
        s->top = n;
    }else{
        n->next = s->top;
        s->top = n;
    }
}

char charPop(charStack *s){
    if(!charIsEmpty(s)){
        char x = s->top->data;
        charNode *temp = s->top;
        s->top = s->top->next;
        free(temp);
        return x;
    } else return '\0'; 
}

char charPeek(charStack *s){
    if(!charIsEmpty(s)) return s->top->data;
    else return '\0';
}

int charIsEmpty(charStack *s){
    return (s->top == NULL);
}

//------------------------------- Questions Functions -------------------------------//
char *infixTopostfix(char *infix){
    char **strArray;
    char *postfixStr = malloc(strlen(infix)+1);

    charStack *S = charInitialize();
    int numOfStr = strSplit(' ', infix, &strArray);
    int j=0;

    for(int i = 0; i < numOfStr; i++){
        if(strArray[i][0] == ')'){
            while(!charIsEmpty(S) && charPeek(S) != '('){
                postfixStr[j++] = charPop(S);
            };
            postfixStr[j++] = ' ';
            if(!charIsEmpty(S)) charPop(S);
        }else if(isdigit(strArray[i][0]) || (strlen(strArray[i]) > 1 && isdigit(strArray[i][1]))){
            for(int k=0; strArray[i][k] != '\0'; k++){
                postfixStr[j++] = strArray[i][k];
            }
            postfixStr[j++] = ' ';
        }else if(charIsEmpty(S))
             charPush(S, strArray[i][0]);
         else{
                while (!charIsEmpty(S) && precedence(strArray[i][0]) <= precedence(charPeek(S)) && charPeek(S) != '('){
                    postfixStr[j++] = charPop(S);
                    postfixStr[j++] = ' ';
                }
                charPush(S, strArray[i][0]);
        }
    }

    while(!charIsEmpty(S)){
        postfixStr[j++] = charPop(S);
        postfixStr[j++] = ' ';
    }

    postfixStr[--j] = '\0' ;
    return postfixStr;
}

int precedence(char c){
    switch(c){
        case '(': return 3;
        case '^': return 2;
        case '*':
        case '/':
        case '%': return 1;
        case '+':
        case '-': return 0;
    }

    return 4;
}

float evaluatePostfix(char* postfix){
    Stack *s = initialize();
    char **strArray;
    int numOfStr = strSplit(' ', postfix, &strArray);
    float x, y, value;

    for(int i = 0; i< numOfStr; i++){
        if(isdigit(strArray[i][0]) || (strlen(strArray[i]) > 1 && isdigit(strArray[i][1])))
            push(s, atof(strArray[i]));
        else{
            y = pop(s);
            x = pop(s);

            switch (strArray[i][0]){
                case '^':
                    value = pow(x, y);
                    break;
                case '*':
                    value = x * y;
                    break;
                case '/':
                    value = x / y;
                    break;
                case '%':
                    value = fmod(x, y);
                    break;
                case '+':
                    value = x + y;
                    break;
                case '-':
                    value = x - y;
                    break;
            }

            push(s, value);
        }
    }

    value = pop(s);
    return value;
}

//------------------------------- Utility Functions -------------------------------//
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


int strSplit(char delimiter, char *mainStr, char ***strArray) {
    int mainLength = strlen(mainStr);
    int numStrings = 0;
    
    *strArray = malloc(mainLength * sizeof(char *));
    if (*strArray == NULL) {
        exit(1);
    }

    int start = 0;
    for (int i = 0; i <= mainLength; i++) {
        if (mainStr[i] == delimiter || mainStr[i] == '\0') {
            int strLen = i - start;
            if (strLen > 0) {
                (*strArray)[numStrings] = malloc(strLen + 1);
                if ((*strArray)[numStrings] == NULL) {
                    exit(1);
                }

                strncpy((*strArray)[numStrings], mainStr + start, strLen);
                (*strArray)[numStrings][strLen] = '\0';
                numStrings++;
            }
            start = i + 1;
        }
    }

    return numStrings;
}
//------------------------------------------- Menu Functions -------------------------------//
void colorPrint(char *string, char color, ...){
    va_list args;
    va_start(args, color);
    
    char *colorCode;
    switch(color){
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