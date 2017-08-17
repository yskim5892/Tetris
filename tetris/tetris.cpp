/*
	made by K.Y.S
	1. ���� �ý���
	2. ���̵� ����
	3. �ܻ� ���̱� ����
	4. �� �ö���� ��� ���� 
	5. ������ ������� ����
	2012/09/18
*/

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define BLACK setcolor(0)
#define DARK_BLUE setcolor(1)
#define DARK_GREEN setcolor(2)
#define DARK_SKY_BLUE setcolor(3)
#define DARK_RED setcolor(4)
#define DARK_VIOLET setcolor(5)
#define DARK_YELLOW setcolor(6)
#define GRAY setcolor(7)
#define DARK_GRAY setcolor(8)
#define BLUE setcolor(9)
#define GREEN setcolor(10)
#define SKY setcolor(11)
#define RED setcolor(12)
#define VIOLET setcolor(13)
#define YELLOW setcolor(14)
#define WHITE setcolor(15)

#define hrz 11
#define vtc 25
#define leftend 20
#define upend 4
#define holdboxsize 5
#define nextboxsize 5

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ESC 27
#define ENTER 13
#define SPACE_BAR 32
#define BACKSPACE 8
#define TAB 9
#define PAUSE 112
#define PLUS 43
#define MINUS 45

#define empty 7
#define second 30

#define BS blockshape[0] 
#define RBS blockshape[1]

int delay=9;
bool crashcheck=true;
int block[hrz][vtc];
int highest[hrz+1];
HANDLE hOut;
int readyblockshape=0;
int holdblockshape=7;
int tempshape;//holdblock�� movingblock�� ��ü�� �� �Ű�ü

int score;  
/*��������

 

     0�޺� 1�޺�  2�޺� 3�޺� 4�޺� 5�޺� 6�޺�  7�޺� 8�޺�  9�޺� 10�޺� 11�޺�  12�޺�  13�޺� 14�޺� 15�޺� 16�޺� 
1��    0     1     1     2     2     3     3     4     4     4      4     4		  4       4     4      4       4


2��    1     2     2     3     3     4     4     4     5     5      5


3��    2     2     3    (3)    4    (4)   (5)   (5)   (6)    6                  


4��    4     5     5     6     6     6                       8                                


����Ʈ Ŭ���� 12��
T���� 1��: 4��
      2��: 6��


*/
bool ifcombo=false;
int combo=0;
int maxcombo=0;
int lines=0;

int timer=120;
int ifcrashdelay=0; //����� �� ��� crashdelay�� 0���� �������ϸ� 1�� ��
bool ifempty=false; //���� ���� �� ���Ǵ� BOOL ����(����� ��� ������ 1)
int crashdelay=0; //�ٴڿ� �ݾ��� �� �����̸� ���� ���� 
bool ifspacebar=false; //spacebar�� ������ �� 1�� ��(�ٴڿ� �ݾ��� �� �� �����̰� �ϱ� ����)
bool ifsame=false; //���� ���� �������� ���� ��� 1
bool perfectclear=false;
bool tspin=false;

char *blockshape[]={"��", "��"};

typedef struct movingblock{
	int x[4];
	int y[4];
	int shape; //shape=0: ��� shape=1: ���		shape=2:   ��� shape=3: ����shape=4: ���� shape=5: ���� shape=6: �����
			   //		  ���			  ���			     ���	           ��               ��          ��
	int direct;// 1&1: ���    1&2:    ��   2&1: ���   2&2:  ��   3&1:   ��  3&2:    �� 3&4:    ��   4&1:  ��  4&2 :  ���  4&4: ��  5&1:    ��   5&2: ��   5&4: ���  6&1: ����� 6&2: ��
               //        ���        ���      ���           ���      ����        ���     ���         ����     ��         ��      ����        ��          ��                     ��
	           //                    ��                         �� 3&3:   ����      ��         ��  4&3: ����       ��       ���  5&3: ����       ���        ��                     ��
	           //                                                           ��                                ��                           ��                                              ��
}TETRIS;             

TETRIS MOV;


int minimum(int a[4]);
void start();
void drawUI();
bool oneframe(int frame);
void gotoxy1(int x, int y);
void gotoxy2(int x, int y);
void cursorview(bool show);
void blockshow();
void blockmove();
void blockdraw(int blockshape, int pivotx);
void blockmake();
void control();
void lose();
void scorecalculate();
void rank();
void setcolor(int color);

void main()
{
	int i, j;
	int frame=0;
	srand(time(NULL));
	readyblockshape=rand()%7;
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);	
	WHITE;	
	cursorview(false);
	start();
	system("cls");
	for(i=0; i<=hrz-1; i++)
	{
		for(j=0; j<=vtc-1; j++)
		{
			block[i][j]=empty;
		}
	}
	for(i=0; i<=hrz; i++)
		highest[i]=vtc;
	drawUI();
	
	while(1)
	{
		int i, j;
		Sleep(delay);
		for(i=1; i<=hrz-1; i++)
		{
			highest[i]=vtc;
			for(j=vtc-1; j>=0; j--)
			{
				if(block[i][j]<=6)
					highest[i]=j;
			}
		}
		WHITE;

		gotoxy2(hrz+nextboxsize+4, 5); printf("combo : %d", combo);
		gotoxy2(hrz+nextboxsize+4, 6); printf("maxcombo: %d", maxcombo);
		gotoxy2(hrz+nextboxsize+4, 7); printf("score: %d", score);
		YELLOW;
		gotoxy2(hrz/2, -2); printf("      "); 
		gotoxy2(hrz/2, -2); printf("%d:%d", timer/60, timer%60);
		if(timer%60<10)
		{
			gotoxy2(hrz/2, -2); printf("%d:0%d", timer/60, timer%60);
		}
			
		if(!oneframe(frame))
			break;

		frame+=1;
	}
}
		
int minimum(int a[4])
{
	if(a[0]>a[1])
		a[0]=a[1];
	if(a[0]>a[2])
		a[0]=a[2];
	if(a[0]>a[3])
		a[0]=a[3];
	return a[0];
}

