#pragma once
#include "Engine/GameObject.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class CDTimer :
    public GameObject
{
private:
    double CountDownTimer_;  //現在の残り時間
    double TIMER_INIT_TIME_; //初期時間
    bool isTimerRun_;//タイマーが動いているかどうか？
    DWORD oldTime_;
    DWORD deltaTime_;
public:
    CDTimer(GameObject* parent);
    CDTimer(GameObject* parent, double _initTime);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
public:
    bool IsTimeOver();//タイマーは０になりましたか？ＹＥＳ？ＮＯ？
    void ResetTimer();//タイマーを初期時間に戻す
    void StartTimer();//タイマーをスタートします
    void StopTimer();//タイマーをストップします
    void SetInitTime(double cdTime) { TIMER_INIT_TIME_ = cdTime; ResetTimer(); }
    double GetTime() { return(CountDownTimer_); }
    DWORD GetDeltaTime() { return(deltaTime_); }
};

