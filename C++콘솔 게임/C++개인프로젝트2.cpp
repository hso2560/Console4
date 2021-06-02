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

//키 입력
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27

//색깔
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

#pragma region 전역변수

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

#pragma region 클래스

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

void setTone()  //음 설정
{
	music.insert(pair<string, float>("도", 523.25));
	music.insert(pair<string, float>("레", 587.33));
	music.insert(pair<string, float>("미", 659.26));
	music.insert(pair<string, float>("파", 698.46));
	music.insert(pair<string, float>("솔", 783.99));
	music.insert(pair<string, float>("라", 880));
	music.insert(pair<string, float>("시", 987.77));
	music.insert(pair<string, float>("높은 도", 1046.50));
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

void setMap(int x, int y)  //맵 형태 정함
{
	srand((unsigned int)time(NULL));

	int i, j, k;
	for (i = 0; i < x; i++) Stage[0][i] = "■";
	for (i = 0; i < x; i++) Stage[x - 1][i] = "■";
	for (i = 0; i < y; i++) Stage[i][0] = "■";
	for (i = 0; i < y; i++) Stage[i][y - 1] = "■";
	for (i = 0; i < y; i++)
	{
		for (j = 0; j < x; j++)
		{
			if (Stage[i][j] != "■")
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

		Stage[j][k] = "■";
	}
}

void setInit(int x, int y)  //처음 세팅
{
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			itemStage[i][j] = " ";
		}
	}
}

void playSound(float t = 523, float time = soundTime) //사운드 실행
{
	Beep(t, time);
}

void delay(clock_t ms)  //딜레이
{
	Sleep((DWORD)ms);
}

void gotoxy(int x, int y)  //좌표 지정
{
	HANDLE hOut;
	COORD Cur;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(hOut, Cur);
}

void clrscr()  //화면 지우기
{
	system("cls");
}

void scolor(unsigned short text = 15, unsigned short back = 0)  //색 변경
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
	cout << "재시작: G";
	gotoxy(15, 6);
	cout << "종료: ESC" << endl << "-------------------------------------------";

	scolor(YELLOW);
	gotoxy(2, 8);
	cout << "  *상점*  ";
	gotoxy(4, 9);
	cout << "          보유중인 코인: " << p->coin;

	scolor(GREEN);
	gotoxy(1, 13);
	cout << "저장하기(S): 100코인";
	scolor(CYAN);
	gotoxy(1, 15);
	cout << "-----능력치 UP-----";
	gotoxy(1, 16);
	cout << "움직임 횟수 증가(+30)(M): 50코인";
	gotoxy(1, 17);
	cout << "코인 획득량 증가(+5)(A): 120코인";
	gotoxy(1, 18);
	cout << "점수 획득량 증가(+2)(C): 400코인";
	gotoxy(1, 19);
	cout << "움직임 횟수 무시 확률 증가(+1%)(I): 200코인";
	scolor(LIGHTCYAN);
	gotoxy(1, 21);
	cout << "-----아이템 구입-----";
	gotoxy(1, 22);
	cout << "벽 충돌 1회 무시(V): 100코인";
	gotoxy(1, 23);
	cout << "정지 허용 시간 증가(+1초)(T): 150코인";
}

void shopBuy(Player* p, char ch)
{
	try
	{
		if (p->coin >= charPrice[ch])
		{
			p->coin -= charPrice[ch];
			playSound(music["파"], 50);

			if (ch == 's')  //저장
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
					shopMsg("해당 능력치는 더 이상 올릴 수 없습니다.");
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
			shopMsg("코인이 부족합니다.");
		}
	}
	catch (exception e) {}
}

void gameOver(string msg)  //게임오버
{
	isGameEnd = true;
	clrscr();
	scolor(RED);
	cout << "------------------GAME OVER------------------" << endl;
	cout << "       (" + msg + ")" << endl;
	cout << "------------------------------------------------" << endl;
	scolor();
}