void start()
{
	int cursor=1;
	int scores[11]={0};
	int names[11][3]={0};
	system("mode con: cols=80 lines=40");
	while(1){
		cursor=1;
		printf("������� �������  �������  ������     ����   ����� \n");
		printf("     ��      ��                ��        ��       ��      ��    ���   ��� \n");
		printf("     ��      ��                ��        ��       ��      ��   ���     ���\n");
		printf("     ��      ��                ��        ������       ��    ����     \n");
		printf("     ��      �������      ��        ����           ��        ���   \n");
		printf("     ��      ��                ��        ��   ���        ��          ��� \n");
		printf("     ��      ��                ��        ��     ���      ��    ��   ���� \n");
		printf("     ��      �������      ��        ��       ���  ����  ������ \n");
		printf("");

		gotoxy1(24,15);
		printf("     1. ���ӽ���");
		gotoxy1(24,17);
		printf("     2. ��ŷ����");
		gotoxy1(24,19);
		printf("     3. �����ϱ�");

		while(1)
		{
			gotoxy1(24, 2*cursor+17);
			printf("  ");
			gotoxy1(24, 2*cursor+15);
			printf("  ");
			gotoxy1(24, 2*cursor+13);
			printf("->");
			gotoxy1(24, 2*cursor+11);
			printf("  ");
			gotoxy1(24, 2*cursor+9);
			printf("  ");
			int key = 0;
			bool ifenter = false;
			while(true)
			{
				key = getch();
				if(key==UP && cursor!=1)
				{
					cursor-=1;
					break;
				}
				if(key==DOWN && cursor!=3)
				{
					cursor+=1;
					break;
				}
				if(key==ENTER)
				{
					system("cls");
					ifenter = true;
					break;
				}
			}
			if(ifenter)
			{
				system("cls");
				break;
			}
		}
		if(cursor==1)
			break;
		if(cursor==2)
		{
			int i, j;
			FILE *input = fopen("rank.txt","r");
			for(i=0;i<=9;i++)
			{
				fscanf(input,"%d",&scores[i]);
			}
			for(i=0; i<=9; i++)
			{
				for(j=0; j<=2; j++)
				{
					fscanf(input, "%d",&names[i][j]);
				}
			}
			gotoxy1(25, 1); printf("��ŷ");
			for(i=0; i<=9; i++)
			{
				gotoxy1(20, 3+i); printf("%2d�� :  %2d��     �̸�: %c%c%c", i+1, scores[i], names[i][0], names[i][1], names[i][2]);
			}
			while(getch()!=ENTER)
				i=1;
			system("cls");
			continue;
		}
		if(cursor==3)
			exit(1);
	}
}

void drawUI(){
	int i, j;
	for(i=1; i<=hrz-1; i++)
	{
		gotoxy2(i, -1); printf("--");
		gotoxy2(i, vtc); printf("--");
	}
	for(i=0; i<=vtc-1; i++)
	{
		gotoxy2(0, i); printf("|");
		gotoxy2(hrz, i); printf("|");
	}

	for(i=1; i<=holdboxsize; i++)
	{
		gotoxy2(-i, -1); printf("==");
		gotoxy2(-i, -1+holdboxsize); printf("==");
	}
	gotoxy2(-holdboxsize+1, -1); printf("=hold");
	for(i=0; i<=holdboxsize-2; i++)
	{
		gotoxy2(-holdboxsize-1, i); printf(" |");
	}

	for(i=1; i<=nextboxsize; i++)
	{
		for(j=0; j<=4; j++)
		{
			gotoxy2(i+hrz, -1+j*nextboxsize); printf("==");
		}	
	}
	for(i=0; i<=nextboxsize-2; i++)
	{
		for(j=0; j<=3; j++)
		{
			gotoxy2(hrz+nextboxsize+1, i+j*nextboxsize); printf("|");
		}
	}
	gotoxy2(hrz+2, -1); printf("=next");
}

bool oneframe(int frame){
	WHITE;
	int i, j, k;
	if(crashdelay>0)
	{
		crashdelay-=1;
		if(crashdelay==0)
		{
			crashcheck=true;
		}
	}
	if(crashcheck==false)
	{
		if(frame%50==0 && crashdelay==0)
			blockmove();
		blockshow();
	}
	if(frame%second==0)
	{
		timer-=1;
	}
	if(crashcheck==true)
	{
		crashcheck=false;
		for(i=0; i<=3; i++)
		{
			block[MOV.x[i]][MOV.y[i]]=MOV.shape;
		}
		for(i=1; i<=hrz-1; i++)
		{
			highest[i]=vtc;
			for(j=vtc-1; j>=0; j--)
			{
				if(block[i][j]<=6)
					highest[i]=j;
			}
		}
		for(j=0; j<=vtc-1; j++)
		{
			for(i=1; i<=hrz-1; i++)
			{
				if(block[i][j]==empty)
					ifempty=true;
			}
			if(ifempty==false)
			{
				lines+=1;
				perfectclear=true;
				for(i=1; i<=hrz-1; i++)
				{

					block[i][j]=empty;
					for(k=j; k>=highest[i]; k--)
					{
						block[i][k]=block[i][k-1];
					}
					if(highest[i]<=vtc-1)
						highest[i]+=1;
					gotoxy2(i, highest[i]-1); printf("  ");
					gotoxy2(i, highest[i]-2); printf("  ");
					gotoxy2(i, highest[i]-3); printf("  ");
					gotoxy2(i, highest[i]-4); printf("  ");
					WHITE;
					gotoxy2(i, vtc); printf("--");
					if(highest[i]!=0)
						perfectclear=false;
				}
			}
			ifempty=false;
		}
			
		if(perfectclear==true)
		{
			score+=12;
			RED;
			gotoxy2(hrz/2, vtc+4); printf("perfect clear!!!!");
			WHITE;
		}
		if(lines!=0)
		{
				
			if(ifcombo==true)
			{
				combo+=1;
			}
			if(ifcombo==false)
			{
				ifcombo=1;
			}
			if(combo>maxcombo)
			{
				maxcombo=combo;
			}
			if(tspin==true)
			{
				tspin=false;
				RED;
				gotoxy2(hrz/2, vtc+3); printf("T-spin!!!!!!!!");
				WHITE;
				if(lines==1)	score+=4;
				if(lines==2)	score+=6;
			}
		}
		if(lines==0)
		{
			if(ifcombo==true)
			{
				ifcombo=false;
				combo=0;
			}
		}
		scorecalculate();
		lines=0;
		blockmake();
		ifspacebar=false;
	}
	for(i=0; i<1000; i++)
	{
		if(kbhit())  //Ű���尡 ����������
		{
			control(); //��Ʈ�� �Լ� ����
		}
	}
	for(i=0; i<=3; i++)
	{
		if(block[MOV.x[i]][MOV.y[i]+1]<=6 || MOV.y[i]==vtc-1) //�̵����� ����� �Ʒ� ĭ�� �Ϲ� ����� ���� ���
		{
			if(crashdelay==0 && ifspacebar==false)
				crashdelay=50;
		}
		if(block[MOV.x[i]][MOV.y[i]+1]==7 && MOV.y[i]!=vtc-1 && crashdelay>0) //crashdelay�� ���ư��� �ִ� �߿� ����� �� ���
		{
			ifcrashdelay=1;
		}
		for(j=0; j<=3; j++)
		{
			if(MOV.y[i]+1==MOV.y[j])
				ifcrashdelay=0;
		}
	}
	if(ifcrashdelay==1)
	{
		crashdelay=0;
		ifcrashdelay=0;
	}

	for(i=1; i<=hrz-1; i++)
	{
		if(block[i][0]<=6)	//�й� �˻�
		{
			lose();
			system("cls");
			rank();
			frame=0;
			score=0;
			ifcombo=false;
			combo=0;
			maxcombo=0;
			lines=0;
			ifempty=false; 
			crashdelay=0; 
			ifspacebar=false;
			ifsame=false; 
			return false;
		}
	}
	if(timer==0)
	{
		Sleep(1000);
		while(kbhit())
			i=getch();
		YELLOW;
		gotoxy2(hrz/2, -2); printf("      "); 
		gotoxy2(hrz/2, -2); printf("0:00", timer/60, timer%60);
		WHITE;
		for(i=0; i<=15; i++)
		{
			gotoxy2(hrz/2, vtc+6);
			setcolor(i);
			printf("time over");
			Sleep(150);
		}
		Sleep(1000);
		system("cls");
		WHITE;
		rank();
		frame=0;
		score=0;
		ifcombo=false;
		combo=0;
		maxcombo=0;
		lines=0;
		ifempty=false; 
		crashdelay=0; 
		ifspacebar=false;
		ifsame=false; 
		return false;
	}
	return true;
}

