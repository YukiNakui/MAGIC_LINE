#pragma once
#include "Engine/GameObject.h"
class HeightMeter :
    public GameObject
{
	int hMeterPict_;
	int hTrianglePict_;

	Transform meterUITrs_;
	Transform triangleUITrs_;
public:
	HeightMeter(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

