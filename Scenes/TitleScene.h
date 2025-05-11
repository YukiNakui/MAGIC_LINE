#pragma once
#include "../Engine/GameObject.h"
#include "../Functions/CDTimer.h"

class TitleScene : public GameObject
{
	int hPict_;
	int hSelectSound_;
	int hBGM_;
public:
	TitleScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	CDTimer* cdTimer_;//タイマー
};