void gotoxy1(int x, int y)
{
	COORD Pos={x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void gotoxy2(int x, int y)
{
	COORD Pos={2*x-1+leftend, y+upend};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void cursorview(bool show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}

void blockshow() //��� �����ִ� �Լ�
{
	int i, j;
	int a[4], c[4], ifminus=0;
	for(i=1; i<=hrz-1; i++)
	{
		for(j=0; j<=vtc-1; j++)
		{
			if(block[i][j]==0)
			{
				YELLOW;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==1)
			{
				RED;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==2)
			{
				GREEN;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==3)
			{
				VIOLET;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==4)
			{
				BLUE;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==5)
			{
				WHITE;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if(block[i][j]==6)
			{
				SKY;
				gotoxy2(i, j); 
				printf("%s", BS);
			}
			if((j>=highest[i] || j>=highest[i-1] || j>=highest[i+1]) && block[i][j]==7)
			{
				gotoxy2(i, j);
				printf("  ");
			}
		}
	}
	for(j=0; j<=vtc-1; j++)
	{
		if(j>=highest[0] && block[0][j]==7)
		{
			gotoxy2(0, j);
			printf("  ");
		}
	}
	if(MOV.shape==0) YELLOW;
	if(MOV.shape==1) RED;
	if(MOV.shape==2) GREEN;
	if(MOV.shape==3) VIOLET;
	if(MOV.shape==4) BLUE;
	if(MOV.shape==5) WHITE;
	if(MOV.shape==6) SKY;
	if(crashdelay==0)
	{
		for(i=0; i<=3; i++)
		{
			if(highest[MOV.x[i]]<MOV.y[i])
				ifminus=1;
		}
		if(ifminus==0)
		{
			for(i=0; i<=3; i++)
				a[i]=highest[MOV.x[i]]-MOV.y[i];
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]+minimum(a)-1); printf("%s", RBS);
			}
		}
		if(ifminus==1)
		{
			a[0]=0; //�������� �� ĭ �� a[0]�� 0���� �ʱ�ȭ 
			for(i=0; i<=3; i++) c[i]=MOV.y[i];  //c�� MOV.y ���� 
			while(1)
			{
				for(i=0; i<=3; i++)
				{
					if(block[MOV.x[i]][c[i]+1]<=6 || c[i]==vtc-1)
						crashcheck=true;
				}
				if(crashcheck==true) 
					break;
				a[0]+=1;
				for(i=0; i<=3; i++) 
					c[i]+=1;
			}
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], c[i]); printf("%s", RBS);
			}
			crashcheck=false;
		}
	}
	for(i=0; i<=3; i++)
	{
		gotoxy2(MOV.x[i], MOV.y[i]); 
		printf("%s", BS);
	}
	
}

void blockmove() //��� �̵��Լ�
{	
	int i;
	if(crashdelay==0)
	{
		for(i=0; i<=3; i++)
		{
			gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
		}
		for(i=0; i<=3; i++)
			MOV.y[i]+=1;
	}
}

