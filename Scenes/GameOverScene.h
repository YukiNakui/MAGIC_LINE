#pragma once
#include "../Engine/GameObject.h"
#include "../Functions/CDTimer.h"

class GameOverScene : public GameObject
{
	int hPict_;
	int hSelectSound_;
	int hBGM_;
public:
	GameOverScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	CDTimer* cdTimer_;//�^�C�}�[
};

