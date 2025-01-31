#pragma once
#include "Engine/GameObject.h"

class Meter :
    public GameObject
{
	int hMeterPict_;
	Transform MeterTrans_;
	int hMeterFramePict_;
	Transform MeterFrameTrans_;
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
	float meterMaxVal_;//���[�^�[�̍ő�l
	float meterCurrentVal_;//���݂̒l
	float currentMeterScaleRate_;//���[�^�[�̑傫���̊���
	float previousMeterScaleRate_;//�ȑO�̃��[�^�[�̑傫���̊���
};