void blockdraw(int shape, int pivotx)
{
	if(shape==0)
	{
		YELLOW;
		gotoxy2(pivotx+2, 2); printf("%s", BS); gotoxy2(pivotx+3, 2); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+3, 3); printf("%s", BS);
	}
	if(shape==1)
	{
		RED;
		gotoxy2(pivotx+1, 2); printf("%s", BS); gotoxy2(pivotx+2, 2); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+3, 3); printf("%s", BS);
	}
	if(shape==2)
	{
		GREEN;
		gotoxy2(pivotx+3, 2); printf("%s", BS); gotoxy2(pivotx+2, 2); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+1, 3); printf("%s", BS);
	}
	if(shape==3)
	{
		VIOLET;
		gotoxy2(pivotx+2, 2); printf("%s", BS); gotoxy2(pivotx+1, 3); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+3, 3); printf("%s", BS);
	}
	if(shape==4)
	{
		BLUE;
		gotoxy2(pivotx+1, 2); printf("%s", BS); gotoxy2(pivotx+1, 3); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+3, 3); printf("%s", BS);
	}
	if(shape==5)
	{
		WHITE;
		gotoxy2(pivotx+3, 2); printf("%s", BS); gotoxy2(pivotx+1, 3); printf("%s", BS); gotoxy2(pivotx+2, 3); printf("%s", BS); gotoxy2(pivotx+3, 3); printf("%s", BS);
	}
	if(shape==6)
	{
		SKY;
		gotoxy2(pivotx+1, 2); printf("%s", BS); gotoxy2(pivotx+2, 2); printf("%s", BS); gotoxy2(pivotx+3, 2); printf("%s", BS); gotoxy2(pivotx+4, 2); printf("%s", BS);
	}
}
void blockmake()//��� ���� ���� �Լ�
{
	int i, j;
	MOV.shape=readyblockshape;
	MOV.direct=1;
	do
	{
		readyblockshape=rand()%7;
	}while(ifsame==true && readyblockshape==MOV.shape);
	ifsame=false;
	if(readyblockshape==MOV.shape) //���� ���� ��� ������ ��� ����
	{
		ifsame=true;
	}
	
	for(i=1; i<=nextboxsize; i++)
	{
		for(j=1; j<=nextboxsize-2; j++)
		{
			gotoxy2(hrz+i, j); printf("  ");
		}
	}

	blockdraw(readyblockshape, hrz);
	blockdraw(holdblockshape, -holdboxsize);

	if(MOV.shape==0)
	{
		MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;  
		MOV.x[1]=(hrz/2); MOV.y[1]=1;     //        01
		MOV.x[2]=(hrz/2)-1; MOV.y[2]=2;   //        23   
		MOV.x[3]=(hrz/2); MOV.y[3]=2;
	}
	if(MOV.shape==1)
	{
		MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;
		MOV.x[1]=(hrz/2); MOV.y[1]=1;     //       01
		MOV.x[2]=(hrz/2); MOV.y[2]=2;     //        23
		MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
	}
	if(MOV.shape==2)
	{
		MOV.x[0]=(hrz/2)+1; MOV.y[0]=1;
		MOV.x[1]=(hrz/2); MOV.y[1]=1;     //        10
		MOV.x[2]=(hrz/2); MOV.y[2]=2;     //       32
		MOV.x[3]=(hrz/2)-1; MOV.y[3]=2;
	}
	if(MOV.shape==3)
	{
		MOV.x[0]=(hrz/2); MOV.y[0]=1;
		MOV.x[1]=(hrz/2)-1; MOV.y[1]=2;     //        0
		MOV.x[2]=(hrz/2); MOV.y[2]=2;       //       123
		MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
	}
	if(MOV.shape==4)
	{
		MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;
		MOV.x[1]=(hrz/2)-1; MOV.y[1]=2;     //       0
		MOV.x[2]=(hrz/2); MOV.y[2]=2;       //       123
		MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
	}
	if(MOV.shape==5)
	{
		MOV.x[0]=(hrz/2)+1; MOV.y[0]=1;
		MOV.x[1]=(hrz/2)+1; MOV.y[1]=2;     //       0
		MOV.x[2]=(hrz/2); MOV.y[2]=2;       //     321 
		MOV.x[3]=(hrz/2)-1; MOV.y[3]=2;
	}
	if(MOV.shape==6)
	{
		MOV.x[0]=(hrz/2)-2; MOV.y[0]=1;
		MOV.x[1]=(hrz/2)-1; MOV.y[1]=1;   //      0123
		MOV.x[2]=(hrz/2); MOV.y[2]=1;     //
		MOV.x[3]=(hrz/2)+1; MOV.y[3]=1;
	}
}

