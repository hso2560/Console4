#include <iostream>
#include <vector>
#include<algorithm>
#include "Console.h"
using namespace std;

int X = 10;
int Y = 10;
string alive = "■";
string death = "□";

class Cell
{
public:

    bool dead = true;
    bool isNextAlive = false;
    int x, y;

    Cell() {}

    Cell(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void printCell()
    {
        gotoXY(x, y);
        if (dead) cout << death;
        else cout << alive;
        //cout << dead ? death : alive;
    }
};


vector<vector<Cell>> cells;

void printCells();

void init();

void checkAlive(int x, int y, int* count);

void next();

bool allDeath();

void create();

int main()
{
    create();
    init();

    while (!allDeath())
    {
        printCells();
        sleep(1);
        clrscr();

        next();
    }

    cout << "-----------전부 사망---------------" << endl;
    sleep(5);
}

void create()
{
    for (int i = 0; i < Y; i++)
    {
        vector<Cell> v;
        for (int j = 0; j < X; j++)
        {
            v.push_back(Cell(j, i));
        }
        cells.push_back(v);
    }
}

void printCells()
{
    for (int i = 0; i < cells.size(); i++) {
        for (int j = 0; j < cells[i].size(); j++)
        {
            cells[i][j].printCell();
        }
    }
}

void init()
{
    cells[4][4].dead = false;
    cells[5][4].dead = false;
    cells[6][4].dead = false;
    cells[5][5].dead = false;
    cells[5][3].dead = false;
}

void next()
{
    int count = 0;
    for (int i = 0; i < cells.size(); i++)
    {
        for (int j = 0; j < cells[i].size(); j++)
        {
            checkAlive(j, i + 1, &count);
            checkAlive(j, i - 1, &count);
            checkAlive(j + 1, i, &count);
            checkAlive(j - 1, i, &count);
            checkAlive(j + 1, i + 1, &count);
            checkAlive(j - 1, i - 1, &count);
            checkAlive(j - 1, i + 1, &count);
            checkAlive(j + 1, i - 1, &count);

            if (count == 3 || (count == 2 && !cells[i][j].dead))
            {
                cells[i][j].isNextAlive = true;
            }
            else
            {
                cells[i][j].isNextAlive = false;
            }

            count = 0;
        }
    }

    for (int i = 0; i < cells.size(); i++)
    {
        /* for (int j = 0; j < cells[i].size(); j++)
         {
             cells[i][j].dead = !cells[i][j].isNextAlive;
             cells[i][j].isNextAlive = false;
         }*/

        for_each(cells[i].begin(), cells[i].end(), [](Cell& c) {
            c.dead = !c.isNextAlive;
            c.isNextAlive = false;
            });
    }
}

void checkAlive(int x, int y, int* count)
{
    if (*count > 3 || x >= X || y >= Y || x < 0 || y < 0) return;

    if (!cells[y][x].dead) (*count)++;
}

bool allDeath()
{
    for (vector<vector<Cell>>::iterator iter = cells.begin(); iter != cells.end(); iter++)
    {
        for (vector<Cell>::iterator it = (*iter).begin(); it != (*iter).end(); it++)
        {
            if (!(*it).dead) return false;
        }
    }
    return true;
}