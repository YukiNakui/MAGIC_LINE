#pragma once
#include "../Engine/GameObject.h"
#include "../Functions/CDTimer.h"

class ExplanationScene :
    public GameObject
{
	int hPict_;
	int hSelectSound_;
public:
	ExplanationScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	CDTimer* cdTimer_;//タイマー
};

