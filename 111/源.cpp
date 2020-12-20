#pragma warning (disable:4996 6031)
#pragma comment (lib, "winmm.lib")

#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include <mmsystem.h>

using namespace std;

int grade = 0;
int stonegrade = 0;
int bestgrade;
int beststonegrade;

MOUSEMSG m;

void gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

#define square 20
int lastx, lasty;
enum { shang, xia, zuo, you };
/******************定义区*************************/
typedef struct snake {
	int x;
	int y;
	int position;
	struct snake* syg = nullptr;
	struct snake* next = nullptr;
} snake;

struct Food {
	int x;
	int y;
	int flag;
	int grade;
};

typedef struct stone {
	int x;
	int y;
	stone* next;
} stone;

int stonenum = 0;

stone* stonehead = new stone;
snake* head = new snake;
Food* food = new Food;

/********************最高分********************/
void savegrade(int n) {
	FILE* fp;
	if (n == 111)
		fp = fopen("1.txt", "w+");
	else
		fp = fopen("2.txt", "w+");

	if (n == 111) {
		if (grade > bestgrade)
			fprintf(fp, "%d", grade);
	}
	else {
		if (stonegrade > beststonegrade)
			fprintf(fp, "%d", stonegrade);
	}

	fclose(fp);
}

void takegrade(int n) {
	FILE* fp;
	if (n == 111) {
		fp = fopen("1.txt", "a+");
		fscanf(fp, "%d", &bestgrade);
	}
	else {
		fp = fopen("2.txt", "a+");
		fscanf(fp, "%d", &beststonegrade);
	}
	fclose(fp);
}
/*****************玩蛇区********************/
//初始化蛇
void setsnake() {
	snake* a = head->next;
	snake* node;
	head->next = NULL;
	while (a) {
		node = a->next;
		free(a);
		a = node;
	} 


	head->position = 3;
	head->next = new snake;
	head->next->syg = head;
	head->next->next = new snake;
	head->next->next->syg = head->next;

	head->x = 40;
	head->y = 0;
	head->next->x = 20;
	head->next->y = 0;
	head->next->next->x = 0;
	head->next->next->y = 0;
	head->next->next->next = NULL;
	head->syg = NULL;
}

//画蛇
void printsnake() {
	snake* a = head;

	while (a) {
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(a->x, a->y, a->x + square, a->y + square);
		a = a->next;
	}

}

//蛇的位置变换
void move() {
	//身体移动
	snake* a = head;
	while (a->next) 
		a = a->next;
	
	lastx = a->x;
	lasty = a->y;

	while (a->syg) {
		a->x = a->syg->x;
		a->y = a->syg->y;
		a = a->syg;
	}
	//头部移动
	switch (head->position) {
	case shang:
		head->y -= square;
		break;
	case xia:
		head->y += square;
		break;
	case zuo:
		head->x -= square;
		break;
	case you:
		head->x += square;
		break;
	}
}

void key(int position) {
	char kb = _getch();
	switch (kb) {
	case 'w':
	case 'W':
	case 72:
		if (position != xia)
			head->position = shang;
		break;
	case 's':
	case 'S':
	case 80:
		if (position != shang)
			head->position = xia;
		break;
	case 'a':
	case 'A':
	case 75:
		if (position != you)
			head->position = zuo;
		break;
	case 'd':
	case 'D':
	case 77:
		if (position != zuo)
			head->position = you;
		break;
	}

}

void addsnake() {
	snake* b = head;
	while (b->next)
		b = b->next;

	snake* a = new snake;
	a->x = lastx;
	a->y = lasty;
	b->next = a;
	a->syg = b;
	a->next = NULL;
}
/******************食物区*********************/
void startfood() {
	food->flag = 1;
	food->grade = 1;
	food->x = rand() % 45 * 20;
	food->y = rand() % 35 * 20;

	snake* a = head;
	while (a) {
		if (a->x == food->x && a->y == food->y) {
			startfood();
			break;
		}
		a = a->next;
	}
}

void printfood() {
	setlinecolor(BLACK);
	setfillcolor(RGB(255, 0, 0));
	fillrectangle(food->x, food->y, food->x + square, food->y + square);
}
/******************石头区*********************/
void startstone() {
	stonehead->x = -20;
	stonehead->y = -20;
	stonehead->next = NULL;
}