void gameInit()  //시작
{
	fin.open("SaveFile.txt");
	if (!fin.fail()) {
		fin >> bestScore;
		fin.close();
	}

	scolor(YELLOW);
	cout << endl << endl << "------------------------Wall Star------------------------" << endl << endl << endl;
	scolor(LIGHTBLUE);
	cout << "---------------제한된 이동 횟수 안에 최대한 높은 점수를 얻어라!---------------" << endl << endl << endl;
	cout << "----------------------------아무 키나 누르면 시작합니다----------------------------" << endl << endl << endl;
	scolor(LIGHTGREEN);
	cout << "------------------------------------게임 설명--------------------------------" << endl << endl << endl;

	cout << "1. 화면 왼쪽 밑에 보이는 움직임 가능 최대 횟수 안에 최대한 높은 점수를 얻어야합니다." << endl;
	cout << "2. 점수와 최고점수는 화면 왼쪽 하단에 표시되며 오른쪽에서는 자신의 현재 능력치, 코인, 아이템 개수가 표시됩니다." << endl;
	cout << "3. 플레이어(⊙)는 방향키로 움직일 수 있으며 일정 시간 움직임이 없을 경우 게임오버가 됩니다." << endl;
	cout << "4. 움직임 가능 횟수를 전부 사용하거나 ■에 부딪힐 경우에도 게임오버가 됩니다." << endl;
	cout << "5. 상점에서는 플레이어의 능력치를 올리거나 아이템을 구매할 수 있습니다." << endl;
	cout << "6. 게임을 저장하기 위해서는 상점에서 100코인을 소모해서 저장을 해야합니다.(단, 최고점수는 자동으로 저장됩니다.)" << endl;
	cout << "7. ★: 점수를 획득합니다." << endl;
	cout << "8. $: 코인을 획득합니다." << endl;
	cout << "9. ▲: 점수가 감점됩니다." << endl;
	cout << "10. Ｘ: 코인을 일정량 잃습니다." << endl;
	cout << "11. Θ: 남은 움직임 가능 횟수가 추가로 감소됩니다." << endl;

	_getch();
}

void gameQuit()  //종료
{
	isGameEnd = true;
	isQuit = true;
	clrscr();
	scolor(BLACK, LIGHTCYAN);
	cout << "------------------게임종료------------------" << endl;
	scolor();
}

void showGameExplain()  //설명
{
	scolor();
	gotoxy(60, 2);
	cout << "방향키: 움직임" << endl;
	gotoxy(60, 3);
	cout << "ESC: 종료" << endl;
}

void showItemCnt(Player* p)  //아이템 개수
{
	scolor(YELLOW);
	gotoxy(90, 3);
	cout << "보유중인 코인: " << p->coin;
	gotoxy(80, 5);
	cout << "벽 충돌 무시 아이템(V) 개수: " << p->invinciCnt << "    ";
	gotoxy(80, 6);
	cout << "정지 허용 시간 증가 아이템(T) 개수: " << p->idleTimeCnt << "    ";
}

void star(Player* p, int count, string s)  //무언가 생성
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

void showStat(Player* p)  //능력치
{
	gotoxy(80, 8);
	cout << "움직임 가능 횟수: " << p->movableCnt;
	gotoxy(80, 9);
	cout << "코인 획득량(+): " << p->addCoin;
	gotoxy(80, 10);
	cout << "점수 획득량(+): " << p->addScore;
	gotoxy(80, 11);
	cout << "움직임 횟수 무시 확률: " << p->moveCntIgnPrb << "%";
	gotoxy(80, 14);
	if (isInvin) {
		cout << "벽 충돌시 무시 아이템 적용중(1회)";
	}
	else {
		cout << "                                 ";
	}
	gotoxy(80, 16);
	cout << "정지 허용 시간: " << delayMs << "초" << "    ";
}

