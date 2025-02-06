#pragma once
#include "Engine/GameObject.h"

class Meter :
    public GameObject
{
	int hMeterPict_;
	Transform meterTrans_;
	int hMeterFramePict_;
	Transform meterFrameTrans_;
public:
	Meter(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetMeterVal(float _max, float _crr)
	{
		meterMaxVal_ = _max;
		meterCurrentVal_ = _crr;
	}
private:
	float meterMaxVal_;//メーターの最大値
	float meterCurrentVal_;//現在の値
	float meterScaleRate_;//メーターの大きさの割合
	float meterDefalutScaleY_;//メーターのY方向のデフォルトの大きさ
};

