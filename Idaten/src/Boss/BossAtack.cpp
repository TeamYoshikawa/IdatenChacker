#include "BossAtack.h"
#include<iostream>

BossAtack::BossAtack(int x,int y)
{
	atack.x = x;
	atack.y = y;

	atack_hb = (HBITMAP)LoadImage(NULL, TEXT("res/BOSS/thender_h.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	init();
}

BossAtack::~BossAtack()
{
}

int BossAtack::init()
{
	atack.width = 50;
	atack.height = 120;

	atack.onActive = 1;
	return 0;
}

int BossAtack::move()
{
	hit_atackcheck();
	return 0;
}