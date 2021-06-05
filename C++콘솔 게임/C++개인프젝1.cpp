#include <iostream>
#include<ctime>
#include<cstdlib>
#include <windows.h>
#include <conio.h>
#include<fstream>
using namespace std;

#pragma region ��������
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
#pragma endregion

#pragma region ��������
int food_x;  //�� ��ġ
int food_y;  //�� ��ġ
int foodX;  //�� ��ġ ����
int foodY;   //�� ��ġ ����

int score;  //����
int bestScore = 0; //����
int _count = 500;  //500���� ������ �ȿ� ���� ���� ���� ȹ���ϱ� ����.
int speed;  //���� �ӵ�
int length = 5; //���� ����

int item_x, item_y;  //������ ��ġ
int itemX, itemY;  //������ ��ġ ����
bool isItem = false, isP = false;  //�����۰� �ι�° ��
bool A = false;  //true�� ���� ����
bool B = false; //true�� ���� �ٽ� ����

int poison_x, poison_y;  //���� �Ǵ� ������ ��ġ
int poisonX, poisonY;  //``��ġ ����
int px, py;  //�� ��ġ
int pX, pY;  //�� ��ġ ����

int recX, recY;  //��Ź� ��ġ
int recX2, recY2;
int recX3, recY3;
int recX4, recY4;
int recX5, recY5;
int recX6, recY6;

int ran_x, ran_y, ranX, ranY; //��������ġ�� ����
string pswd;  //��й�ȣ

ofstream fout;
ifstream fin;
#pragma endregion

int GetKey(void)
{
	int ch = _getch();
	if (ch == 0 || ch == 224)	// ����Ű�� ��� 0 �Ǵ� 224�� ���� ���� �Էµ�
		ch = _getch();	// �� ������ �ش� ����Ű�� ���� 72(Up),80(Down), 75(Left), 77(Right) ���� �Էµ�
	return ch;
}

void scolor(unsigned short text = 15, unsigned short back = 0)  //�� ����
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}

