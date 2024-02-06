#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdbool.h>  //path�Լ� ��������� ������ �߰��� �ش�

//gotoxy�� ������� ������ ���ͳݺ��� �������.
void gotoxy(int x, int y)

{

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

#define m 12
#define p 12
#define Max_dir 8	//�� ������ �̵������� ��
#define Timestep 1000 //1��

int maze[m + 2][p + 2];  //maze�� total ũ�� 14 * 14
int mark[m + 2][p + 2];  //mark���� total ũ�� 14 * 14

typedef struct Aoff {
	short int vert;
	short int horiz;
}offsets;  //dir�� ���� ���� ����ü

typedef struct St_element { //���ÿ� ������ ������
	short int row;
	short int col;
	short int dir;
}ty_element;

offsets move[Max_dir] = { {-1,0}, {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} }; //���ʷ� ��, �ϵ�, ��, ����, ��,����, ��, �ϼ�

//typedef struct listNode* listPointer; //���ὺ���� ���

typedef struct StackNode {	//���ὺ���� ���
	ty_element data;
	struct listNode* link;
}listNode;

typedef struct LinkedStack { //������ top�κ��� ����ü�� ����
	listNode* top;
}Stack;

Stack* creat() { //������ �ʱ�ȭ ���ִ� creat()�Լ�
	Stack* S = (Stack*)malloc(sizeof(Stack));
	S->top = NULL;
	return S;
}
 
int isEmpty(Stack* S) { //������ �������� Ȯ���� �ִ� �Լ�
	return S->top == NULL;
}


void push(Stack* S, ty_element x) { //���ÿ� push������ �����ϴ� �Լ�
	listNode* newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data.col = x.col;
	newNode->data.row = x.row;
	newNode->data.dir = x.dir;
	newNode->link = S->top;
	S->top = newNode;
}

ty_element pop(Stack* S) { //���ÿ� pop������ �����ϴ� �Լ�
	listNode* temp;
	ty_element e;
	if (isEmpty(S)) {
		printf("[ERROR] ������ ����ֽ��ϴ�.\n");
	}
	else {
		temp = S->top;
		e = temp->data;
		S->top = temp->link;
		free(temp);
		return e;
	}
}

void reverse(Stack* S) { // reverse�Լ�
	listNode* q, * r, * s;

	q = S->top;
	r = NULL;

	while (q != NULL) {
		s = r;
		r = q;
		q = q->link;
		r->link = s;
	}
	S->top = r;
}

bool path(int sy, int sx, int dy, int dx) {
	//listPointer top = NULL; //������ top����, �ʱ⿡�� NULL
	Stack s, * S;
	S = &s;
	S = creat(); // ���� �ʱ�ȭ

	int i, j, row, col, nextRow, nextCol, dir, basex, basey, ocount;
	int found, num_bktrack = 0, path_length = 0;
	int EXIT_ROW = dy, EXIT_COL = dx; //��ǥ��
	ty_element position;

	//�̷θ� �׸��� �κ�
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	basex = presentCur.dwCursorPosition.X;
	basey = presentCur.dwCursorPosition.Y;

	for (i = 0; i < m + 2; i++) {  //maze�� �׸���
		for (j = 0; j < p + 2; j++) {
			gotoxy(j + basex, i + basey);
			printf("%1d", maze[i][j]);
		}
	}
	printf("\n");

	//�� ���ÿ� ������� �ִ´�. (�ʱ�ȭ)
	position.row = sy;
	position.col = sx;
	position.dir = 0;
	push(S, position);

	//mark�����ʱ�ȭ, �ٿ������ ������ ���� ���� ��� 0���� �ʱ�ȭ�Ѵ�.
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[i + 1][j + 1] = 0;
		}
	}

	if ((maze[sy][sx] == 1) || (maze[dy][dx] == 1)) {
		printf("�Է��� ��ǥ�� ���� ���Դϴ�.\nŽ���� �ź��մϴ�.\n");
		return false;
	}

	
	mark[sy][sx] = 1; //������ġ�� �湮����
	gotoxy(sx + basex, sy + basey);
	printf(">");    // ������� ǥ���Ѵ�
	Sleep(Timestep);

	found = 0;

	while (!isEmpty(S)) { //������ �� ��ǥ�� ���ÿ� push ���� �ʴ´� �׷��� ������ �� ��ǥ�� �˰��ִ� row,col,nextRow,nextCol�� �̿��Ұ��̴�
		position = pop(S);
		row = position.row;
		col = position.col;
		dir = position.dir;
		while (dir < Max_dir) {
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if ((nextRow == EXIT_ROW) && (nextCol == EXIT_COL)) { //�������� ����
				found = 1;
				gotoxy(nextCol + basex, nextRow + basey);
				printf("<");
				
				break;
			}
			else if ((maze[nextRow][nextCol] == 0) && (mark[nextRow][nextCol] == 0)) {//dir�� ���� ��ġ�� ������ ��ġ
				position.row = row;
				position.col = col;
				position.dir = ++dir;
				push(S, position);
				row = nextRow;
				col = nextCol;
				gotoxy(col + basex, row + basey);
				printf("*");
				Sleep(Timestep);

				dir = 0; //���� �̵��� dir = 0���� �ٽ� �õ�
				mark[row][col] = 1; //�̵������� mark

			}
			else
				++dir; // ++dir�� �����ν� ���� dir���� ����
		}
		if (found)
			break;

		gotoxy(col + basex, row + basey);  // ��Ʈ��ŷ ���� ���ǥ�ø� ����� 0 �� ����.
		printf("%c", '$');


		num_bktrack++;
		Sleep(Timestep);

	}
	gotoxy(0, basey + m + 2);  // �̷ο� ��ġ�� �ʰ� �Ʒ��� ����� �ǰ� Ŀ���� �ű�.
	printf("\n");

	if (found) {
		printf("���: [ row , col ]: \n");
		reverse(S); // ������ �������ؼ�
		ocount = 0;
		while (!isEmpty(S)) { //������ �����
			position.col = S->top->data.col;
			position.row = S->top->data.row;
			position.dir = S->top->data.dir;
			if ((ocount != 0) && (ocount % 6 == 0))  //���ÿ��� ������ ��ǥ�� 6���� ���� ����ϱ� ����
				printf("\n");
			printf("[ %2d , %2d ]", position.row, position.col);
			ocount++;
			S->top = S->top->link;
		}
		printf("[ %2d , %2d ]", row, col);  //��� ���ÿ��� ������ �� ��ǥ�� push���� ���� �׷��� �� �� �������� ����Ʈ ���ִ� ����
		printf("[ %2d , %2d ]\n", EXIT_ROW, EXIT_COL);
		path_length = ocount + 2;
		printf("��α���: %d, ��Ʈ��ŷ�� = %d\n", path_length, num_bktrack);
	}
	else
		printf("������ ��ΰ� �����ϴ�.\n");
	return true;

}



int main()
{
	FILE* fp;
	int hoho; //���� ���� count
	int sty, stx, edy, edx; //��ǥ��

	//������ ����
	fp = fopen("maze_14_2.txt", "r");
	if (fp == NULL) {
		printf("������ ���µ� �����߽��ϴ�.\n");
		return 1;
	}
	//maze�� ���Ͽ��� ���� ������ ����
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < p + 2; j++) {
			fscanf(fp, "%d", &maze[i][j]);
		}
	}
	
	while (1) {

		printf("�����, ��ǥ����?(4���� ����; ����� Control+D Enter}: ");
		hoho = scanf("%d%d%d%d", &sty, &stx, &edy, &edx);
		if (hoho != 4)
			break;
		path(sty, stx, edy, edx);
	}

	fclose(fp);
	return 0;
}