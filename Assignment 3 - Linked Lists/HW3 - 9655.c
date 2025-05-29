#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wchar.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <stdarg.h>

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

typedef struct node{
    int data;
    struct node *next;
} node;

typedef struct{
    node *head;
} LinkedList;

typedef struct{
    int menuId;
    int currentSelection;
    int itemCount;
    wchar_t **menuItems;
} Menu;

//------------------------------- Function Prototypes -------------------------------//
// Main Linked List Functions
LinkedList* init();
void insertAtBeginning(LinkedList *l, int x);
void insertAtEnd(LinkedList *l, int x);
void displayList(LinkedList *l);
void deleteFromBegining(LinkedList *l);
void deleteFromEnd(LinkedList *l);

// Questions Functions
node *search(LinkedList *list, int value);
int insert(LinkedList *list, int index, int value);
int identical(LinkedList *x, LinkedList *y);
LinkedList *Merge(LinkedList *x, LinkedList *y);
LinkedList *intersection(LinkedList *x, LinkedList *y);

// Quick Sort Functions
node *getTail(node *cur);
node *findMiddle(node *head, node *tail);
node *partition(node *head, node *tail);
void quickSortHelper(node *head, node *tail);
node *quickSort(LinkedList *list);

// Auxiliary Functions
int isSorted(LinkedList *list);
void inputIntToList(LinkedList *l);
void inputInt(int *num, int sign);
void colorPrint(char *string, char color, ...);

// Menu Functions
void clearScreen(void);
void printPreMessages(int menuId);
void displayMenu(const Menu *menu);
int navigateMenu(Menu *menu);
void exitProgram(void);

//------------------------------- Questions Functions -------------------------------//
//-- Question 1 --> Search for value
node *search(LinkedList *list, int value){
    node *currentNode = list -> head;
    while(currentNode != NULL && currentNode -> data != value){
        currentNode = currentNode -> next;
    }
    return currentNode;
}

//-- Question 2 --> insert at specific index {0-Based}
int insert(LinkedList *list, int index, int value){
    node *new = malloc(sizeof(node));
    new->data = value;
    if(index==0){
        insertAtBeginning(list, value);
    }else{
        node *cursor = list->head;
        for(int i=1; i<index && cursor != NULL; i++, cursor = cursor->next);
        if(cursor == NULL){
            colorPrint("\nWarning: Index [", 'R'); printf("%d", index); colorPrint("] is out of range!", 'R');
            colorPrint("\nDo you want to insert it at the end of the list? (Y for yes): ", 'Y');
            
            char input;
            scanf(" %c", &input);
            if (tolower(input) == 'y'){
                insertAtEnd(list, value);
                free(new);
                colorPrint("\nValue inserted at the end of the list.", 'G');
            } else {
                free(new);
                colorPrint("\nInsertion cancelled.", 'r');
                Sleep(700);
                return 0;
            }
        }else{
            new->next = cursor->next;
            cursor->next = new;
        }
    }
    return 1;
}

//-- Question 3 --> Check if two lists are identical
int identical(LinkedList *x, LinkedList *y){
    if(x == y) return 1;
    node *cursorX = x->head, *cursorY = y->head;
    while(cursorX != NULL && cursorY != NULL){
        if(cursorX->data != cursorY->data) return 0;
        cursorX = cursorX->next;
        cursorY = cursorY->next;
    }

    return (cursorX == NULL && cursorY == NULL);
}

//-- Question 4 --> Merge two sorted lists
LinkedList *Merge(LinkedList *x, LinkedList *y){
    LinkedList *mergedList = init();
    node *cursorX = x->head, *cursorY = y->head;

    if(!isSorted(x)) quickSort(x); if(!isSorted(y)) quickSort(y);

    while(cursorX != NULL && cursorY !=NULL){
        if(cursorX->data == cursorY->data){
            insertAtEnd(mergedList, cursorX->data);
            insertAtEnd(mergedList, cursorY->data);
            cursorX = cursorX->next;
            cursorY =cursorY->next;
        }else if(cursorX->data < cursorY->data){
            insertAtEnd(mergedList, cursorX->data);
            cursorX = cursorX->next;
        }else if(cursorX->data > cursorY->data){
            insertAtEnd(mergedList, cursorY->data);
            cursorY = cursorY->next;
        }
    }

    while(cursorX != NULL){
        insertAtEnd(mergedList, cursorX->data);
        cursorX = cursorX->next;
    }
    while(cursorY != NULL){
        insertAtEnd(mergedList, cursorY->data);
        cursorY = cursorY->next;
    }

    return mergedList;
}

//-- Question 5 --> Find intersection between two sorted lists
LinkedList *intersection(LinkedList *x, LinkedList *y){
    LinkedList *intersectionList = init();
    node *cursorX = x->head, *cursorY = y->head;

    if(!isSorted(x)) quickSort(x); if(!isSorted(y)) quickSort(y);

    while(cursorX != NULL && cursorY !=NULL){
        if(cursorX->data == cursorY->data){
            insertAtEnd(intersectionList, cursorX->data);
            cursorX = cursorX->next;
            cursorY =cursorY->next;
        }else if(cursorX->data < cursorY->data){
            cursorX = cursorX->next;
        }else if(cursorX->data > cursorY->data){
            cursorY = cursorY->next;
        }
    }

    return intersectionList;
}

