#pragma once
#include "../Engine/GameObject.h"

class PlayUI :
    public GameObject
{
    int hLinePict_;
    int hDeletePict_;
    int hFinishPict_;
	Transform lineUITrs_;
	Transform deleteUITrs_;
	Transform finishUITrs_;
public:
	PlayUI(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

