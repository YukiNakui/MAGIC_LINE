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
	float meterMaxVal_;//���[�^�[�̍ő�l
	float meterCurrentVal_;//���݂̒l
	float meterScaleRate_;//���[�^�[�̑傫���̊���
	float meterDefalutScaleY_;//���[�^�[��Y�����̃f�t�H���g�̑傫��
};

