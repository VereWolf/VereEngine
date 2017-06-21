#ifndef GAMETIMER_H
#define GAMETIMER_H

#define GAME_TIMER_DLL_EXPORTS
#ifdef GAME_TIMER_DLL_EXPORTS
#define GAME_TIMER_API __declspec( dllexport ) 

#else
#define GAME_TIMER_API __declspec( dllimport )
#endif

class GAME_TIMER_API GameTimer
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

#endif //GAMETIMER_H