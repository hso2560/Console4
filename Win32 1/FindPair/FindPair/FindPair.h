#pragma once
#include "framework.h"
#include "resource.h"
#include <memory>

using namespace std;

enum class Status
{
	HIDDEN = 1, //카드 뒤집힌 상태
	FLIP = 2, //카드 열려있음
	TEMPFLIP = 3, //카드가 임시로 열려있음
};

enum class GameStatus
{
	PAUSED = 0,
	RUN = 1,
	HINT = 2,
	VIEW = 3
};

struct CardCell
{
	int _num;
	Status _status;
};
