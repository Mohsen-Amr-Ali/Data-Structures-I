#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>

//------------------------------------- Definitions -------------------------------------//
typedef struct node{
    struct node *left;
    char *key;
    struct node *right;
    int height;
} node;

typedef struct{
    int menuId;
    int currentSelection;
    int itemCount;
    wchar_t **menuItems;
} Menu;

//------------------------------------- Function Prototypes -------------------------------------//
// AVL Tree Functions
int getHeight(node *n);
node *createNode(char *key);
int getBalance(node *n);
node *rotateRight(node *y);
node *leftRotate(node *x);
node *insert(node *n, char *key);
node *search(node *n, char *key);
void freeTree(node *root);

// Dictionary Functions
node *predecessor(node *root, node *n);
node *successor(node *root, node *n);
void correctWords(node *root, node *n, char *key, char ***suggestionsArr);

// File Reading Functions
int loadDictionaryToTree(node **root, char *filePath);
char *getDictPath();

// Menu and UI Functions
void clearScreen(void);
void colorPrint(char *string, char color, ...);
void printPreMessages(int menuId);
void displayMenu(const Menu *menu);
int navigateMenu(Menu *menu);
void exitProgram(void);

// Utility Functions
void inputStr(char **str, int *size);
int strSplit(char *delimiter, char *mainStr, char ***strArray);

//------------------------------------- AVL Tree Functions -------------------------------------//
int getHeight(node *n){
    if (n == NULL) return 0;
    return n->height;
}

