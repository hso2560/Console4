#pragma region include, namespace

#include<iostream>
#include <time.h>
#include<stdlib.h>
#include <Windows.h>
#include<conio.h>
#include<vector>
#include<string>
#include<map>
#include<fstream>
using namespace std;

#pragma endregion

#pragma region define

//Ű �Է�
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27

//����
#define BLACK 0 
#define BLUE 1 
#define GREEN 2 
#define CYAN 3 
#define RED 4 
#define MAGENTA 5 
#define BROWN 6 
#define LIGHTGRAY 7 
#define DARKGRAY 8 
#define LIGHTBLUE 9 
#define LIGHTGREEN 10 
#define LIGHTCYAN 11 
#define LIGHTRED 12 
#define LIGHTMAGENTA 13 
#define YELLOW 14 
#define WHITE 15 

//ETC
#define MAPSIZE_X 25
#define MAPSIZE_Y 25
#define ObstableCnt 80
#define STARCOUNT 5
#define MAXCOINCNT 10

#pragma endregion

#pragma region ��������

float soundTime = 500.0;
time_t newTime, oldTime;
map<string, float> music;
map<char, int> charPrice;
ofstream fout;
ifstream fin;
int starScore1 = 10, starScore2 = 20, starScore3 = 50;

int score = 0;
int bestScore = 0;
int moveCnt = 0;
int delayMs = 500;
int mapCoinCount = 0;
bool isGameEnd = false;
bool isQuit = false;
bool isIdle = false, isInvin = false;
string Stage[MAPSIZE_Y][MAPSIZE_X + 1];
string itemStage[MAPSIZE_Y][MAPSIZE_X + 1];

#pragma endregion

#pragma region Ŭ����

class Player
{
public:
	int curX, curY;
	int coin;  //1
	int movableCnt;  //2
	int addCoin;  //3
	int addScore;  //4
	int moveCntIgnPrb;  //5
	int invinciCnt;  //6
	int idleTimeCnt;  //7
	pair<int, int> beforePos = make_pair(2, 2);

public:
	Player(int x, int y, int m, int c, int a, int as, int mc, int iv, int it)
	{
		coin = c;
		curX = x;
		curY = y;
		movableCnt = m;
		addCoin = a;
		addScore = as;
		moveCntIgnPrb = mc;
		invinciCnt = iv;
		idleTimeCnt = it;
	}

};

#pragma endregion

void setTone()  //�� ����
{
	music.insert(pair<string, float>("��", 523.25));
	music.insert(pair<string, float>("��", 587.33));
	music.insert(pair<string, float>("��", 659.26));
	music.insert(pair<string, float>("��", 698.46));
	music.insert(pair<string, float>("��", 783.99));
	music.insert(pair<string, float>("��", 880));
	music.insert(pair<string, float>("��", 987.77));
	music.insert(pair<string, float>("���� ��", 1046.50));
}

void setShopPrice()
{
	charPrice.insert(pair<char, int>('s', 100));

	charPrice.insert(pair<char, int>('m', 50));
	charPrice.insert(pair<char, int>('a', 120));
	charPrice.insert(pair<char, int>('c', 400));
	charPrice.insert(pair<char, int>('i', 200));

	charPrice.insert(pair<char, int>('v', 100));
	charPrice.insert(pair<char, int>('t', 150));
}

void setMap(int x, int y)  //�� ���� ����
{
	srand((unsigned int)time(NULL));

	int i, j, k;
	for (i = 0; i < x; i++) Stage[0][i] = "��";
	for (i = 0; i < x; i++) Stage[x - 1][i] = "��";
	for (i = 0; i < y; i++) Stage[i][0] = "��";
	for (i = 0; i < y; i++) Stage[i][y - 1] = "��";
	for (i = 0; i < y; i++)
	{
		for (j = 0; j < x; j++)
		{
			if (Stage[i][j] != "��")
			{
				Stage[i][j] = " ";
			}
		}
	}

	for (i = 0; i < ObstableCnt; i++)
	{
		j = rand() % MAPSIZE_X + 1;
		k = rand() % MAPSIZE_Y + 1;

		if (j == 3 && k == 3) continue;

		Stage[j][k] = "��";
	}
}

