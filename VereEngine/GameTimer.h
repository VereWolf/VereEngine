#pragma once

class GameTimer
{
public:
	GameTimer();
	float TotalTime()const;
	float DeltaTime()const;
	void Reset();
	void Start();
	void Stop();
	void Tick();
	bool GetIsFrame() { return mIsFrame; }
	void SetTimePerFrame(double timePerFrame) { mTimePerFrame = timePerFrame; }
private:
	double  mSecondsPerCount;
	double  mDeltaTime;
	double mTimePerFrame;

	bool mIsFrame;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};