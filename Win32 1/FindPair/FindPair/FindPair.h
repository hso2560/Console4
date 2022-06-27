#pragma once
#include "framework.h"
#include "resource.h"
#include <memory>

using namespace std;

enum class Status
{
	HIDDEN = 1, //ī�� ������ ����
	FLIP = 2, //ī�� ��������
	TEMPFLIP = 3, //ī�尡 �ӽ÷� ��������
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