void setInit(int x, int y)  //ó�� ����
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			itemStage[i][j] = " ";
		}
	}
}

void playSound(float t = 523, float time = soundTime) //���� ����
{
	Beep(t, time);
}

void delay(clock_t ms)  //������
{
	Sleep((DWORD)ms);
}

void gotoxy(int x, int y)  //��ǥ ����
{
	HANDLE hOut;
	COORD Cur;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(hOut, Cur);
}

void clrscr()  //ȭ�� �����
{
	system("cls");
}

void scolor(unsigned short text = 15, unsigned short back = 0)  //�� ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

void shopMsg(string msg)
{
	gotoxy(10, 10);
	scolor(BLACK, BLUE);
	cout << msg;
	delay(1000);
	scolor();
}

void shop(Player* p)
{
	scolor(BLUE);
	gotoxy(2, 6);
	cout << "�����: G";
	gotoxy(15, 6);
	cout << "����: ESC" << endl << "-------------------------------------------";

	scolor(YELLOW);
	gotoxy(2, 8);
	cout << "  *����*  ";
	gotoxy(4, 9);
	cout << "          �������� ����: " << p->coin;

	scolor(GREEN);
	gotoxy(1, 13);
	cout << "�����ϱ�(S): 100����";
	scolor(CYAN);
	gotoxy(1, 15);
	cout << "-----�ɷ�ġ UP-----";
	gotoxy(1, 16);
	cout << "������ Ƚ�� ����(+30)(M): 50����";
	gotoxy(1, 17);
	cout << "���� ȹ�淮 ����(+5)(A): 120����";
	gotoxy(1, 18);
	cout << "���� ȹ�淮 ����(+2)(C): 400����";
	gotoxy(1, 19);
	cout << "������ Ƚ�� ���� Ȯ�� ����(+1%)(I): 200����";
	scolor(LIGHTCYAN);
	gotoxy(1, 21);
	cout << "-----������ ����-----";
	gotoxy(1, 22);
	cout << "�� �浹 1ȸ ����(V): 100����";
	gotoxy(1, 23);
	cout << "���� ��� �ð� ����(+1��)(T): 150����";
}

void shopBuy(Player* p, char ch)
{
	try
	{
		if (p->coin >= charPrice[ch])
		{
			p->coin -= charPrice[ch];
			playSound(music["��"], 50);

			if (ch == 's')  //����
			{
				fout.open("PlayerSaveFile.txt");
				fout << to_string(p->movableCnt) + "|";
				fout << to_string(p->coin) + "|";
				fout << to_string(p->addCoin) + "|";
				fout << to_string(p->addScore) + "|";
				fout << to_string(p->moveCntIgnPrb) + "|";
				fout << to_string(p->invinciCnt) + "|";
				fout << to_string(p->idleTimeCnt);
				fout.close();
			}
			else if (ch == 'm')
			{
				p->movableCnt += 30;
			}
			else if (ch == 'a')
			{
				p->addCoin += 5;
			}
			else if (ch == 'c')
			{
				p->addScore += 2;
			}
			else if (ch == 'i')
			{
				if (p->moveCntIgnPrb >= 31)
					shopMsg("�ش� �ɷ�ġ�� �� �̻� �ø� �� �����ϴ�.");
				else
					p->moveCntIgnPrb++;
			}
			else if (ch == 'v')
			{
				p->invinciCnt++;
			}
			else if (ch == 't')
			{
				p->idleTimeCnt++;
			}
		}
		else
		{
			shopMsg("������ �����մϴ�.");
		}
	}
	catch (exception e) {}
}

void gameOver(string msg)  //���ӿ���
{
	isGameEnd = true;
	clrscr();
	scolor(RED);
	cout << "------------------GAME OVER------------------" << endl;
	cout << "       (" + msg + ")" << endl;
	cout << "------------------------------------------------" << endl;
	scolor();
}

