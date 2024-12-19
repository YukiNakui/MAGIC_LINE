#pragma once
#include "Engine/GameObject.h"
#include"StageBox.h"

class Stage :
    public GameObject
{
	int hModel_;
public:
	Stage(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	int stageLength_;//�X�e�[�W�̗���
	int stageWidth_;//�X�e�[�W�̉���
	int stageHeight_;//�X�e�[�W�̍���

	StageBox* pStageBox_;
};

