#include "CDTimer.h"

const int DEF_TIMER_TIME{ 5 };

CDTimer::CDTimer(GameObject* parent)
    :TIMER_INIT_TIME_(DEF_TIMER_TIME),
    CountDownTimer_(DEF_TIMER_TIME),
    isTimerRun_(true)
{
    oldTime_ = timeGetTime();
    deltaTime_ = 0.0;
};

CDTimer::CDTimer(GameObject* parent, double _initTime)
    :TIMER_INIT_TIME_(_initTime),
    CountDownTimer_(_initTime),
    isTimerRun_(true)
{
    oldTime_ = timeGetTime();
    deltaTime_ = 0.0;
};

bool CDTimer::IsTimeOver()
{
    return(CountDownTimer_ <= 0);
}

void CDTimer::ResetTimer()
{
    CountDownTimer_ = TIMER_INIT_TIME_;
    StartTimer();
}

void CDTimer::StartTimer()
{
    isTimerRun_ = true;
}

void CDTimer::StopTimer()
{
    isTimerRun_ = false;
}

void CDTimer::Initialize()
{
}

void CDTimer::Update()
{
    DWORD nowTime = timeGetTime();                     //現在の時刻を取得
    deltaTime_ = nowTime - oldTime_;                   //フレーム間の経過時間を計算
    if (isTimerRun_)                                   //タイマーが動作中の場合
        CountDownTimer_ = CountDownTimer_ - (float)deltaTime_ / 1000.0; //経過時間をカウントダウンタイマーから減算
    oldTime_ = nowTime;                                //時刻を更新
}

void CDTimer::Draw()
{
}

void CDTimer::Release()
{
}