void gameInit()  //����
{
	fin.open("SaveFile.txt");
	if (!fin.fail()) {
		fin >> bestScore;
		fin.close();
	}

	scolor(YELLOW);
	cout << endl << endl << "------------------------Wall Star------------------------" << endl << endl << endl;
	scolor(LIGHTBLUE);
	cout << "---------------���ѵ� �̵� Ƚ�� �ȿ� �ִ��� ���� ������ ����!---------------" << endl << endl << endl;
	cout << "----------------------------�ƹ� Ű�� ������ �����մϴ�----------------------------" << endl << endl << endl;
	scolor(LIGHTGREEN);
	cout << "------------------------------------���� ����--------------------------------" << endl << endl << endl;

	cout << "1. ȭ�� ���� �ؿ� ���̴� ������ ���� �ִ� Ƚ�� �ȿ� �ִ��� ���� ������ �����մϴ�." << endl;
	cout << "2. ������ �ְ������� ȭ�� ���� �ϴܿ� ǥ�õǸ� �����ʿ����� �ڽ��� ���� �ɷ�ġ, ����, ������ ������ ǥ�õ˴ϴ�." << endl;
	cout << "3. �÷��̾�(��)�� ����Ű�� ������ �� ������ ���� �ð� �������� ���� ��� ���ӿ����� �˴ϴ�." << endl;
	cout << "4. ������ ���� Ƚ���� ���� ����ϰų� �ῡ �ε��� ��쿡�� ���ӿ����� �˴ϴ�." << endl;
	cout << "5. ���������� �÷��̾��� �ɷ�ġ�� �ø��ų� �������� ������ �� �ֽ��ϴ�." << endl;
	cout << "6. ������ �����ϱ� ���ؼ��� �������� 100������ �Ҹ��ؼ� ������ �ؾ��մϴ�.(��, �ְ������� �ڵ����� ����˴ϴ�.)" << endl;
	cout << "7. ��: ������ ȹ���մϴ�." << endl;
	cout << "8. $: ������ ȹ���մϴ�." << endl;
	cout << "9. ��: ������ �����˴ϴ�." << endl;
	cout << "10. ��: ������ ������ �ҽ��ϴ�." << endl;
	cout << "11. ��: ���� ������ ���� Ƚ���� �߰��� ���ҵ˴ϴ�." << endl;

	_getch();
}

void gameQuit()  //����
{
	isGameEnd = true;
	isQuit = true;
	clrscr();
	scolor(BLACK, LIGHTCYAN);
	cout << "------------------��������------------------" << endl;
	scolor();
}

void showGameExplain()  //����
{
	scolor();
	gotoxy(60, 2);
	cout << "����Ű: ������" << endl;
	gotoxy(60, 3);
	cout << "ESC: ����" << endl;
}

void showItemCnt(Player* p)  //������ ����
{
	scolor(YELLOW);
	gotoxy(90, 3);
	cout << "�������� ����: " << p->coin;
	gotoxy(80, 5);
	cout << "�� �浹 ���� ������(V) ����: " << p->invinciCnt << "    ";
	gotoxy(80, 6);
	cout << "���� ��� �ð� ���� ������(T) ����: " << p->idleTimeCnt << "    ";
}

void star(Player* p, int count, string s)  //���� ����
{
	srand((unsigned int)time(NULL));
	int x, y;

	for (int i = 0; i < count; i++)
	{
		x = rand() % MAPSIZE_X + 1;
		y = rand() % MAPSIZE_Y + 1;

		if (Stage[x][y] == " " && itemStage[x][y] == " ")
		{
			if (p->curX != x || p->curY != y)
			{
				itemStage[x][y] = s;
			}
			else i--;
		}
		else i--;
	}

}

