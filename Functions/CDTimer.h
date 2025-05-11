#pragma once
#include "../Engine/GameObject.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class CDTimer :
    public GameObject
{
    double CountDownTimer_;      //現在のカウントダウンタイム（秒単位）
    double TIMER_INIT_TIME_;     //タイマーの初期時間
    bool isTimerRun_;            //タイマーが動作中かどうかのフラグ
    DWORD oldTime_;              //前回のタイムスタンプ（ミリ秒単位）
    DWORD deltaTime_;            //フレーム間の経過時間（ミリ秒単位）
public:
    CDTimer(GameObject* parent);
    CDTimer(GameObject* parent, double _initTime);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
public:
    //タイマーが時間切れかを判定
    bool IsTimeOver();

    //タイマーをリセット（初期時間に戻す）
    void ResetTimer();

    //タイマーをスタート
    void StartTimer();

    //タイマーをストップ
    void StopTimer();

    //タイマーの初期時間を設定
    void SetInitTime(double cdTime) { TIMER_INIT_TIME_ = cdTime; ResetTimer(); }

    //現在のタイマー時間を取得
    double GetTime() { return CountDownTimer_; }

    //フレーム間の経過時間を取得
    double GetDeltaTime() { return deltaTime_ / 1000.0; }
};

