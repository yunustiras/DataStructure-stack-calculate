#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define BUFFER_SIZE 2048
#define STR_SIZE 128
#define OPERATION_SIZE 256

typedef struct Stack Stack;
typedef struct Operation Operation;

struct Stack
{
    char *data;
    Stack *next;
};

struct Operation
{
    char name;
    int value;
    Stack *head;
    char *posfix;
};

Stack *stackPush(Stack *, char *);
char *stackPop(Stack **);
int toInteger(char *);
Operation *createPosfix(char *);
//debug functions
void printStack(Stack *head);
//file functions
char *readInputsFromFile(char *);

int main()
{
    Operation *operations;
    char *input;
    input = readInputsFromFile("input.txt");
    printf("%s\n", input);
    operations = createPosfix(input);
    return 0;
}

// will add data to start of linked list
Stack *stackPush(Stack *head, char *data)
{
    Stack *newStack = (Stack *)malloc(sizeof(Stack));
    newStack->data = (char *)malloc(sizeof(char) * STR_SIZE);
    newStack->next = head;
    strcpy(newStack->data, data);
    return newStack;
}

//will return first element
char *stackPop(Stack **head)
{
    if (*head == 0)
        return 0;
    Stack *tmp = *head;
    *head = (*head)->next;
    //free(tmp->data);
    //free(tmp);
    return tmp->data;
}

//Err döndürse güzel olabilir bakarız
int toInteger(char *str)
{
    int i, num;
    i = num = 0;
    while (str[i] != '\0')
    {
        num *= 10;
        num += str[i] - '0';
        i++;
    }

    return num;
}

Operation *createPosfix(char *str)
{
    Operation *operations = (Operation *)malloc(sizeof(Operation) * OPERATION_SIZE);
    int operationIndex = 0;
    char opFinded = 0;
    char tmpStr[STR_SIZE];
    int tmpStrIndex = 0;
    int strIndex = 0;
    //printf("B1\n");
    while (str[strIndex] != '\0')
    {

        if (!opFinded)
        {
            //printf("B1\n");
            operations[operationIndex].head = 0;
            operations[operationIndex].name = str[strIndex];
            operations[operationIndex].head = 0;
            operations[operationIndex].posfix = (char *)malloc(sizeof(char) * BUFFER_SIZE);
            operations[operationIndex].posfix[0] = NULL;
            strIndex += 4;
            opFinded = 1;
            continue;
        }
        if (str[strIndex] == ' ')
        {
            if (tmpStrIndex > 0)
            {
                tmpStr[tmpStrIndex] = NULL;
                if (operations[operationIndex].posfix[0] != NULL)
                {
                    sprintf(operations[operationIndex].posfix, "%s %s", operations[operationIndex].posfix, tmpStr);
                }
                else
                {
                    sprintf(operations[operationIndex].posfix, "%s", tmpStr);
                }
                tmpStrIndex = 0;
            }
        }
        else if (str[strIndex] == ';')
        {
            printf("STACK : ");
            printStack(operations[operationIndex].head);
            printf(" POSFIX : %s \n", operations[operationIndex].posfix);
            operationIndex++;
            strIndex += 2;
            opFinded = 0;
        }
        else if (str[strIndex] == ')')
        {
            char *data = stackPop(&(operations[operationIndex].head));
            while (data[0] != '(')
            {
                sprintf(operations[operationIndex].posfix, "%s %s", operations[operationIndex].posfix, data);
                data = stackPop(&(operations[operationIndex].head));
            }
        }
        else if (str[strIndex] == '(' || str[strIndex] == '*' || str[strIndex] == '/')
        {
            char data[2];
            data[0] = str[strIndex];
            data[1] = NULL;
            operations[operationIndex].head = stackPush(operations[operationIndex].head, data);
        }
        else
        {
            tmpStr[tmpStrIndex] = str[strIndex];
            tmpStrIndex++;
        }
        /*
        else if (str[strIndex] == '+')
        {
        }
        else if (str[strIndex] == '/')
        {
        }
        else if (str[strIndex] == '-')
        {
        }
        */
        strIndex++;
    }
    return operations;
}
//debug functions
void printStack(Stack *head)
{
    while (head)
    {
        printf("%s ", head->data);
        head = head->next;
    }
}

//file functions
char *readInputsFromFile(char *filename)
{
    FILE *file = fopen(filename, "r");
    char *buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    fread(buffer, BUFFER_SIZE, 1, file);
    fclose(file);
    return buffer;
}