void showStat(Player* p)  //�ɷ�ġ
{
	gotoxy(80, 8);
	cout << "������ ���� Ƚ��: " << p->movableCnt;
	gotoxy(80, 9);
	cout << "���� ȹ�淮(+): " << p->addCoin;
	gotoxy(80, 10);
	cout << "���� ȹ�淮(+): " << p->addScore;
	gotoxy(80, 11);
	cout << "������ Ƚ�� ���� Ȯ��: " << p->moveCntIgnPrb << "%";
	gotoxy(80, 14);
	if (isInvin) {
		cout << "�� �浹�� ���� ������ ������(1ȸ)";
	}
	else {
		cout << "                                 ";
	}
	gotoxy(80, 16);
	cout << "���� ��� �ð�: " << delayMs << "��" << "    ";
}

void mapDraw()  //�� �׸���
{
	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			scolor(GREEN);
			gotoxy(j * 2, i);          //string�̶� x��ǥ�� *2�� ����
			puts(Stage[i][j].c_str());

			if (itemStage[i][j] != " ")
			{
				if (itemStage[i][j] == "��" || itemStage[i][j] == "$")
					scolor(YELLOW);

				else if (itemStage[i][j] == "��")
					scolor(GREEN);

				else if (itemStage[i][j] == "��")
					scolor(RED);
				else if (itemStage[i][j] == "��")
					scolor(DARKGRAY);

				gotoxy(j * 2, i);
				puts(itemStage[i][j].c_str());
			}


			if (Stage[i][j] == " ")  //�̰� ���ϸ� �÷��̾� ���� ������ ��� �׷���
			{
				gotoxy(j * 2 - 1, i);
				puts(Stage[i][j].c_str());
			}

			if (Stage[i][j] == "��")
			{
				if (Stage[i][j - 1] == " ")
				{
					gotoxy((j - 1) * 2 + 1, i);
					puts(Stage[i][j - 1].c_str());
				}
			}
		}
		cout << endl;
	}
	scolor();
}

void showScore()
{
	scolor(LIGHTBLUE);
	gotoxy(3, 25);
	cout << "����: " << score;

	scolor(BLUE);
	gotoxy(3, 26);
	cout << "�ְ�����: " << bestScore;
}

void displayScreen(Player* p)  //ȭ�� ����
{
	showGameExplain();
	showItemCnt(p);
	showStat(p);
	showScore();
	mapDraw();

	scolor(WHITE);
	gotoxy(p->curX * 2, p->curY);
	puts("��");

	gotoxy(23, 26);
	cout << "���� �̵� ���� Ƚ��: " << moveCnt << "/" << p->movableCnt << "          ";
	//printf("���� �̵� ���� Ƚ��: %d/%d", moveCnt, p->movableCnt);

	if (moveCnt == 0)
		gameOver("��� �̵� ���� Ƚ���� �Ҹ��Ͽ����ϴ�.");

	gotoxy(55, 20);
}

void game_reset() //����
{
	if (score > bestScore) {
		bestScore = score;
		fout.open("SaveFile.txt");
		fout << bestScore;
		fout.close();
	}

	isGameEnd = false;
	score = 0;

	delayMs = 500;
	isIdle = false;
	isInvin = false;

	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			Stage[i][j] = "";
		}
	}
}

