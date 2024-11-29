#pragma once
#include "Engine/GameObject.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class CDTimer :
    public GameObject
{
private:
    double CountDownTimer_;  //���݂̎c�莞��
    double TIMER_INIT_TIME_; //��������
    bool isTimerRun_;//�^�C�}�[�������Ă��邩�ǂ����H
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
    bool IsTimeOver();//�^�C�}�[�͂O�ɂȂ�܂������H�x�d�r�H�m�n�H
    void ResetTimer();//�^�C�}�[���������Ԃɖ߂�
    void StartTimer();//�^�C�}�[���X�^�[�g���܂�
    void StopTimer();//�^�C�}�[���X�g�b�v���܂�
    void SetInitTime(double cdTime) { TIMER_INIT_TIME_ = cdTime; ResetTimer(); }
    double GetTime() { return(CountDownTimer_); }
    DWORD GetDeltaTime() { return(deltaTime_); }
};