void change() {
	stone* last = stonehead;
	while (last->next)
		last = last->next;

	stone* a = new stone;
	a->x = food->x;
	a->y = food->y;
	a->next = NULL;
	last->next = a;
}

void printstone() {
	stone* a = stonehead;

	while (a) {
		setlinecolor(BLACK);
		setfillcolor(RGB(0, 0, 0));
		fillrectangle(a->x, a->y, a->x + square, a->y + square);
		a = a->next;
	}
}

void setstone() {
	stone* a;
	stone* b = stonehead->next;
	
	while (b) {
		a = b;
		b = b->next;
		free(a);
	}
	stonehead->next = NULL;
}

/******************分数区********************/
void printgrade(int n) {
	settextcolor(BLACK);
	settextstyle(25, 10, "楷体");
	setbkmode(TRANSPARENT);
	char words[] = "Grade:";
	outtextxy(750, 10, words);
	char s[5];
	if (n == 111)
		sprintf(s, _T("%d"), grade);	
	else
		sprintf(s, _T("%d"), stonegrade);
	outtextxy(830, 10, s);
}


void startgrade() {
	grade = 0;
	stonegrade = 0;
}

/******************检测区*********************/
bool check() {
	snake* a = head->next;
	while (a) {
		if (a->x == head->x && a->y == head->y)
			return 1;
		a = a->next;
	}

	stone* s = stonehead;
	while (s) {
		if (s->x == head->x && s->y == head->y)
			return 1;
		s = s->next;
	}

	if (head->x == 900 || head->y == 700 || head->x < 0 || head->y < 0)
		return 1;

	return 0;
}

void timestone(int a) {
	// 111  70
	if (stonenum == a) {
		change();
		startfood();
		stonenum = 0;
		stonegrade++;
	}
	else
		stonenum++;
}


/*******************封装*********************/
void play(int n) {
	stonenum = 0;             //初始化计时器
	setstone();
	setlinecolor(BLACK);
	setsnake();
	startgrade();
	
	mciSendString("close all", 0, 0, 0);
	if (n == 111) {
		mciSendString("open 2.WMA", 0, 0, 0);
		mciSendString("play 2.WMA repeat ", 0, 0, 0);
	}
	else {
		mciSendString("open 3.MP3", 0, 0, 0);
		mciSendString("play 3.MP3 repeat ", 0, 0, 0);
	}

	while (1) {
		cleardevice();
		int position = head->position;
		if (food->flag == 0) {
			startfood();
		}

		BeginBatchDraw();
		timestone(n);
		printfood();
		printstone();
		printgrade(n);

		while (_kbhit())
			key(position);

		move();

		if (head->x == food->x && head->y == food->y) {
			addsnake();
			food->flag = 0;
			stonenum = 0;
			grade++;
		}

		printsnake();
		EndBatchDraw();

		Sleep(100);
		if (check()) { 
			mciSendString("close all", 0, 0, 0);
			mciSendString("open 4.MP3", 0, 0, 0);
			mciSendString("play 4.MP3 ", 0, 0, 0); //结束               
			MOUSEMSG m;
			char endgame[20] = "游戏结束";
			char endgrade[20] = "最终得分为：";
			char s[10];
			char playagain[100] = "普通模式";
			char playagain2[100] = "生存模式";
			char thebestgrade[100] = "最高分：";
			char quit[100] = "退出";
			char tbg[10];

			
			food->flag = 0;

			while (1) {
				BeginBatchDraw();
				settextstyle(100, 50, "楷体");
				settextcolor(RED);
				outtextxy(260, 100, endgame);
				settextstyle(50, 25, "楷体");
				outtextxy(260, 300, endgrade);
				

				if (n == 111) {
					takegrade(n);
					if (bestgrade < grade)
						savegrade(n);
					sprintf(s, _T("%d"), grade);
					sprintf(tbg, _T("%d"), bestgrade);
				}
				else {
					takegrade(n);
					if (beststonegrade < stonegrade)
						savegrade(n);
					sprintf(s, _T("%d"), stonegrade);
					sprintf(tbg, _T("%d"), beststonegrade);
				}
				outtextxy(560, 300, s);
				outtextxy(260, 460, playagain);
				outtextxy(260, 545, playagain2);
				outtextxy(310, 630, quit);
				outtextxy(260, 350, thebestgrade);
				outtextxy(470, 350, tbg);

				m = GetMouseMsg();

				if (m.x > 250 && m.x < 470 && m.y > 450 && m.y < 510) {
					fillrectangle(250, 460, 470, 510);
					outtextxy(260, 460, playagain);
					if (m.mkLButton != 0) {
						play(111);
						
					}
				}

				if (m.x > 250 && m.x < 470 && m.y > 545 && m.y < 605) {
					fillrectangle(250, 545, 470, 595);
					outtextxy(260, 545, playagain2);
					if (m.mkLButton != 0) {
						play(2);
						
					}
				}

				if (m.x > 250 && m.x < 470 && m.y > 630 && m.y < 690) {
					fillrectangle(250, 630, 470, 690);
					outtextxy(310, 630, quit);
					if (m.mkLButton != 0) {
						exit(0);
					}
				}

				EndBatchDraw();
				cleardevice();

			}
			setsnake();
		}
	}
}