void mapDraw()  //맵 그리기
{
	for (int i = 0; i < MAPSIZE_Y; i++)
	{
		for (int j = 0; j < MAPSIZE_X; j++)
		{
			scolor(GREEN);
			gotoxy(j * 2, i);          //string이라서 x좌표는 *2로 받음
			puts(Stage[i][j].c_str());

			if (itemStage[i][j] != " ")
			{
				if (itemStage[i][j] == "★" || itemStage[i][j] == "$")
					scolor(YELLOW);

				else if (itemStage[i][j] == "▲")
					scolor(GREEN);

				else if (itemStage[i][j] == "Ｘ")
					scolor(RED);
				else if (itemStage[i][j] == "Θ")
					scolor(DARKGRAY);

				gotoxy(j * 2, i);
				puts(itemStage[i][j].c_str());
			}


			if (Stage[i][j] == " ")  //이걸 안하면 플레이어 몸통 절반이 계속 그려짐
			{
				gotoxy(j * 2 - 1, i);
				puts(Stage[i][j].c_str());
			}

			if (Stage[i][j] == "■")
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
	cout << "점수: " << score;

	scolor(BLUE);
	gotoxy(3, 26);
	cout << "최고점수: " << bestScore;
}

void displayScreen(Player* p)  //화면 띄우기
{
	showGameExplain();
	showItemCnt(p);
	showStat(p);
	showScore();
	mapDraw();

	scolor(WHITE);
	gotoxy(p->curX * 2, p->curY);
	puts("⊙");

	gotoxy(23, 26);
	cout << "남은 이동 가능 횟수: " << moveCnt << "/" << p->movableCnt << "          ";
	//printf("남은 이동 가능 횟수: %d/%d", moveCnt, p->movableCnt);

	if (moveCnt == 0)
		gameOver("모든 이동 가능 횟수를 소모하였습니다.");

	gotoxy(55, 20);
}

void game_reset() //리셋
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

void move(int dir, Player* p)  //움직임
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

	if (Stage[p->curY + dir_y][p->curX + dir_x] != "■")
	{
		newTime = clock();
		p->beforePos.first = p->curX;
		p->beforePos.second = p->curY;
		p->curX += dir_x;
		p->curY += dir_y;
		playSound(music["미"], 30);

		if (difftime(newTime, oldTime) > delayMs)  //일정 시간 움직임이 없으면
		{
			gameOver("일정 시간동안 움직임이 없었습니다.");
		}

		else if (itemStage[p->curY][p->curX] == "★")  //별을 먹으면
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			star(p, 1, "★");
			playSound(music["높은 도"], 30);

			if (rand() % 5 == 2) score += (starScore2 + p->addScore);
			else score += (starScore1 + p->addScore);

			p->coin++;
		}
		else if (itemStage[p->curY][p->curX] == "$")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["시"], 30);
			p->coin += (10 + p->addCoin);
			mapCoinCount--;

			int c = rand() % 2 + 1;
			if (mapCoinCount + c > MAXCOINCNT)
				c--;
			star(p, c, "$");
		}
		else if (itemStage[p->curY][p->curX] == "▲")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["솔"], 30);

			score -= (rand() % 5 + 1);
			if (score < 0) score = 0;
			star(p, 1, "▲");
		}
		else if (itemStage[p->curY][p->curX] == "Ｘ")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["솔"], 30);

			p->coin -= (rand() % 2 + 1);
			if (p->coin < 0) p->coin = 0;
			star(p, 1, "Ｘ");
		}
		else if (itemStage[p->curY][p->curX] == "Θ")
		{
			itemStage[p->curY][p->curX] = " ";
			gotoxy(p->curX * 2, p->curY);
			puts(itemStage[p->curY][p->curX].c_str());
			playSound(music["솔"], 30);

			moveCnt -= (rand() % 2 + 1);
			if (moveCnt < 1) moveCnt = 1;
			star(p, 1, "Θ");
		}

		if (rand() % 100 >= p->moveCntIgnPrb)
			moveCnt--;
	}
	else
	{
		if (isInvin) isInvin = false;
		else {
			playSound(music["레"], 200);
			gameOver("벽과 충돌해서 사망하였습니다.");
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

	Player* player = new Player(3, 3, ab[0], ab[1], ab[2], ab[3], ab[4], ab[5], ab[6]); //나중에 저장한 값 집어넣기

RE:
	clrscr();
	setMap(MAPSIZE_X, MAPSIZE_Y);
	setInit(MAPSIZE_X, MAPSIZE_Y);


	moveCnt = player->movableCnt;
	star(player, STARCOUNT, "★");
	star(player, 3, "$");
	star(player, 3, "▲");
	star(player, 3, "Ｘ");
	star(player, 3, "Θ");
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