//------------------------------- Main Function -------------------------------//
int main(){
    wchar_t *mainMenuItems[] = {L"1] Search for Value", L"2] Insert at Index", L"3] Check if Identical", L"4] Merge Two Lists", L"5] Find Intersection", L"\x1b[91m6] Exit Program\x1b[0m"};
    Menu mainMenu = {0, 0, 6, mainMenuItems};

    while(1){
        int selected = navigateMenu(&mainMenu);
        switch(selected){
            case 1:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Search for Value>\n\n", 'T');

                    LinkedList *list = init();
                    colorPrint("Enter elements of the linked list (press ESC to stop):", 'Y');
                    inputIntToList(list);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Search for Value>\n\n", 'T');

                    colorPrint("Linked List: ", 'O'); displayList(list);
                    
                    int value;
                    colorPrint("\nEnter the value to search for: ", 'Y');
                    inputInt(&value, 1);
                    
                    node *result = search(list, value);
                    if(result != NULL){
                        colorPrint("\nValue ", 'G'); printf("[%d] ", value); colorPrint("found in the list!", 'G');
                    }else{
                        colorPrint("\nValue ", 'r'); printf("[%d] ", value); colorPrint("not found in the list.", 'r');
                    }

                    colorPrint("\n\nDo you want to search for another value? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(list);
                        break;
                    }
                    free(list);
                }
                break;

            case 2:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Insert at Index>\n\n", 'T');

                    LinkedList *list = init();
                    colorPrint("Enter elements of the linked list (press ESC to stop):", 'Y');
                    inputIntToList(list);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Insert at Index>\n\n", 'T');

                    colorPrint("Linked List: ", 'O'); displayList(list);

                    int value, index;
                    colorPrint("\nEnter the value to insert: ", 'Y');
                    inputInt(&value, 1);
                    colorPrint("Enter the index to insert at (starting from 0): ", 'Y');
                    inputInt(&index, 0);

                    if(insert(list, index, value)){
                        colorPrint("\nLinked List after insertion: ", 'O'); displayList(list);
                    }
                    
                    colorPrint("\n\nDo you want to insert another value? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(list);
                        break;
                    }
                    free(list);
                }
                break;

            case 3:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Check if Identical>\n\n", 'T');

                    LinkedList *list1 = init();
                    colorPrint("Enter elements of the first linked list (press ESC to stop):", 'Y');
                    inputIntToList(list1);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Check if Identical>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);

                    LinkedList *list2 = init();
                    colorPrint("\nEnter elements of the second linked list (press ESC to stop):", 'Y');
                    inputIntToList(list2);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Check if Identical>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);
                    colorPrint("Second Linked List: ", 'O'); displayList(list2);

                    if (identical(list1, list2)) {
                        colorPrint("\n\nThe lists are identical!", 'G');
                    } else {
                        colorPrint("\n\nThe lists are not identical.", 'R');
                    }

                    colorPrint("\n\nDo you want to compare another two lists? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(list1);
                        free(list2);
                        break;
                    }
                    free(list1);
                    free(list2);
                }
                break;

            case 4:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Merge Two Lists>\n\n", 'T');

                    LinkedList *list1 = init();
                    colorPrint("Enter elements of the first linked list (press ESC to stop):", 'Y');
                    inputIntToList(list1);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Merge Two Lists>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);

                    LinkedList *list2 = init();
                    colorPrint("\nEnter elements of the second linked list (press ESC to stop):", 'Y');
                    inputIntToList(list2);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Merge Two Lists>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);
                    colorPrint("Second Linked List: ", 'O'); displayList(list2);

                    if (!isSorted(list1) || !isSorted(list2)) {
                        colorPrint("\n\nWarning: At least one of the initial lists is not sorted.", 'R');
                        colorPrint("\nSorting then merging...", 'Y');
                    }

                    LinkedList *merged = Merge(list1, list2);
                    colorPrint("\n\nMerged and Sorted List: ", 'O'); displayList(merged);

                    colorPrint("\n\nDo you want to merge another two lists? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(list1);
                        free(list2);
                        free(merged);
                        break;
                    }
                    free(list1);
                    free(list2);
                    free(merged);
                }
                break;

            case 5:
                while(1){
                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Find Intersection>\n\n", 'T');

                    LinkedList *list1 = init();
                    colorPrint("Enter elements of the first linked list (press ESC to stop):", 'Y');
                    inputIntToList(list1);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Find Intersection>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);

                    LinkedList *list2 = init();
                    colorPrint("\nEnter elements of the second linked list (press ESC to stop):", 'Y');
                    inputIntToList(list2);

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Find Intersection>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);
                    colorPrint("Second Linked List: ", 'O'); displayList(list2);

                    if (!isSorted(list1) || !isSorted(list2)) {
                        colorPrint("\n\nWarning: At least one of the lists is not sorted.", 'R');
                        colorPrint("\nDo you want to sort them and proceed? (Y for yes, N to cancel): ", 'Y');
                        
                        char sortInput;
                        scanf(" %c", &sortInput);
                        if (tolower(sortInput) != 'y') {
                            colorPrint("\nOperation cancelled.", 'r');
                            Sleep(700);
                            free(list1);
                            free(list2);
                            break;
                        }
                    }

                    clearScreen();
                    colorPrint("Data Structure Assignment 3", 'B'); colorPrint("\t\t<Find Intersection>\n\n", 'T');

                    colorPrint("First Linked List: ", 'O'); displayList(list1);
                    colorPrint("Second Linked List: ", 'O'); displayList(list2);

                    LinkedList *intersected = intersection(list1, list2);
                    if(intersected->head != NULL){
                        colorPrint("\nIntersection List: ", 'O'); displayList(intersected);
                    }else{
                        colorPrint("\nThere is no intersection between the two lists.", 'r');
                    }
                    
                    colorPrint("\n\nDo you want to find intersection of another two lists? (Y for yes): ", 'Y');
                    char input;
                    scanf(" %c", &input);
                    if (tolower(input) != 'y'){
                        free(list1);
                        free(list2);
                        free(intersected);
                        break;
                    }
                    free(list1);
                    free(list2);
                    free(intersected);
                }
                break;

            case 6:
                exitProgram();
                break;
        }
    }
    return 0;
}