node *createNode(char *key){
    node *newNode = malloc(sizeof(node));
    newNode->key = malloc(strlen(key) + 1);
    strcpy(newNode->key, key);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int getBalance(node *n){
    if (n == NULL) return 0;
    return getHeight(n->left) - getHeight(n->right);
}

node *rotateRight(node *y){
    node *x = y->left;
    node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x;
}

node *leftRotate(node *x){
    node *y = x->right;
    node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

node *insert(node *n, char *key){
    if (n == NULL) return createNode(key);

    int cmp = strcasecmp(key, n->key);
    if (cmp < 0)
        n->left = insert(n->left, key);
    else if (cmp > 0)
        n->right = insert(n->right, key);
    else return n;

    n->height = 1 + max(getHeight(n->left), getHeight(n->right));

    int balance = getBalance(n);

    if (balance > 1 && strcasecmp(key, n->left->key) < 0) return rotateRight(n);

    if (balance < -1 && strcasecmp(key, n->right->key) > 0) return leftRotate(n);

    if (balance > 1 && strcasecmp(key, n->left->key) > 0){
        n->left = leftRotate(n->left);
        return rotateRight(n);
    }

    if (balance < -1 && strcasecmp(key, n->right->key) < 0){
        n->right = rotateRight(n->right);
        return leftRotate(n);
    }

    return n;
}

node *search(node *n, char *key){
    if (n == NULL) return n;
    
    int cmp = strcasecmp(key, n->key);
    if (cmp == 0) return n;
    if (cmp < 0) return search(n->left, key);
    return search(n->right, key);
}

void freeTree(node *root){
    if (root == NULL) return;
    
    freeTree(root->left);
    freeTree(root->right);
    
    free(root->key);
    free(root);
}

//-------------------------------- Dictionary Functions -------------------------------//
node *predecessor(node *root, node *n){
    if(n == NULL) return NULL;
    node *pred = NULL;

    if(n->left){
        pred = n->left;
        while(pred->right) pred = pred->right;
        return pred;
    }
    
    while(root){
        if(n->key == root->key) break;
        if(strcasecmp(n->key, root->key) < 0){
            root = root->left;
        }else{
            pred = root;
            root = root->right;
        }
    }

    return pred;
}

node *successor(node *root, node *n){
    if(n == NULL) return NULL;
    node *succ = NULL;

    if(n->right){
        succ = n->right;
        while(succ->left) succ = succ->left;
        return succ;
    }
    
    while(root){
        if(strcasecmp(n->key, root->key) == 0) break;
        if(strcasecmp(n->key, root->key) > 0){
            root = root->right;
        }else{
            succ = root;
            root = root->left;
        }
    }

    return succ;
}

void correctWords(node *root, node *n, char *key, char ***suggestionsArr){
    int cmp = strcasecmp(key, n->key);
    
    if(cmp < 0){
        if(n->left == NULL){
            strcpy((*suggestionsArr)[0], n->key);
            strcpy((*suggestionsArr)[1], predecessor(root, n)->key);
            strcpy((*suggestionsArr)[2], successor(root, n)->key);
            return;
        }else{
            correctWords(root, n->left, key, suggestionsArr);
        }
    }else if(cmp > 0){
        if(n->right == NULL){
            strcpy((*suggestionsArr)[0], n->key);
            strcpy((*suggestionsArr)[1], predecessor(root, n)->key);
            strcpy((*suggestionsArr)[2], successor(root, n)->key);
            return;
        }else{
            correctWords(root, n->right, key, suggestionsArr);
        }
    }
}

//------------------------------- File Reading Functions -------------------------------//
int loadDictionaryToTree(node **root, char *filePath){
    FILE *file = fopen(filePath, "r");
    if (file == NULL) return -1;

    int size = 0;
    char word[100];
    while (fgets(word, sizeof(word), file) != NULL){
        word[strcspn(word, "\n")] = 0;
        *root = insert(*root, word);
        size++;
    }

    fclose(file);
    return size;
}

//El function de 3ashan ye3raf el path beta3 el dictionary file, in case the .exe file is in an output folder
char *getDictPath(char *fileName){
    char exePath[MAX_PATH]; //feeha el maximum size of a path men el windows library
    GetModuleFileName(NULL, exePath, MAX_PATH); //Null 3alashan heya el current file

    char exeDir[MAX_PATH];
    strcpy(exeDir, exePath);
    char *lastSlash = strrchr(exeDir, '\\');
    if (lastSlash != NULL){
        *lastSlash = '\0';  // remove the .exe file from teh name
    }

    char *finalPath = malloc(MAX_PATH);

    //Check if the Dictionary.txt is in the same directory as the .exe file
    char path1[MAX_PATH];
    snprintf(path1, sizeof(path1), "%s\\%s", exeDir, fileName);

    FILE *fp = fopen(path1, "r");
    if (fp != NULL){
        fclose(fp);
        return strcpy(finalPath, path1);
    }

    //Law La2, shoof el folder el barra el output or bin folder 3ala tool
    char *secondLastSlash = strrchr(exeDir, '\\');
    if (secondLastSlash != NULL){
        *secondLastSlash = '\0';  //remove the last folder (e.g., "output")
    } else {
        return NULL;  // No higher directory to check
    }

    char path2[MAX_PATH];
    snprintf(path2, sizeof(path2), "%s\\%s", exeDir, fileName);

    fp = fopen(path2, "r");
    if (fp != NULL){
        fclose(fp);
        return strcpy(finalPath, path2);  
    }

    // Not found in either location, return NULL
    free(finalPath);
    return NULL;
}

//------------------------------- Main Function -------------------------------//
int main(){
    char *fileName = "Dictionary.txt";
    char *filePath = getDictPath(fileName);
    if (filePath == NULL){
        colorPrint(" '%s' file not found.\n", 'R', fileName);
        printf("Please make sure it's either in the same directory as the executable file or in its parent directory.\n");
        Sleep(15000);
        return 1;
    }
    
    node *root = NULL;
    int dictSize = loadDictionaryToTree(&root, filePath);

    do{
        clearScreen();
        colorPrint("<Mohsen Amr - 9655>", 'T');
        colorPrint("\nData Structures Assignment 4", 'B'); colorPrint("\t\t[AVL Tree Dictionary]\n\n", 'B');
        colorPrint("Dictionary loaded successfully", 'G'); colorPrint("\t|", 'B'); colorPrint(" Size = %d \t Height = %d\n\n", 'O', dictSize, root->height);

        int size = 32;
        char *inputSentence = malloc(size);

        colorPrint("Please enter a sentence to check for spelling errors:\n", 'Y');
        inputStr(&inputSentence, &size);

        char **strArr;
        int wordNum = strSplit(" ,.!?;:()[]{}\"", inputSentence, &strArr);
        char ***suggestionsArr = malloc(wordNum * sizeof(char **));
        
        
        for (int i = 0; i < wordNum; i++){
            node *found = search(root, strArr[i]);
            if(found){
                suggestionsArr[i] = NULL;
            }else{
                suggestionsArr[i] = malloc(3 * sizeof(char *));
                for(int j = 0; j < 3; j++){
                    suggestionsArr[i][j] = malloc(100);
                }
                correctWords(root, root, strArr[i], &suggestionsArr[i]);
            }
        }

        printf("\n\n");
        colorPrint("Sentence: ", 'O');
        int wordIdx = 0;
        int len = strlen(inputSentence);
        for (int i = 0; i < len; ) {
            if (strchr(" ,.!?;:()[]{}\"", inputSentence[i]) != NULL) {
                colorPrint("%c", 'W', inputSentence[i]);
                i++;
            } else {
                if (suggestionsArr[wordIdx] == NULL) {
                    colorPrint("%s", 'G', strArr[wordIdx]);
                } else {
                    colorPrint("%s", 'U', strArr[wordIdx]);
                }
                i += strlen(strArr[wordIdx]);
                wordIdx++;
            }
        }

        printf("\n");
        colorPrint("Suggestions:\n", 'O');
        for (int i = 0; i < wordNum; i++){
            if(suggestionsArr[i] != NULL){
                printf("%s", strArr[i]);
                colorPrint("-> ", 'Y');
                for(int j = 0; j < 3; j++){
                    if(suggestionsArr[i][j] != NULL){
                        colorPrint("%s", 'w', suggestionsArr[i][j]);
                        if(j < 2){
                            colorPrint(", ", 'Y');
                        }else{
                            colorPrint(".", 'Y');
                        }
                    }
                }
                printf("\n");
            }
        }

        printf("\n");
        colorPrint("Do you want to check another sentence? (Y for yes): ", 'Y');
        char input;
        scanf(" %c", &input);
        getchar();
        if (tolower(input) != 'y'){
            exitProgram();
        } 
    }while(1);
}

//------------------------------- Menu and UI Functions -------------------------------//
void clearScreen(void){
    system("cls");
}

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
        case 'U': colorCode = "\x1b[31;4m"; break; //red and underlined
        default: colorCode = "\033[0m"; break;
    }
    
    printf("%s", colorCode);
    vprintf(string, args);
    printf("\033[0m");

    va_end(args);
}

void exitProgram(void){
    clearScreen();
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

int strSplit(char *delimiters, char *mainStr, char ***strArray){
    int mainLength = strlen(mainStr);
    int numStrings = 0;
    
    *strArray = malloc(mainLength * sizeof(char *));
    if (*strArray == NULL){
        exit(1);
    }

    int start = 0;
    for (int i = 0; i <= mainLength; i++) {
        if (mainStr[i] == '\0' || strchr(delimiters, mainStr[i]) != NULL){
            int strLen = i - start;
            if (strLen > 0) {
                (*strArray)[numStrings] = malloc(strLen + 1);
                if ((*strArray)[numStrings] == NULL){
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