void move(int dir, Player* p)  //������
{
	int dir_x = 0, dir_y = 0;

	switch (dir)
	{
	case LEFT:
		dir_x = -1;
		break;
	case RIGHT:
		dir_x = 1;
		break;
	case UP:
		dir_y = -1;
		break;
	case DOWN:
		dir_y = 1;
		break;
	}

	if (Stage[p->curY + dir_y][p->curX + dir_x] != "��")
	{
		newTime = clock();
		p->beforePos.first = p->curX;
		p->beforePos.second = p->curY;
		p->curX += dir_x;
		p->curY += dir_y;
		playSound(music["��"], 30);

		if (difftime(newTime, oldTime) > delayMs)  //���� �ð� �������� ������
		{
			gameOver("���� �ð����� �������� �������ϴ�.");
		}

		else if (itemStage[p->curY][p->curX] == "��")  //���� ������
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			star(p, 1, "��");
			playSound(music["���� ��"], 30);

			if (rand() % 5 == 2) score += (starScore2 + p->addScore);
			else score += (starScore1 + p->addScore);

			p->coin++;
		}
		else if (itemStage[p->curY][p->curX] == "$")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["��"], 30);
			p->coin += (10 + p->addCoin);
			mapCoinCount--;

			int c = rand() % 2 + 1;
			if (mapCoinCount + c > MAXCOINCNT)
				c--;
			star(p, c, "$");
		}
		else if (itemStage[p->curY][p->curX] == "��")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["��"], 30);

			score -= (rand() % 5 + 1);
			if (score < 0) score = 0;
			star(p, 1, "��");
		}
		else if (itemStage[p->curY][p->curX] == "��")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["��"], 30);

			p->coin -= (rand() % 2 + 1);
			if (p->coin < 0) p->coin = 0;
			star(p, 1, "��");
		}
		else if (itemStage[p->curY][p->curX] == "��")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["��"], 30);

			moveCnt -= (rand() % 2 + 1);
			if (moveCnt < 1) moveCnt = 1;
			star(p, 1, "��");
		}

		if (rand() % 100 >= p->moveCntIgnPrb)
			moveCnt--;
	}
	else
	{
		if (isInvin) isInvin = false;
		else {
			playSound(music["��"], 200);
			gameOver("���� �浹�ؼ� ����Ͽ����ϴ�.");
		}
	}
}

void setConsoleView()
{
	system("title Wall Star");
}

int main()
{
	setConsoleView();
	srand((unsigned int)time(NULL));
	setTone();
	setShopPrice();
	gameInit();

	string savedStr;
	int ab[7] = { 100,0,0,0,0,0,0 };
	int n = 0;
	int n2 = 0;

	fin.open("PlayerSaveFile.txt");
	if (!fin.fail()) {
		fin >> savedStr;
		string ss;
		for (int i = 0; i < savedStr.size(); i++)
		{
			if (savedStr[i] == '|') {
				ss = savedStr.substr(n, i - n);
				ab[n2] = stoi(ss);
				n = i + 1;
				n2++;
			}
		}

		fin.close();
	}

	Player* player = new Player(3, 3, ab[0], ab[1], ab[2], ab[3], ab[4], ab[5], ab[6]); //���߿� ������ �� ����ֱ�

RE:
	clrscr();
	setMap(MAPSIZE_X, MAPSIZE_Y);
	setInit(MAPSIZE_X, MAPSIZE_Y);


	moveCnt = player->movableCnt;
	star(player, STARCOUNT, "��");
	star(player, 3, "$");
	star(player, 3, "��");
	star(player, 3, "��");
	star(player, 3, "��");
	mapCoinCount += 3;
	int ch;

	while (!isGameEnd)
	{
		displayScreen(player);
		oldTime = clock();
		ch = _getch();

		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();
			move(ch, player);
		}
		else
		{
			ch = tolower(ch);

			if (ch == ESC)
			{
				gameQuit();
			}
			else if (ch == 'v')
			{
				if (player->invinciCnt > 0 && !isInvin) {
					player->invinciCnt--;
					isInvin = true;
				}
			}
			else if (ch == 't')
			{
				if (player->idleTimeCnt > 0 && !isIdle) {
					player->idleTimeCnt--;
					isIdle = true;
					delayMs = 1500;
				}
			}
		}

		if (isGameEnd)
		{
			break;
		}
	}

	delay(2000);

	while (true)
	{
		clrscr();
		shop(player);
		ch = _getch();
		ch = tolower(ch);

		if (ch == ESC)
		{
			isQuit = true;
			gameQuit();
			break;
		}
		else if (ch == 'g')
		{
			break;
		}
		else {
			shopBuy(player, ch);
		}
	}

	if (!isQuit)
	{
		game_reset();

		goto RE;
	}

	delete player;
}