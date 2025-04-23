#pragma once
#include "../Engine/GameObject.h"
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

	int GetModelHandle() { return hModel_; }
private:
	StageBox* pStageBox_;
};

