#include "timer.h"
#include"debugmsg.h"
#include "ranking.h"

Timer::Timer()
{
}

void Timer::StartTimer(HDC hdc){
	timewindows = GetTickCount();
	return;
}

void Timer::ShowTime(HDC hdc){
	S_time = (timewindows / 100) / 10.0 - (timestart / 100) / 10.0;					//windows�N��-�v���O�������N��������

	DebugStringFloat("Time:%.1f�b", S_time, hdc, 950, 10, 20);
	return;
}



Timer::~Timer()
{

}

void Timer::WindowsTimer(HDC hdc){

	timestart = GetTickCount();			//windows�N������̎���

	return;
}
