#pragma warning(disable: 4996)
#include "DxLib.h"
#include "sub.h"

static int mStartTime;
static int mCount;
static double mFps;
static double ave;
static const int N = 60;
static const int FPS = 60;

bool FPSUpdate()
{
	if (mCount == 0)
	{
		mStartTime = GetNowCount();
	}
	if (mCount == N)
	{
		int t = GetNowCount();
		ave = (t - (double)mStartTime) / (double)N;
		if (ave != 0)
		{
			mFps = 1000.0 / ((t - (double)mStartTime) / (double)N);
		}
		else mFps = 0;
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void FPSDraw()
{
	DrawFormatString(0, FONTSIZE * 0, GetColor(255, 255, 255), "%.1f FPS", mFps);
	DrawFormatString(0, FONTSIZE * 1, GetColor(255, 255, 255), "%.1f ms", ave);
}

void FPSWait()
{
	int tookTime = GetNowCount() - mStartTime;
	int waitTime = mCount * 1000 / FPS - tookTime;
	if (waitTime > 0)
	{
		Sleep(waitTime);
	}
}