#pragma once
class MoveInfo
{
public:
	char dx : 3;  //비트 필드
	char dy : 3;
	char bWithPack : 2;

	char pDir : 3;
	MoveInfo(int dx, int dy, bool bWithPack, int pDir)
	{
		this->dx = dx;
		this->dy = dy;
		this->bWithPack = bWithPack;
		this->pDir = pDir;
	}
};