void control()
{
	int i, j;
	int tipcheck=0;
	int a[4]; //spacebar�� ���� �� highest-MOV.y[i]=a[i] (����� ���������� ĭ ��)
	int c[4]; //spacebar�� �����µ� highest�� MOV.y[i]���� ���� ��� a�� ������ �����Ƿ� MOV�� c�� ������ �� for������ a ����
	int ifminus=0; //spacebar�� �����µ� highest �� �ϳ��� MOV.y[i]���� ���� ��� 1

	switch(getch())
	{
	case LEFT:
		for(i=0; i<=3; i++)
		{
			if(block[MOV.x[i]-1][MOV.y[i]]<=6 || MOV.x[i]==1)
				tipcheck=1;
		}
		if(tipcheck==0)
		{
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]); printf("  "); //��� ��� ���ֱ� 
			}

			for(i=0; i<=3; i++)
			{
				if(highest[MOV.x[i]]<MOV.y[i])
					ifminus=1;
			}
			if(ifminus==0)
			{
				for(i=0; i<=3; i++)
					a[i]=highest[MOV.x[i]]-MOV.y[i];
				for(i=0; i<=3; i++)
				{
					gotoxy2(MOV.x[i], MOV.y[i]+minimum(a)-1); printf("  "); //RBS ��� ���ֱ� 
				}
			}
			if(ifminus==1)
			{
				a[0]=0; //�������� �� ĭ �� a[0]�� 0���� �ʱ�ȭ 
				for(i=0; i<=3; i++) c[i]=MOV.y[i];  //c�� MOV.y ���� 
				while(1)
				{
					for(i=0; i<=3; i++)
					{
						if(block[MOV.x[i]][c[i]+1]<=6 || c[i]==vtc-1)
							crashcheck=true;
					}
					if(crashcheck==true) 
						break;
					a[0]+=1;
					for(i=0; i<=3; i++) 
						c[i]+=1;
				}
				for(i=0; i<=3; i++)
				{
					gotoxy2(MOV.x[i], c[i]); printf("  "); //RBS ��� ���ֱ�
				}
				crashcheck=false;
			}

			for(i=0; i<=3; i++)
				MOV.x[i]-=1;
			blockshow();
		}
		break;
	case RIGHT:
		for(i=0; i<=3; i++)
		{
			if(block[MOV.x[i]+1][MOV.y[i]]<=6 || MOV.x[i]==hrz-1)
				tipcheck=1;
		}
		if(tipcheck==0)
		{
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
			}
			
			for(i=0; i<=3; i++)
			{
				if(highest[MOV.x[i]]<MOV.y[i])
					ifminus=1;
			}
			if(ifminus==0)
			{
				for(i=0; i<=3; i++)
					a[i]=highest[MOV.x[i]]-MOV.y[i];
				for(i=0; i<=3; i++)
				{
					gotoxy2(MOV.x[i], MOV.y[i]+minimum(a)-1); printf("  "); //RBS ��� ���ֱ� 
				}
			}
			if(ifminus==1)
			{
				a[0]=0; //�������� �� ĭ �� a[0]�� 0���� �ʱ�ȭ 
				for(i=0; i<=3; i++) c[i]=MOV.y[i];  //c�� MOV.y ���� 
				while(1)
				{
					for(i=0; i<=3; i++)
					{
						if(block[MOV.x[i]][c[i]+1]<=6 || c[i]==vtc-1)
							crashcheck=true;
					}
					if(crashcheck==1) 
						break;
					a[0]+=1;
					for(i=0; i<=3; i++) 
						c[i]+=1;
				}
				for(i=0; i<=3; i++)
				{
					gotoxy2(MOV.x[i], c[i]); printf("  "); //RBS ��� ���ֱ�
				}
				crashcheck=false;
			}

			for(i=0; i<=3; i++)
				MOV.x[i]+=1;
			blockshow();
		}
		break;
	case DOWN:
		for(i=0; i<=3; i++)
		{
			if(block[MOV.x[i]][MOV.y[i]+1]<=6 || MOV.y[i]==vtc-1)
				tipcheck=1;
		}
		if(tipcheck==0)
		{
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
			}
			for(i=0; i<=3; i++)
				MOV.y[i]+=1;
			blockshow();
		}
		break;
	case UP:  
		for(i=0; i<=3; i++)
		{
			if(highest[MOV.x[i]]<MOV.y[i])
				ifminus=1;
		}
		if(ifminus==0)
		{
			for(i=0; i<=3; i++)
				a[i]=highest[MOV.x[i]]-MOV.y[i];
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]+minimum(a)-1); printf("  "); //RBS ��� ���ֱ� 
			}
		}
		if(ifminus==1)
		{
			a[0]=0; //�������� �� ĭ �� a[0]�� 0���� �ʱ�ȭ 
			for(i=0; i<=3; i++) c[i]=MOV.y[i];  //c�� MOV.y ���� 
			while(1)
			{
				for(i=0; i<=3; i++)
				{
					if(block[MOV.x[i]][c[i]+1]<=6 || c[i]==vtc-1)
						crashcheck=true;
				}
				if(crashcheck==1) 
					break;
				a[0]+=1;
				for(i=0; i<=3; i++) 
					c[i]+=1;
			}
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], c[i]); printf("  "); //RBS ��� ���ֱ�
			}
			crashcheck=false;
		}
											
										   //  ---     --0
		if(MOV.shape==1 && MOV.direct==1)  //  01-     -21
		{                                  //  -23     -3-
			if(block[MOV.x[1]+1][MOV.y[1]-1]==empty && block[MOV.x[1]+1][MOV.y[1]]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");

				MOV.direct=2;
				MOV.x[0]+=2; MOV.y[0]-=1; MOV.x[1]+=1; MOV.y[2]-=1; MOV.x[3]-=1; 
			}
			break;
		}

		if(MOV.shape==1 && MOV.direct==2)
		{
			if(MOV.x[2]>=2)
			{
				if(block[MOV.x[2]-1][MOV.y[2]]==empty && block[MOV.x[3]+1][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					MOV.direct=1;			
					MOV.x[0]-=2; MOV.y[0]+=1; MOV.x[1]-=1; MOV.y[2]+=1; MOV.x[3]+=1;
				}
				break;
			}
			if(MOV.x[2]==1)
			{
				if(block[MOV.x[3]+1][MOV.y[3]]==empty && block[MOV.x[3]+2][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");

					MOV.direct=1;
					MOV.x[0]-=1; MOV.y[0]+=1; MOV.x[2]+=1; MOV.y[2]+=1; MOV.x[3]+=2;
				}
				break;
			}	
		}
		 								   //  ---      0--
		if(MOV.shape==2 && MOV.direct==1)  //  -10      12-
		{                                  //  32-      -3-
			if(block[MOV.x[1]-1][MOV.y[1]-1]==empty && block[MOV.x[1]-1][MOV.y[1]]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=2;
				MOV.x[0]-=2; MOV.y[0]-=1; MOV.x[1]-=1; MOV.y[2]-=1; MOV.x[3]+=1;
			}
			break;
		}
		if(MOV.shape==2 && MOV.direct==2)
		{
			if(MOV.x[2]<=hrz-2)
			{
				if(block[MOV.x[2]+1][MOV.y[2]]==empty && block[MOV.x[3]-1][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					MOV.direct=1;
					MOV.x[0]+=2; MOV.y[0]+=1; MOV.x[1]+=1; MOV.y[2]+=1; MOV.x[3]-=1;
				}
				break;
			}
			if(MOV.x[2]==hrz-1)
			{
				if(block[MOV.x[3]-1][MOV.y[3]]==empty && block[MOV.x[3]-2][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]+=1; MOV.y[0]+=1; MOV.x[2]-=1; MOV.y[2]+=1; MOV.x[3]-=2;
				}
				break;
			}
		}
										    // ---  -1-  ---  -3-
		if(MOV.shape==3 && MOV.direct==1)   // -0-  -20  321  02-
		{									// 123  -3-  -0-  -1-
			if(block[MOV.x[0]][MOV.y[0]-1]==empty && block[MOV.x[0]+1][MOV.y[0]]==empty)
			{
				gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=2;
				MOV.x[0]+=1; MOV.x[1]+=1; MOV.y[1]-=2; MOV.y[2]-=1; MOV.x[3]-=1;
			}
			break;
		}
		if(MOV.shape==3 && MOV.direct==2)
		{
			if(MOV.x[1]>=2)
			{
				if(block[MOV.x[2]-1][MOV.y[2]]==empty)
				{
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					if(block[MOV.x[1]-1][MOV.y[1]]<=6 && block[MOV.x[3]-1][MOV.y[3]]<=6 && block[MOV.x[3]+1][MOV.y[3]]<=6)
					{
						tspin=true;
					}
					MOV.direct=3;
					MOV.x[0]-=1; MOV.y[0]+=1; MOV.x[1]+=1; MOV.y[1]+=1; MOV.x[3]-=1; MOV.y[3]-=1;
				}
				break;
			}
			if(MOV.x[1]==1)
			{
				if(block[MOV.x[0]][MOV.y[0]+1]==empty && block[MOV.x[0]+1][MOV.y[0]]==empty)
				{
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=3;
					MOV.y[0]+=1; MOV.x[1]+=2; MOV.y[1]+=1; MOV.x[2]+=1; MOV.y[3]-=1;
				}
				break;
			}
		}                                    // ---  -1-  ---  -3-
		if(MOV.shape==3 && MOV.direct==3)    // -0-  -20  321  02-
		{                                    // 123  -3-  -0-  -1-
			if(block[MOV.x[2]][MOV.y[2]-1]==empty)
			{
				tspin=false;
				gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
				
				MOV.direct=4;
				MOV.x[0]-=1; MOV.y[0]-=1; MOV.x[1]-=1; MOV.y[1]+=1; MOV.x[3]+=1; MOV.y[3]-=1;
			}
			break;
		}
		if(MOV.shape==3 && MOV.direct==4)
		{
			if(MOV.x[1]<=hrz-2)
			{
				if(block[MOV.x[1]-1][MOV.y[1]]==empty && block[MOV.x[1]+1][MOV.y[1]]==empty)
				{
					
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]+=1; MOV.x[1]-=1; MOV.y[2]+=1; MOV.x[3]+=1; MOV.y[3]+=2;
				}
				break;
			}
			if(MOV.x[1]==hrz-1)
			{
				if(block[MOV.x[1]-1][MOV.y[1]]==empty && block[MOV.x[1]-2][MOV.y[1]]==empty)
				{
					
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[1]-=2; MOV.x[2]-=1; MOV.y[2]+=1; MOV.y[3]+=2;
				}
				break;
			}
		}
									      //---  -10  ---  -3-
		if(MOV.shape==4 && MOV.direct==1) //0--  -2-  321  -2-
		{								  //123  -3-  --0  01-
			if(block[MOV.x[0]+1][MOV.y[0]]==empty && block[MOV.x[0]+1][MOV.y[0]-1]==empty && block[MOV.x[3]][MOV.y[3]-2]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=2;
				MOV.x[0]+=2; MOV.y[0]-=1; MOV.x[1]+=1; MOV.y[1]-=2; MOV.y[2]-=1; MOV.x[3]-=1;
			}
			break;
		}
		if(MOV.shape==4 && MOV.direct==2)
		{
			if(MOV.x[1]>=2)
			{
				if(block[MOV.x[2]-1][MOV.y[2]]==empty && block[MOV.x[2]+1][MOV.y[2]]==empty && block[MOV.x[3]+1][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=3;
					MOV.y[0]+=2; MOV.x[1]+=1; MOV.y[1]+=1; MOV.x[3]-=1; MOV.y[3]-=1;
				}
				break;
			}
			if(MOV.x[1]==1)
			{
				if(block[MOV.x[2]+1][MOV.y[2]]==empty && block[MOV.x[2]+2][MOV.y[2]]==empty && block[MOV.x[3]+2][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=3;
					MOV.x[0]+=1; MOV.y[0]+=2; MOV.x[1]+=2; MOV.y[1]+=1; MOV.x[2]+=1; MOV.y[3]-=1;
				}
				break;
			}
		}                                 //---  -10  ---  -3-
		if(MOV.shape==4 && MOV.direct==3) //0--  -2-  321  -2-
		{								  //123  -3-  --0  01-
			if(block[MOV.x[0]-2][MOV.y[0]]==empty && block[MOV.x[0]-1][MOV.y[1]]==empty && block[MOV.x[2]][MOV.y[2]-1]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=4;
				MOV.x[0]-=2; MOV.x[1]-=1; MOV.y[1]+=1; MOV.x[3]+=1; MOV.y[3]-=1;
			}
			break;
		}
		if(MOV.shape==4 && MOV.direct==4)
		{
			if(MOV.x[3]<=hrz-2)
			{
				if(block[MOV.x[0]][MOV.y[0]-1]==empty && block[MOV.x[1]+1][MOV.y[0]]==empty)
				{
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.y[0]-=1; MOV.x[1]-=1; MOV.x[3]+=1; MOV.y[2]+=1; MOV.y[3]+=2;
				}
				break;
			}
			if(MOV.x[3]==hrz-1)
			{
				if(block[MOV.x[0]-1][MOV.y[0]-1]==empty && block[MOV.x[0]-1][MOV.y[0]]==empty)
				{
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]-=1; MOV.y[0]-=1; MOV.x[1]-=2; MOV.x[2]-=1; MOV.y[2]+=1; MOV.y[3]+=2;
				}
				break;
			}
		}
										  // ---  -3-  ---  01-
		if(MOV.shape==5 && MOV.direct==1) // --0  -2-  123  -2-
		{								  // 321  -10  0--  -3-
			if(block[MOV.x[2]][MOV.y[2]-2]==empty && block[MOV.x[2]][MOV.y[2]-1]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=2;
				MOV.y[0]+=1; MOV.x[1]-=1; MOV.y[2]-=1; MOV.x[3]+=1; MOV.y[3]-=2;
			}
			break;
		}
		if(MOV.shape==5 && MOV.direct==2)
		{
			if(MOV.x[1]>=2)
			{
				if(block[MOV.x[1]-1][MOV.y[1]]==empty && block[MOV.x[2]-1][MOV.y[2]]==empty && block[MOV.x[2]+1][MOV.y[2]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=3;
					MOV.x[0]-=2; MOV.x[1]-=1; MOV.y[1]-=1; MOV.x[3]+=1; MOV.y[3]+=1;
				}
				break;
			}
			if(MOV.x[1]==1)
			{
				if(block[MOV.x[2]+1][MOV.y[i]]==empty && block[MOV.x[2]+2][MOV.y[i]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=3;
					MOV.x[0]-=1; MOV.y[1]-=1; MOV.x[2]+=1; MOV.x[3]+=2; MOV.y[3]+=1;
				}
				break;
			}
		}								  // ---  -3-  ---  01-
		if(MOV.shape==5 && MOV.direct==3) // --0  -2-  123  -2-
		{								  // 321  -10  0--  -3-
			if(block[MOV.x[1]][MOV.y[1]-1]==empty && block[MOV.x[2]][MOV.y[2]-1]==empty && block[MOV.x[2]][MOV.y[2]+1]==empty)
			{
				gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
				gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
				gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
				MOV.direct=4;
				MOV.y[0]-=2; MOV.x[1]+=1; MOV.y[1]-=1; MOV.x[3]-=1; MOV.y[3]+=1; 
			}
			break;
		}
		if(MOV.shape==5 && MOV.direct==4)
		{
			if(MOV.x[1]<=hrz-2)
			{
				if(block[MOV.x[2]+1][MOV.y[2]]==empty && block[MOV.x[3]+1][MOV.y[3]]==empty && block[MOV.x[3]-1][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					MOV.direct=1;
					MOV.x[0]+=2; MOV.y[0]+=1; MOV.x[1]+=1; MOV.y[1]+=2; MOV.y[2]+=1; MOV.x[3]-=1;
				}
				break;
			}
			if(MOV.x[1]==hrz-1)
			{
				if(block[MOV.x[3]-1][MOV.y[3]]==empty && block[MOV.x[3]-2][MOV.y[3]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
			
					MOV.direct=1; 
					MOV.x[0]+=1; MOV.y[0]+=1; MOV.y[1]+=2; MOV.x[2]-=1;	MOV.y[2]+=1;  MOV.x[3]-=2;
				}
				break;
			}
		}
										   // ----  --0-
		if(MOV.shape==6 && MOV.direct==1)  // 0123  --1-
		{								   // ----  --2-
										   // ----  --3-
			if(MOV.x[0]==1 && MOV.y[0]<=vtc-4)				   
			{
				if(block[MOV.x[0]][MOV.y[0]+1]==empty && block[MOV.x[0]][MOV.y[0]+2]==empty && block[MOV.x[0]][MOV.y[0]+3]==empty)
				{
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=2;
					MOV.x[1]-=1; MOV.y[1]+=1; MOV.x[2]-=2; MOV.y[2]+=2; MOV.x[3]-=3; MOV.y[3]+=3;
				}
				break;
			}
			if(MOV.x[3]==hrz-1 && MOV.y[0]<=vtc-4)
			{
				if(block[MOV.x[3]][MOV.y[0]+1]==empty && block[MOV.x[3]][MOV.y[0]+2]==empty && block[MOV.x[0]][MOV.y[0]+3]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					MOV.direct=2;
					MOV.x[0]+=3; MOV.y[0]-=1; MOV.x[1]+=2; MOV.x[2]+=1; MOV.y[2]+=1; MOV.y[3]+=2;
				}
				break;
			}
			if(2<=MOV.x[0] && MOV.x[3]<=hrz-2 && MOV.y[0]<=vtc-3)
			{
				if(block[MOV.x[2]][MOV.y[2]-1]==empty && block[MOV.x[2]][MOV.y[2]+1]==empty && block[MOV.x[2]][MOV.y[2]+2]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[1], MOV.y[1]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=2;
					MOV.x[0]+=2; MOV.y[0]-=1; MOV.x[1]+=1; MOV.y[2]+=1; MOV.x[3]-=1; MOV.y[3]+=2;
				}
				break;
			}
		}								  // ----  --0-
		if(MOV.shape==6 && MOV.direct==2) // 0123  --1-
		{								  // ----  --2-
			if(MOV.x[0]==1)				  // ----  --3-
			{
				if(block[MOV.x[1]+1][MOV.y[1]]==empty && block[MOV.x[1]+2][MOV.y[1]]==empty && block[MOV.x[1]+3][MOV.y[1]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.y[0]+=1; MOV.x[1]+=1; MOV.x[2]+=2; MOV.y[2]-=1; MOV.x[3]+=3; MOV.y[3]-=2;
				}
				break;
			}
			if(MOV.x[0]==2)
			{
				if(block[MOV.x[1]-1][MOV.y[1]]==empty && block[MOV.x[1]+1][MOV.y[1]]==empty && block[MOV.x[1]+2][MOV.y[1]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]-=1; MOV.y[0]+=1; MOV.x[2]+=1; MOV.y[2]-=1; MOV.x[3]+=2; MOV.y[3]-=2;
				}
				break;
			}
			if(MOV.x[0]==hrz-1)
			{
				if(block[MOV.x[1]-3][MOV.y[1]]==empty && block[MOV.x[1]-2][MOV.y[1]]==empty && block[MOV.x[1]-1][MOV.y[1]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]-=3; MOV.y[0]+=1; MOV.x[1]-=2; MOV.x[2]-=1; MOV.y[2]-=1; MOV.y[3]-=2;
				}
				break;
			}
			if(3<=MOV.x[0] && MOV.x[0]<=hrz-2)
			{
				if(block[MOV.x[1]-2][MOV.y[1]]==empty && block[MOV.x[1]-1][MOV.y[1]]==empty && block[MOV.x[1]+1][MOV.y[1]]==empty)
				{
					gotoxy2(MOV.x[0], MOV.y[0]); printf("  ");
					gotoxy2(MOV.x[2], MOV.y[2]); printf("  ");
					gotoxy2(MOV.x[3], MOV.y[3]); printf("  ");
					MOV.direct=1;
					MOV.x[0]-=2; MOV.y[0]+=1; MOV.x[1]-=1; MOV.y[2]-=1; MOV.x[3]+=1; MOV.y[3]-=2; 
				}
				break;
			}
		}
		break;
	case SPACE_BAR:
		gotoxy2(hrz/2, vtc+4); printf("                      ");
		gotoxy2(hrz/2, vtc+3); printf("                      ");
		for(i=0; i<=3; i++)
		{
			if(highest[MOV.x[i]]<MOV.y[i])
				ifminus=1;
		}
		if(ifminus==0)
		{
			for(i=0; i<=3; i++)
				a[i]=highest[MOV.x[i]]-MOV.y[i];
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
			}
			for(i=0; i<=3; i++)
			{
				MOV.y[i]+=minimum(a)-1;
			}
		}
		if(ifminus==1)
		{
			a[0]=0; //�������� �� ĭ �� a[0]�� 0���� �ʱ�ȭ 
			for(i=0; i<=3; i++) c[i]=MOV.y[i];  //c�� MOV.y ���� 
			while(1)
			{
				for(i=0; i<=3; i++)
				{
					if(block[MOV.x[i]][c[i]+1]<=6 || c[i]==vtc-1)
						crashcheck=true;
				}
				if(crashcheck==true) 
					break;
				a[0]+=1;
				for(i=0; i<=3; i++) c[i]+=1;
			}
			for(i=0; i<=3; i++)
			{
				gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
			}
			for(i=0; i<=3; i++)
			{
				MOV.y[i]+=a[0];
			}
		}
		crashcheck=true;
		ifspacebar=true;
		break;
	case TAB:
		tspin=false;
		for(i=0; i<=3; i++)
		{
			gotoxy2(MOV.x[i], MOV.y[i]); printf("  ");
		}
		for(i=0; i<=3; i++)
			a[i]=highest[MOV.x[i]]-MOV.y[i];
		for(i=0; i<=3; i++)
		{
			gotoxy2(MOV.x[i], MOV.y[i]+minimum(a)-1); printf("  ");
		}
		if(holdblockshape==7)
		{
			holdblockshape=MOV.shape;
			blockmake();
			break;
		}
		if(holdblockshape<=6)
		{
			tempshape=holdblockshape;
			holdblockshape=MOV.shape;
			MOV.shape=tempshape;
			MOV.direct=1;
			for(i=-holdboxsize+1; i<=-holdboxsize+4; i++)
			{
				for(j=2; j<=3; j++)
				{
					gotoxy2(i, j); printf("  ");
				}
			}//holdbox�� ��µ� ��� ����� 
			blockdraw(holdblockshape, -holdboxsize);
			//holdbox ���
			if(MOV.shape==0)
			{
				MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;  
				MOV.x[1]=(hrz/2); MOV.y[1]=1;     //        01
				MOV.x[2]=(hrz/2)-1; MOV.y[2]=2;   //        23   
				MOV.x[3]=(hrz/2); MOV.y[3]=2;
			}
			if(MOV.shape==1)
			{
				MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;
				MOV.x[1]=(hrz/2); MOV.y[1]=1;     //       01
				MOV.x[2]=(hrz/2); MOV.y[2]=2;     //        23
				MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
			}
			if(MOV.shape==2)
			{
				MOV.x[0]=(hrz/2)+1; MOV.y[0]=1;
				MOV.x[1]=(hrz/2); MOV.y[1]=1;     //        10
				MOV.x[2]=(hrz/2); MOV.y[2]=2;     //       32
				MOV.x[3]=(hrz/2)-1; MOV.y[3]=2;
			}
			if(MOV.shape==3)
			{
				MOV.x[0]=(hrz/2); MOV.y[0]=1;
				MOV.x[1]=(hrz/2)-1; MOV.y[1]=2;     //        0
				MOV.x[2]=(hrz/2); MOV.y[2]=2;       //       123
				MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
			}
			if(MOV.shape==4)
			{
				MOV.x[0]=(hrz/2)-1; MOV.y[0]=1;
				MOV.x[1]=(hrz/2)-1; MOV.y[1]=2;     //       0
				MOV.x[2]=(hrz/2); MOV.y[2]=2;       //       123
				MOV.x[3]=(hrz/2)+1; MOV.y[3]=2;
			}
			if(MOV.shape==5)
			{
				MOV.x[0]=(hrz/2)+1; MOV.y[0]=1;
				MOV.x[1]=(hrz/2)+1; MOV.y[1]=2;     //       0
				MOV.x[2]=(hrz/2); MOV.y[2]=2;       //     321 
				MOV.x[3]=(hrz/2)-1; MOV.y[3]=2;
			}
			if(MOV.shape==6)
			{
				MOV.x[0]=(hrz/2)-2; MOV.y[0]=1;
				MOV.x[1]=(hrz/2)-1; MOV.y[1]=1;   //      0123
				MOV.x[2]=(hrz/2); MOV.y[2]=1;     //
				MOV.x[3]=(hrz/2)+1; MOV.y[3]=1;
			}
			break;
		}
	case PLUS:
		if(delay>5)
			delay-=1;
		break;
	case MINUS:
		if(delay<15)
			delay+=1;
		break;
	case PAUSE:
		while(1)
		{
			if(kbhit())
			{
				if(getch()==PAUSE)
					break;
			}
		}
		break;
	}
}

void scorecalculate()
{
	if(lines==1)
	{
		if(combo==1 || combo==2)	score++;
		if(combo==3 || combo==4)	score+=2;
		if(combo==5 || combo==6)	score+=3;
		if(combo>=7)	score+=4;
	}
	if(lines==2)
	{
		if(combo==0)	score++;
		if(combo==1 || combo==2)	score+=2;
		if(combo==3 || combo==4)	score+=3;
		if(5<=combo && combo<=7)	score+=4;
		if(8<=combo)	score+=5;
	}
	if(lines==3)
	{
		if(combo==0 || combo==1)	score+=2;
		if(combo==2 || combo==3)	score+=3;
		if(combo==4 || combo==5)	score+=4;
		if(combo==6 || combo==7)	score+=5;
		if(combo>=8)	score+=6;
	}
	if(lines==4)
	{
		if(combo==0)	score+=4;
		if(combo==1 || combo==2)	score+=5;
		if(3<=combo && combo<=5)	score+=6;
		if(6<=combo && combo<=8)	score+=7;
		if(9<=combo)	score+=8;
	}
}
void lose()
{
	WHITE;
	system("cls");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("���������������������������������\n");
	printf("                      ENTER��  ��������	       			        \n");
	while(getch()!=ENTER)
		true;
}
void rank()
{
	int i, j, k;
	int scores[11]={0}, temp, newrecord, ranking;
	char name[11][3]={0};
	char newname[11]={0};
	FILE *input = fopen("rank.txt","r");
	for(i=0;i<=9;i++)
	{
		fscanf(input,"%d",&scores[i]);
	}
	for(i=0; i<=9; i++)
	{
		for(j=0; j<=2; j++)
		{
			fscanf(input, "%d",&name[i][j]);
		}
	}
	fclose(input);
	remove("rank.txt");

	newrecord=score;
	
	for(i=9; i>=0; i--)
	{
		if(scores[i]>=newrecord)
		{
			ranking=i+2;
			for(j=9; j>=i+1;j--)
			{
				scores[j+1]=scores[j];
				for(k=0; k<=2; k++)
				{
					name[j+1][k]=name[j][k];
				}
			}
			break;
		}
	}
	

	if(newrecord>scores[0])
	{
		ranking=1;
		for(i=9; i>=0;i--)
		{
			scores[i+1]=scores[i];
			for(j=0; j<=2; j++)
			{
				name[i+1][j]=name[i][j];
			}
		}
	}
	scores[ranking-1]=newrecord;
	for(i=0; i<=2; i++)
	{
		name[ranking-1][i]=0;
	}
	
	gotoxy1(25, 1);
	printf("��ŷ");

	for(i=0; i<=9; i++)
	{
		gotoxy1(20, 3+i); printf("%2d�� :  %2d��     �̸�: %c%c%c", i+1, scores[i], name[i][0], name[i][1], name[i][2]);
	}
	if(ranking<=10)
	{
		gotoxy1(20, 15);
		printf("����� %d���Դϴ�. ", ranking);
		gotoxy1(20, 16); 
		printf("����� �̸��� �Է��ϼ���.");	
		
		while(1)
		{
			gotoxy1(43, 2+ranking);
			scanf("%s",newname);
			if(newname[2]==0 || newname[3]!=0)
			{
				gotoxy1(43,2+ranking);
				printf("                       ");
				gotoxy1(43,2+ranking);
				for(i=0;i<=9;i++)
				{
					newname[i]=0;
				}
				continue;
			}
			break;
		}

		
		for(i=0; i<=2; i++)
		{
			name[ranking-1][i]=newname[i];
		}
	}



	FILE *output = fopen("rank.txt","w");
	for(i=0;i<=9;i++)
	{
		fprintf(output, "%d\n", scores[i]);
	}
	for(i=0; i<=9; i++)
	{
		for(j=0; j<=2; j++)
		{
			fprintf(output, "%d\n", name[i][j]);
		}
	}
	fclose(output);

	while(getch()!=ENTER)
		i=1;
	system("cls");
}
void setcolor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}