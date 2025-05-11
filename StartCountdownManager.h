#pragma once
#include "Engine/GameObject.h"
#include "CountdownNumber.h"
#include "StartDisplay.h"

class StartCountdownManager : public GameObject {
public:
    StartCountdownManager(GameObject* parent);
    ~StartCountdownManager();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

	//�J�E���g�_�E�����I���������ǂ���
    bool IsFinished() const { return isFinished_; }

private:
	CountdownNumber* countdownNumber_;//�J�E���g�_�E���\��
	StartDisplay* startDisplay_; //�X�^�[�g�\��
    bool isCounting_;  //���݃J�E���g�_�E�������ǂ���
    bool isStartActive_; //���݃X�^�[�g�\�������ǂ���
    bool isFinished_;  //�S�Ă̏������I���������ǂ���
};