void CursorView(char show) //Ŀ�������
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void PrintTo(int x, int y, const char* s) //x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է�
{
	COORD pos = { (short)(2 * x), (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	cout << s;
}

void MapDraw()  //�� �׸�
{
	// draw map
	scolor(BROWN, 0);
	for (size_t i = 0; i < 25; i++) { PrintTo(i, 0, "��"); }
	for (size_t i = 0; i < 25; i++) { PrintTo(i, 24, "��"); }
	for (size_t j = 1; j < 24; j++)
	{
		PrintTo(0, j, "��");	PrintTo(24, j, "��");
	}
}

void Food() //�� ������ ��ġ ����
{
	srand((unsigned)time(NULL));
	food_x = (rand() % 23) + 1;
	food_y = (rand() % 23) + 1;

	scolor(YELLOW, 0);
	PrintTo(food_x, food_y, "��");
	foodX = food_x;
	foodY = food_y;
}

void Item()  //��Ʈ ������ ����
{
	srand((unsigned)time(NULL) + 3);
	item_x = (rand() % 23) + 1;
	item_y = (rand() % 23) + 1;

	scolor(GREEN, 0);
	PrintTo(item_x, item_y, "��");
	itemX = item_x;
	itemY = item_y;
}

void Poison()  //�� ������(�ﰢ��) ����
{

	srand((unsigned)time(NULL) - 3);
	poison_x = (rand() % 23) + 1;
	poison_y = (rand() % 23) + 1;

	scolor(RED, 0);
	PrintTo(poison_x, poison_y, "��");
	poisonX = poison_x;
	poisonY = poison_y;

}

void Poison2()  //�ι�° �� ������ ����
{
	srand((unsigned)time(NULL) - 2);
	px = (rand() % 23) + 1;
	py = (rand() % 23) + 1;

	scolor(RED, 0);
	PrintTo(px, py, "��");
	pX = px;
	pY = py;
}

void WallOut()  //���̳� �����̴� ��ֹ��� ������ ���
{
	Sleep(500);
	if (score > bestScore) {  //�ְ����̸� �����ϰ� ��й�ȣ ������
		bestScore = score;
		PrintTo(5, 5, "------------�ְ������Դϴ�. �� ��й�ȣ�� �����ϼ���------------");
		string a;
		cin >> a;

		PrintTo(5, 5, "------------��й�ȣ ���� �Ϸ�------------");
		pswd = a;
		fout.open("passfile.txt"); fout << pswd;  fout.close();
		Sleep(2000);
	}

	fout.open("scorefile.txt");  //�ְ��� ������.
	fout << bestScore;
	fout.close();

	int key;

	while (true) {
		scolor(RED);
		PrintTo(5, 5, "------------����Ͽ����ϴ�. �ƹ� Ű�� �����ּ���.------------");


		if (_kbhit)
		{
			key = _getch();

			Sleep(1000);
			B = true;
			break;
		}
	}
}

void _Rand()  //�� or �� �Ǵ� ������ ����
{
	srand((unsigned)time(NULL) + 2);
	ran_x = (rand() % 23) + 1;
	ran_y = (rand() % 23) + 1;

	scolor(CYAN, 0);
	PrintTo(ran_x, ran_y, "?");
	ranX = ran_x;
	ranY = ran_y;
}

void Move(int x, int y)  //�÷��̾�� ������ Ȥ�� ���� �浹ó��
{
	if (x == food_x)
	{
		if (y == food_y)
		{
			score += 10;
			Food();
			Poison();
			if (speed > 1)
				speed -= 3;
		}
	}
	else if (x == item_x)
	{
		if (y == item_y)
		{
			score += 1;
			Item();
		}
	}

	else if (x == poison_x)
	{
		if (y == poison_y)
		{
			score -= 2;
			Poison();
		}
	}
	else if (x == px)
	{
		if (y == py) {
			score -= 5;
			Poison2();
		}
	}
	else if (x == ran_x)
	{
		if (y == ran_y)
		{
			srand((unsigned)time(NULL));
			int a = rand() % 2 + 10;
			if (a == 10)
				_count += 10;
			else
				_count -= 10;
			_Rand();
		}
	}

	else if (x == recX)
	{
		if (y == recY)
		{
			PrintTo(recX, recY, "��");
			WallOut();
		}
	}

	else if (x == recX2)
	{
		if (y == recY2)
		{
			PrintTo(recX2, recY2, "��");
			WallOut();
		}
	}
	else if (x == recX3)
	{
		if (y == recY3)
		{
			PrintTo(recX3, recY3, "��");
			WallOut();
		}
	}
	else if (x == recX4)
	{
		if (y == recY4)
		{
			PrintTo(recX4, recY4, "��");
			WallOut();
		}
	}
	else if (x == recX5)
	{
		if (y == recY5)
		{
			PrintTo(recX5, recY5, "��");
			WallOut();
		}
	}
	else if (x == recX6)
	{
		if (y == recY6)
		{
			PrintTo(recX6, recY6, "��");
			WallOut();
		}
	}
}

void Start()  //�������� �����۵�� ���� ��� ���� ����
{
	scolor(LIGHTBLUE, LIGHTGREEN);
	PrintTo(27, 4, "���� ������ 10�� �߰�");

	scolor(LIGHTMAGENTA, WHITE);
	PrintTo(27, 1, "����Ű�� ������ �� �ֽ��ϴ�");

	scolor(MAGENTA, WHITE);
	PrintTo(27, 2, "���� �̵� ���� Ƚ��: ");
	cout << _count;

	scolor(GREEN, 0);
	PrintTo(1, 25, "����: ");
	cout << score;
	PrintTo(7, 25, "�ְ�����: ");
	cout << bestScore;

	scolor(RED, 0);
	PrintTo(27, 8, "��: 2�� ����");
	PrintTo(27, 9, "��: 5�� ����");
	scolor(GREEN, 0);
	PrintTo(27, 7, "��: 1�� �߰�");
	scolor(CYAN, 0);
	PrintTo(27, 10, "?: �����̵�����Ƚ�� 10�� �߰� Ȥ�� ����");

	scolor(BLACK, WHITE);
	PrintTo(27, 13, "P: �Ͻ�����");
	PrintTo(27, 14, "ESC: ��������");

	scolor(BROWN);
	PrintTo(27, 12, "��: ���� �� ���");
}

void RePrint() //cls�� ���ؼ� �����۵� ��������ʰ� ����
{
	scolor(YELLOW, 0);
	PrintTo(foodX, foodY, "��");
	scolor(RED, 0);
	PrintTo(poisonX, poisonY, "��");
	scolor(CYAN, 0);
	PrintTo(ranX, ranY, "?");
	scolor(BROWN);
	PrintTo(recX, recY, "��");
	if (speed <= 60)
		PrintTo(recX2, recY2, "��");
	if (speed <= 45)
		PrintTo(recX3, recY3, "��");
	if (speed <= 30)
		PrintTo(recX4, recY4, "��");
	if (speed <= 18)
		PrintTo(recX5, recY5, "��");

	if (speed < 60) {
		if (isItem == false) {
			isItem = true;
			Item();
		}
		else {
			scolor(GREEN, 0);
			PrintTo(itemX, itemY, "��");
		}
	}
	if (speed < 50)
	{
		if (isP == false)
		{
			isP = true;
			Poison2();
		}
		else {
			scolor(RED, 0);
			PrintTo(pX, pY, "��");
		}
	}
}

void Tail(int& x, int& y, int* a, int* b)  //�÷��̾� ���� �κ��� ������
{
	for (int i = 4; i > 0; i--)
	{
		a[i] = a[i - 1]; b[i] = b[i - 1];
	}
	a[0] = x; b[0] = y;
}

void TimeOver()  //���� �̵� ���� Ƚ�� �� ���� ���� ������ ó��
{
	Sleep(500);
	if (score > bestScore) {  //�ְ����̸� �����ϰ� ��й�ȣ ������
		bestScore = score;

		PrintTo(5, 5, "------------�ְ������Դϴ�. �� ��й�ȣ�� �����ϼ���------------");
		string a;
		cin >> a;

		PrintTo(5, 5, "------------��й�ȣ ���� �Ϸ�------------");
		pswd = a;
		fout.open("passfile.txt"); fout << pswd;  fout.close();
		Sleep(2000);
	}

	fout.open("scorefile.txt");  //�ְ��� ������.
	fout << bestScore;
	fout.close();

	int key;

	while (true) {
		PrintTo(5, 5, "------------�̵�Ƚ���� ��� �Ҹ��Ͽ����ϴ�. �ƹ� Ű�� �����ּ���.------------");

		if (_kbhit)
		{
			key = _getch();

			Sleep(1000);
			B = true;
			break;
		}
	}
}

void Pause()  //�Ͻ� ����
{
	int key;

	scolor(LIGHTMAGENTA);
	PrintTo(4, 4, "< �Ͻ�����: P�� ������ ����մϴ� >");

	while (1) {
		if (_kbhit)
		{
			key = _getch();

			if (key == 112)
			{
				PrintTo(4, 4, "                                        ");
				Sleep(200);
				break;
			}
		}
	}
}

void Obstacle()  //��ֹ�(�����̴� ��) ����
{

	srand((unsigned)time(NULL) - 1);

	recX = (rand() % 23) + 1;
	recY = (rand() % 23) + 1;

	scolor(BROWN);
	PrintTo(recX, recY, "��");
	if (speed <= 60)
	{
		srand((unsigned)time(NULL) + 5);

		recX2 = (rand() % 23) + 1;
		recY2 = (rand() % 23) + 1;

		scolor(BROWN);
		PrintTo(recX2, recY2, "��");
	}
	if (speed <= 45)
	{
		srand((unsigned)time(NULL) + 4);

		recX3 = (rand() % 23) + 1;
		recY3 = (rand() % 23) + 1;

		scolor(BROWN);
		PrintTo(recX3, recY3, "��");
	}
	if (speed <= 30)
	{
		srand((unsigned)time(NULL) + 3);

		recX4 = (rand() % 23) + 1;
		recY4 = (rand() % 23) + 1;

		scolor(BROWN);
		PrintTo(recX4, recY4, "��");
	}
	if (speed <= 18)
	{
		srand((unsigned)time(NULL) + 2);

		recX5 = (rand() % 23) + 1;
		recY5 = (rand() % 23) + 1;

		scolor(BROWN);
		PrintTo(recX5, recY5, "��");
	}
	if (speed <= 9)
	{
		srand((unsigned)time(NULL) - 3);

		recX6 = (rand() % 23) + 1;
		recY6 = (rand() % 23) + 1;

		scolor(BROWN);
		PrintTo(recX6, recY6, "��");
	}
}

void MoveObs()  //��ֹ�(�����̴� ��)�� ������ ����
{

	srand((unsigned)time(NULL) - 5);
	int m;
	m = rand() % 4;

	if (m == 0) {
		recX++;
		if (recX > 23)
			recX--;
	}
	else if (m == 1) {
		recX--;
		if (recX < 1)
			recX++;
	}
	else if (m == 2) {
		recY++;
		if (recY > 23)
			recY--;
	}
	else {
		recY--;
		if (recY < 1)
			recY++;
	}
	if (speed <= 60)
	{
		srand((unsigned)time(NULL) + 4);
		int n;
		n = rand() % 4;

		if (n == 0) {
			recX2++;
			if (recX2 > 23)
				recX2--;
		}
		else if (n == 1) {
			recX2--;
			if (recX2 < 1)
				recX2++;
		}
		else if (n == 2) {
			recY2++;
			if (recY2 > 23)
				recY2--;
		}
		else {
			recY2--;
			if (recY2 < 1)
				recY2++;
		}
	}
	if (speed <= 45)
	{
		srand((unsigned)time(NULL) + 3);
		int u;
		u = rand() % 4;

		if (u == 0) {
			recX3++;
			if (recX3 > 23)
				recX3--;
		}
		else if (u == 1) {
			recX3--;
			if (recX3 < 1)
				recX3++;
		}
		else if (u == 2) {
			recY3++;
			if (recY3 > 23)
				recY3--;
		}
		else {
			recY3--;
			if (recY3 < 1)
				recY3++;
		}
	}
	if (speed <= 30)
	{
		srand((unsigned)time(NULL) + 2);
		int t;
		t = rand() % 4;

		if (t == 0) {
			recX4++;
			if (recX4 > 23)
				recX4--;
		}
		else if (t == 1) {
			recX4--;
			if (recX4 < 1)
				recX4++;
		}
		else if (t == 2) {
			recY4++;
			if (recY4 > 23)
				recY4--;
		}
		else {
			recY4--;
			if (recY4 < 1)
				recY4++;
		}
	}
	if (speed <= 18)
	{
		srand((unsigned)time(NULL) - 2);
		int tt;
		tt = rand() % 4;

		if (tt == 0) {
			recX5++;
			if (recX5 > 23)
				recX5--;
		}
		else if (tt == 1) {
			recX5--;
			if (recX5 < 1)
				recX5++;
		}
		else if (tt == 2) {
			recY5++;
			if (recY5 > 23)
				recY5--;
		}
		else {
			recY5--;
			if (recY5 < 1)
				recY5++;
		}
	}
	if (speed <= 9)
	{
		srand((unsigned)time(NULL) + 6);
		int t2;
		t2 = rand() % 4;

		if (t2 == 0) {
			recX6++;
			if (recX6 > 23)
				recX6--;
		}
		else if (t2 == 1) {
			recX6--;
			if (recX6 < 1)
				recX6++;
		}
		else if (t2 == 2) {
			recY6++;
			if (recY6 > 23)
				recY6--;
		}
		else {
			recY6--;
			if (recY6 < 1)
				recY6++;
		}
	}
}

void PlayerDraw(int x, int y, int* a, int* b)  //�÷��̾� �ٽ� �׸���
{
	scolor(LIGHTGREEN, 0);
	PrintTo(x, y, "��");
	scolor(BLUE, 0);
	for (int i = 0; i < length; i++)
	{
		PrintTo(a[i], b[i], "��");
	}
}

int main()
{
	fin.open("scorefile.txt");  //�ְ����� �ҷ�����
	if (!fin.fail())
	{
		fin >> bestScore;
		fin.close();
	}

RE:

	score = 0;
	_count = 500;
	speed = 70;
	Start();
	int x, y, key;
	x = 10;	y = 10;
	int a[5], b[5];  //���� ��ġ

	scolor(LIGHTGREEN, 0);
	PrintTo(x, y, "��");
	scolor(BLUE, 0);
	for (int i = 0; i < length; i++)
	{
		PrintTo(x - (i + 1), y, "��");
		a[i] = x - (i + 1);  b[i] = y;
	}
	Poison();
	Food();
	Obstacle();
	_Rand();
	CursorView(false);
	MapDraw();

	while (true)  //��ŸƮ ���� ȭ��
	{
		if (B) {
			B = false;
			break;
		}
		scolor(BLACK, WHITE);
		PrintTo(5, 5, "------------�ƹ�Ű�� ������ �����մϴ�. ����: ESC. �ְ����� �ʱ�ȭ: R------------");
		int key;
		if (_kbhit) {
			key = _getch();
			//cout << key;   //Ű���� �ش��ϴ� �� ã��
			//cin >> key;
			if (key == 27) { goto Quit; }  //esc������ ���� ����
			else if (key == 114)  //R������ �ְ����� �ʱ�ȭ ����(���� �Է��� ��й�ȣ ����� ��.)
			{
				PrintTo(5, 5, "------------��й�ȣ�� �Է��ϼ���.------------");
				fin.open("passfile.txt"); fin >> pswd;  string p = pswd; fin.close();

				string b; cin >> b;
				if (b == p)
				{
					fout.open("scorefile.txt");
					bestScore = 0;
					fout << bestScore;
					fout.close();
					PrintTo(5, 5, "------------�ְ����� �ʱ�ȭ �Ϸ�------------");
					Sleep(2000);
					goto RE;
				}
				else {
					PrintTo(5, 5, "------------�ְ����� �ʱ�ȭ ����------------");
					Sleep(2000);
					goto RE;
				}
			}
			else { break; }
		}
	}

	while (1) // while(1)�� ���� �ݺ� ������ ���� ����ϴ� ��������, break ���� ������ ������ ���� ����
	{
		MapDraw();

		key = GetKey();


		//if (key == 'q' || key == 'Q' || key == 55) { if (y > 1 && x > 1) { x = x - 1; y = y - 1; for (int i = 0; i < length; i++) { a[i] = x - (i + 1); b[i] = y-(1+i); } } } //�»� �밢��
		//else if (key == 'e' || key == 'E' || key == 57) { if (y > 1 && x < 23) { x = x + 1; y = y - 1; for (int i = 0; i < length; i++) { a[i] = x + (i + 1); b[i] = y - (1 + i); }		} } //��� �밢��
		//else if (key == 'z' || key == 'Z' || key == 49) { if (y < 23 && x >  1) { x = x - 1; y = y + 1; for (int i = 0; i < length; i++) { a[i] = x - (i + 1); b[i] = y + (1 + i); }} } //���� �밢��
		//else if (key == 'c' || key == 'C' || key == 51) { if (y < 23 && x < 23) { x = x + 1; y = y + 1; for (int i = 0; i < length; i++) { a[i] = x + (i + 1); b[i] = y + (1 + i); }		} } //���� �밢��
		//else if (key == 'a' || key == 'A' || key == 52) { if (x > 1) x = x - 1; for (int i = 0; i < length; i++) { a[i] = x - (i + 1); }}	// 52,54,56,50 : 4,6,8,2 ����Ű�е�
		//else if (key == 'd' || key == 'D' || key == 54) { if (x < 23) x = x + 1; for (int i = 0; i < length; i++) { a[i] = x + (i + 1);  }	}
		//else if (key == 'w' || key == 'W' || key == 56) { if (y > 1) y = y - 1; for (int i = 0; i < length; i++) {  b[i] = y - (1 + i); }}
		//else if (key == 's' || key == 'S' || key == 50) { if (y < 23) y = y + 1; for (int i = 0; i < length; i++) { b[i] = y + (1 + i); }}
		if (key == 75)
		{
			Tail(x, y, a, b);
			if (x > 1)
				x = x - 1;
			else
			{
				x = x - 1;
				PlayerDraw(x, y, a, b);
				WallOut();
			}
		}	// �� ����Ű
		else if (key == 77)
		{
			Tail(x, y, a, b);
			if (x < 23) x = x + 1;
			else { x = x + 1;  PlayerDraw(x, y, a, b); WallOut(); }
		}	// �� ����Ű
		else if (key == 72)
		{
			Tail(x, y, a, b);
			if (y > 1) y = y - 1;
			else
			{
				y = y - 1;  PlayerDraw(x, y, a, b); WallOut();
			}
		}	// �� ����Ű
		else if (key == 80)
		{
			Tail(x, y, a, b);
			if (y < 23) y = y + 1;
			else { y = y + 1;  PlayerDraw(x, y, a, b); WallOut(); }
		}// �� ����Ű

		MoveObs();
		Move(x, y);

		if (A)
			goto Quit;
		else if (B) {
			B = false;
			goto RE;
		}

		//if (key == 32) // space bar
		//{
		//}
		if (key == 13) //enter //VK_RETURN
		{
			_count++;
			system("cls");
		}
		if (key == 27) //esc  //VK_ESCAPE
		{
			_count++;
			PrintTo(8, 0, "---���� ����---");
			break;
		}
		if (key == 112)  //�Ͻ�����
		{
			_count++;
			Pause();
		}

		//display -----------------------------------
		Sleep(speed);  //�÷��̾� ������ ������(������ ��ü���� �ӵ�)
		system("cls");

		// draw map
		MapDraw();

		scolor(LIGHTGREEN, 0);
		PrintTo(x, y, "��");  //�Ӹ��κ� ��ġ ���� ó��
		scolor(BLUE, 0);
		for (int i = 0; i < length; i++)  //���� �κ� ��ġ ���� ó��
		{
			PrintTo(a[i], b[i], "��");
		}

		_count--;

		RePrint();

		Start();

		if (_count == 0)  //�̵�Ƚ�� �ٽ��� ���� ó��. (�ٽý���)
		{
			TimeOver();
			if (A)
				break;
			else if (B) {
				B = false;
				goto RE;
			}
		}

	}

Quit:
	cout << "������ �����մϴ�" << endl;

	return 0;
}