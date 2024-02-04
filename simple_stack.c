#include<stdio.h>
#include<stdlib.h>


typedef struct StackNode {
	int data;
	struct StackNode* link;
}stackNode;

typedef struct LinkedStack {
	stackNode* top;
}Stack;

Stack* creat() {
	Stack* S = (Stack*)malloc(sizeof(Stack));
	S->top = NULL;
	return S;
}

int isEmpty(Stack* S) {
	return S->top == NULL;
}

void push(Stack* S, int x) {
	stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));
	newNode->data = x;
	newNode->link = S->top;
	S->top = newNode;
}

int pop(Stack* S) {
	stackNode* temp;
	int e;
	if (isEmpty(S)) {
		printf("[ERROR] Stack is empty\n");
		return 0;
	}
	else {
		temp = S->top;
		e = temp->data;
		S->top = temp->link;
		free(temp);
		return e;
	}
}

int peek(Stack* S) {
	if (isEmpty(S)) {
		printf("[ERROR] Stack is empty\n");
		return 0;
	}
	else {
		return S->top->data;
	}
}

void printStack(Stack* S) {
	stackNode* temp;
	temp = S->top;
	while (temp != NULL) {
		printf("[ %d ]\n", temp->data);
		temp = temp->link;
	}
	printf("\n");
}

int main()
{
	Stack s, * S;
	S = &s;
	S = creat();
	push(S, 10);
	push(S, 20);
	push(S, 30);
	push(S, 40);
	printStack(S);

	int a = pop(S);
	printStack(S);
	printf("%d is poped\n", a);

	return 0;
}