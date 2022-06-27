#pragma once
#include "framework.h"
#include "resource.h"
#include "MoveInfo.h"

#define MAX_STAGE 3
#define BW 32
#define BH 32

#include <vector>
using namespace std;

class Game
{
public:
   
    char arStage[MAX_STAGE][18][21] = {
    {
    "####################",
    "####################",
    "####################",
    "#####   ############",
    "#####O  ############",
    "#####  O############",
    "###  O O ###########",
    "### # ## ###########",
    "#   # ## #####  ..##",
    "# O  O   @      ..##",
    "##### ### # ##  ..##",
    "#####     ##########",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "####..  #     ######",
    "####..  # O  O  ####",
    "####..  #O####  ####",
    "####..    @ ##  ####",
    "####..  # #  O #####",
    "######### ##O O ####",
    "###### O  O O O ####",
    "######    #     ####",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    {
    "####################",
    "####################",
    "####################",
    "####################",
    "##########     @####",
    "########## O#O #####",
    "########## O  O#####",
    "###########O O #####",
    "########## O # #####",
    "##....  ## O  O  ###",
    "###...    O  O   ###",
    "##....  ############",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################",
    "####################"
    },
    };

    vector<MoveInfo>* undoList;
    int undoListIndex = -1;

    int pDir = 0;

    HWND hWndMain;
    Game(HWND main, HINSTANCE hInst);
    ~Game();

    void DrawScreen(HDC hdc, PAINTSTRUCT& ps);
    BOOL TestEnd();
    void Move(int dir);
    void InitStage();
    void DrawBitmap(HDC hdc,int idx ,int x, int y, HBITMAP bBitSheet);

    BOOL GotoNextStage();
    BOOL GotoPrevStage();
    void ErasePack(int x, int y);

    BOOL Undo();
    BOOL Redo();

    void UndoMove(int dx, int dy, bool bWithPack, int dir);
    void RedoMove(int dx, int dy, bool bWithPack, int dir);

    void TestClear();

    int GetCurrentStage(); 
   
    void Ending();

private :
    char ns[18][21] = { 0, };
    int nStage = 0;
    int nx = 0, ny = 0;
    int nMove = 0;
    HBITMAP hBitSheet = nullptr;
};

