#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdbool.h>  //path함수 결과값으로 쓰려고 추가한 해더

//gotoxy는 만들어써야 함으로 인터넷보고 만들었다.
void gotoxy(int x, int y)

{

	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

}

#define m 12
#define p 12
#define Max_dir 8	//총 가능한 이동방향의 수
#define Timestep 1000 //1초

int maze[m + 2][p + 2];  //maze의 total 크기 14 * 14
int mark[m + 2][p + 2];  //mark역시 total 크기 14 * 14

typedef struct Aoff {
	short int vert;
	short int horiz;
}offsets;  //dir을 위해 만든 구조체

typedef struct St_element { //스택에 저장할 데이터
	short int row;
	short int col;
	short int dir;
}ty_element;

offsets move[Max_dir] = { {-1,0}, {-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} }; //차례로 북, 북동, 동, 남동, 남,남서, 서, 북서

//typedef struct listNode* listPointer; //연결스택의 헤더

typedef struct StackNode {	//연결스택의 노드
	ty_element data;
	struct listNode* link;
}listNode;

typedef struct LinkedStack { //스택의 top부분을 구조체로 구현
	listNode* top;
}Stack;

Stack* creat() { //스택을 초기화 해주는 creat()함수
	Stack* S = (Stack*)malloc(sizeof(Stack));
	S->top = NULL;
	return S;
}
 
int isEmpty(Stack* S) { //스택이 공백인지 확인해 주는 함수
	return S->top == NULL;
}


void push(Stack* S, ty_element x) { //스택에 push연산을 수행하는 함수
	listNode* newNode = (listNode*)malloc(sizeof(listNode));
	newNode->data.col = x.col;
	newNode->data.row = x.row;
	newNode->data.dir = x.dir;
	newNode->link = S->top;
	S->top = newNode;
}

ty_element pop(Stack* S) { //스택에 pop연산을 수행하는 함수
	listNode* temp;
	ty_element e;
	if (isEmpty(S)) {
		printf("[ERROR] 스택이 비어있습니다.\n");
	}
	else {
		temp = S->top;
		e = temp->data;
		S->top = temp->link;
		free(temp);
		return e;
	}
}

void reverse(Stack* S) { // reverse함수
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
	//listPointer top = NULL; //스택의 top변수, 초기에는 NULL
	Stack s, * S;
	S = &s;
	S = creat(); // 스택 초기화

	int i, j, row, col, nextRow, nextCol, dir, basex, basey, ocount;
	int found, num_bktrack = 0, path_length = 0;
	int EXIT_ROW = dy, EXIT_COL = dx; //목표점
	ty_element position;

	//미로를 그리는 부분
	CONSOLE_SCREEN_BUFFER_INFO presentCur;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &presentCur);
	basex = presentCur.dwCursorPosition.X;
	basey = presentCur.dwCursorPosition.Y;

	for (i = 0; i < m + 2; i++) {  //maze를 그린다
		for (j = 0; j < p + 2; j++) {
			gotoxy(j + basex, i + basey);
			printf("%1d", maze[i][j]);
		}
	}
	printf("\n");

	//빈 스택에 출발점을 넣는다. (초기화)
	position.row = sy;
	position.col = sx;
	position.dir = 0;
	push(S, position);

	//mark정보초기화, 바운더리를 제외한 내부 셀에 모두 0으로 초기화한다.
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[i + 1][j + 1] = 0;
		}
	}

	if ((maze[sy][sx] == 1) || (maze[dy][dx] == 1)) {
		printf("입력한 좌표는 막힌 셸입니다.\n탐색을 거부합니다.\n");
		return false;
	}

	
	mark[sy][sx] = 1; //시작위치를 방문했음
	gotoxy(sx + basex, sy + basey);
	printf(">");    // 출발점을 표시한다
	Sleep(Timestep);

	found = 0;

	while (!isEmpty(S)) { //마지막 두 좌표는 스택에 push 되지 않는다 그래서 마지막 두 좌표를 알고있는 row,col,nextRow,nextCol을 이용할것이다
		position = pop(S);
		row = position.row;
		col = position.col;
		dir = position.dir;
		while (dir < Max_dir) {
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if ((nextRow == EXIT_ROW) && (nextCol == EXIT_COL)) { //목적지에 도착
				found = 1;
				gotoxy(nextCol + basex, nextRow + basey);
				printf("<");
				
				break;
			}
			else if ((maze[nextRow][nextCol] == 0) && (mark[nextRow][nextCol] == 0)) {//dir로 얻은 위치가 가능한 위치
				position.row = row;
				position.col = col;
				position.dir = ++dir;
				push(S, position);
				row = nextRow;
				col = nextCol;
				gotoxy(col + basex, row + basey);
				printf("*");
				Sleep(Timestep);

				dir = 0; //새로 이동후 dir = 0부터 다시 시도
				mark[row][col] = 1; //이동했음을 mark

			}
			else
				++dir; // ++dir을 함으로써 다음 dir방향 조정
		}
		if (found)
			break;

		gotoxy(col + basex, row + basey);  // 백트래킹 전에 경로표시를 지우고 0 을 넣음.
		printf("%c", '$');


		num_bktrack++;
		Sleep(Timestep);

	}
	gotoxy(0, basey + m + 2);  // 미로와 겹치지 않게 아래에 출력이 되게 커서를 옮김.
	printf("\n");

	if (found) {
		printf("경로: [ row , col ]: \n");
		reverse(S); // 스택을 리버스해서
		ocount = 0;
		while (!isEmpty(S)) { //스택을 출력함
			position.col = S->top->data.col;
			position.row = S->top->data.row;
			position.dir = S->top->data.dir;
			if ((ocount != 0) && (ocount % 6 == 0))  //스택에서 나오는 좌표를 6새씩 끊어 출력하기 위함
				printf("\n");
			printf("[ %2d , %2d ]", position.row, position.col);
			ocount++;
			S->top = S->top->link;
		}
		printf("[ %2d , %2d ]", row, col);  //사실 스택에는 마지막 두 좌표가 push되지 않음 그래서 이 두 문장으로 프린트 해주는 것임
		printf("[ %2d , %2d ]\n", EXIT_ROW, EXIT_COL);
		path_length = ocount + 2;
		printf("경로길이: %d, 백트래킹수 = %d\n", path_length, num_bktrack);
	}
	else
		printf("가능한 경로가 없습니다.\n");
	return true;

}



int main()
{
	FILE* fp;
	int hoho; //들어온 숫자 count
	int sty, stx, edy, edx; //좌표들

	//파일을 연다
	fp = fopen("maze_14_2.txt", "r");
	if (fp == NULL) {
		printf("파일을 여는데 실패했습니다.\n");
		return 1;
	}
	//maze에 파일에서 읽은 내용을 저장
	for (int i = 0; i < m + 2; i++) {
		for (int j = 0; j < p + 2; j++) {
			fscanf(fp, "%d", &maze[i][j]);
		}
	}
	
	while (1) {

		printf("출발점, 목표점은?(4개의 정수; 종료는 Control+D Enter}: ");
		hoho = scanf("%d%d%d%d", &sty, &stx, &edy, &edx);
		if (hoho != 4)
			break;
		path(sty, stx, edy, edx);
	}

	fclose(fp);
	return 0;
}