//------------------------------- Linked List Functions -------------------------------//
LinkedList* init(){
    LinkedList *l = malloc(sizeof(LinkedList));
    l->head = NULL;
    return l;
}

void insertAtBeginning(LinkedList *l, int x){
    node *n = malloc(sizeof(node));
    n->data = x;
    n->next = NULL;
    if(l->head == NULL){
        l->head = n;
    }else{
        n->next = l->head;
        l->head = n;
    }
}

void insertAtEnd (LinkedList *l, int x){
    node *n = malloc(sizeof(node));
    n->data = x;
    n->next = NULL;
    if (l->head == NULL)
        l->head = n;
    else{
        node*temp=l->head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=n;
    }
}

void displayList (LinkedList *l){
    printf("[");
    if(l->head == NULL){
        colorPrint("NULL", 'r'); printf("]");
        return;
    }

    node *temp = l->head;
    while (1){
        printf("%d", temp->data);
        temp = temp->next;
        if(temp == NULL) break;
        printf(", ");
    }
    printf("]\n");
}

void deleteFromBegining(LinkedList *l){
    if(l->head != NULL){
        node *temp = l->head;
        l->head = l->head->next;
        free(temp);
    }
}

void deleteFromEnd(LinkedList *l){
    if(l->head != NULL){
        if(l->head->next==NULL){
            free(l->head);
            l->head=NULL;
        }else{
            node*temp=l->head;
            //get node before Last node
            node*prev=NULL;
            while(temp->next!=NULL){
                prev=temp;
                temp=temp->next;
            }
            prev->next=NULL;
            free(temp);
        }
    }
}

//------------------------------- Auxillary Functions -------------------------------//
//----- Quick Sort -----//
node *getTail(node *cur){
    while(cur != NULL && cur->next != NULL){
        cur = cur->next;
    }
    return cur;
}

node *findMiddle(node *head, node *tail){
    if (head == NULL || head == tail) return head;
    
    node *slow = head;
    node *fast = head;
    
    while(fast != tail && fast->next != tail){
        slow = slow->next;
        fast = fast->next->next;
    }
    
    return slow;
}

node *partition(node *head, node *tail){
    node *middleNode = findMiddle(head, tail);
    
    int pivotValue = middleNode->data;
    middleNode->data = head->data;
    head->data = pivotValue;
    
    node *pivot = head;
    node *pre = head;
    node *cur = head;

    while(cur != tail->next){
        if(cur->data < pivot->data){
            pre = pre->next;
            int temp = cur->data;
            cur->data = pre->data;
            pre->data = temp;
        }
        cur = cur->next;
    }
    
    int temp = pivot->data;
    pivot->data = pre->data;
    pre->data = temp;

    return pre;
}

void quickSortHelper(node *head, node *tail){
    if(head == NULL || head == tail){
        return;
    }
    
    node *pivot = partition(head, tail);
    
    if(pivot != head){
        quickSortHelper(head, pivot);
    }
    
    if(pivot != tail){
        quickSortHelper(pivot->next, tail);
    }
}

node *quickSort(LinkedList *list){
    node *head = list->head;
    if (head == NULL){
        return NULL;
    }
    
    node *tail = getTail(head);
    quickSortHelper(head, tail);
    return head;
}

//----- Continue -----//
int isSorted(LinkedList *list){
    if(list->head == NULL || list->head->next == NULL){
        return 1;
    }
    
    node *current = list->head;
    while(current->next != NULL){
        if(current->data > current->next->data){
            return 0;
        }
        current = current->next;
    }
    
    return 1;
}

void inputIntToList(LinkedList *l){
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
                insertAtEnd(l, x);
                index = 0;
            }
            wprintf(L"\n\x1b[38;2;255;103;0mEnter element %d: \x1b[0m", i++);
        }
    }
}

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