#pragma once
#include "../Engine/GameObject.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

class CDTimer :
    public GameObject
{
    double CountDownTimer_;      //���݂̃J�E���g�_�E���^�C���i�b�P�ʁj
    double TIMER_INIT_TIME_;     //�^�C�}�[�̏�������
    bool isTimerRun_;            //�^�C�}�[�����쒆���ǂ����̃t���O
    DWORD oldTime_;              //�O��̃^�C���X�^���v�i�~���b�P�ʁj
    DWORD deltaTime_;            //�t���[���Ԃ̌o�ߎ��ԁi�~���b�P�ʁj
public:
    CDTimer(GameObject* parent);
    CDTimer(GameObject* parent, double _initTime);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
public:
    //�^�C�}�[�����Ԑ؂ꂩ�𔻒�
    bool IsTimeOver();

    //�^�C�}�[�����Z�b�g�i�������Ԃɖ߂��j
    void ResetTimer();

    //�^�C�}�[���X�^�[�g
    void StartTimer();

    //�^�C�}�[���X�g�b�v
    void StopTimer();

    //�^�C�}�[�̏������Ԃ�ݒ�
    void SetInitTime(double cdTime) { TIMER_INIT_TIME_ = cdTime; ResetTimer(); }

    //���݂̃^�C�}�[���Ԃ��擾
    double GetTime() { return CountDownTimer_; }

    //�t���[���Ԃ̌o�ߎ��Ԃ��擾
    double GetDeltaTime() { return deltaTime_ / 1000.0; }
};

