#pragma once
#include "Engine/GameObject.h"
class Compass :
    public GameObject
{
	int hCirclePict_;
	int hNorthPict_;
	int hSouthPict_;
	int hEastPict_;
	int hWestPict_;

	Transform circleUITrs_;
	Transform northUITrs_;
	Transform southUITrs_;
	Transform eastUITrs_;
	Transform westUITrs_;
	XMVECTOR northVec_;
	XMVECTOR southVec_;
	XMVECTOR eastVec_;
	XMVECTOR westVec_;
public:
	Compass(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

