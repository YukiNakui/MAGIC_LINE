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
	int stageLength_;//ステージの立幅
	int stageWidth_;//ステージの横幅
	int stageHeight_;//ステージの高さ

	StageBox* pStageBox_;
};