/********************菜单区*******************/
void mean() {

	BeginBatchDraw();
	char houzhui[30] = "------汤姆·克兰西·二";
	char title[20] = "彩";
	char title2[20] = "虹";
	char title3[20] = "蛇";
	char title4[20] = "号";

	settextstyle(100, 50, "楷体");
	settextcolor(RED);
	outtextxy(320, 50, title);
	settextcolor(BLUE);
	outtextxy(420, 50, title2);
	settextcolor(GREEN);
	outtextxy(320, 150, title3);
	settextcolor(YELLOW);
	outtextxy(420, 150, title4);
	settextcolor(BLACK);
	settextstyle(30, 0, "黑体");
	outtextxy(450, 250, houzhui);

	char start[20] = "开始游戏";
	char paihang[20] = "生存模式";
	char setting[20] = "退出";
	char rule1[100] = "rule:";
	char rule2[50] = "↑↓ ← → 分别控制蛇";
	char rule3[50] = "的上下左右，碰到墙壁";
	char rule4[50] = "或吃到自己失败，吃到";
	char rule5[50] = "果实加长身体获得分数";
	char rule6[50] = "果实太久不吃会变成石";
	char rule7[50] = "头，生存模式变化超快!";
	char rule8[50] = "坚持即可得分！";

	settextcolor(BLACK);
	settextstyle(40, 0, "");
	outtextxy(340, 350, start);
	outtextxy(340, 420, paihang);
	outtextxy(380, 490, setting);
	settextstyle(20, 0, "");
	outtextxy(100, 350, rule1);
	outtextxy(100, 380, rule2);
	outtextxy(100, 410, rule3);
	outtextxy(100, 440, rule4);
	outtextxy(100, 470, rule5);
	outtextxy(100, 520, rule6);
	outtextxy(100, 550, rule7);
	outtextxy(100, 580, rule8);
	//setlinestyle(PS_NULL);

	settextstyle(40, 0, "");
	m = GetMouseMsg();
	if (m.x > 330 && m.x < 510 && m.y > 340 && m.y < 400) {
		fillrectangle(330, 340, 510, 400);
		outtextxy(340, 350, start);
		if (m.mkLButton != 0) {
			setlinestyle(BLACK);
			play(111);
		}
	}

	char waitting[20] = "pleas watting";
	if (m.x > 330 && m.x < 510 && m.y > 410 && m.y < 470) {
		fillrectangle(330, 410, 510, 470);
		outtextxy(340, 420, paihang);
		if (m.mkLButton != 0) {
			play(2);                      
		}
	} 

	if (m.x > 330 && m.x < 510 && m.y > 480 && m.y < 540) {
		fillrectangle(370, 480, 470, 540);
		outtextxy(380, 490, setting);
		if (m.mkLButton != 0) {
			exit(0);				
		}
	}

	EndBatchDraw();
	cleardevice();
}

/********************************************/
int main() {
	mciSendString(TEXT("open 1.WMA"), 0, 0, 0);
	mciSendString(TEXT("play 1.WMA repeat"), 0, 0, 0);
	
	//playmeanmusic2();

	srand((unsigned)time(NULL));
	food->flag = 0;
	initgraph(900, 700);
	setbkcolor(RGB(255, 255, 255));
	cleardevice();

	setsnake();    //初始化蛇
	printsnake();  //画蛇
	startstone();

	BeginBatchDraw();
	FlushBatchDraw();
	EndBatchDraw();

	while (1)       //菜单
		mean();
	
	_getch();
	closegraph();

	return 0;
}