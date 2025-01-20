#pragma once
#include "Engine/GameObject.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class CDTimer :
    public GameObject
{
private:
    double CountDownTimer_;
    double TIMER_INIT_TIME_;
    bool isTimerRun_;
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
    bool IsTimeOver();
    void ResetTimer();
    void StartTimer();
    void StopTimer();
    void SetInitTime(double cdTime) { TIMER_INIT_TIME_ = cdTime; ResetTimer(); }
    double GetTime() { return(CountDownTimer_); }
    double GetDeltaTime() { return(deltaTime_ / 1000.0); }
};

