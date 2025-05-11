#pragma once
#include "../Engine/GameObject.h"

class LineGauge :
    public GameObject
{
	int hMeterPict_;
	Transform meterTrans_;
	int hMeterFramePict_;
	Transform meterFrameTrans_;
public:
	LineGauge(GameObject* parent);
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
	float meterMaxVal_;			//���[�^�[�̍ő�l
	float meterCurrentVal_;		//���݂̒l
	float meterScaleRate_;		//���[�^�[�̑傫���̊���
	float meterDefalutScaleY_;	//���[�^�[��Y�����̃f�t�H���g�̑傫��

public:
	//���C���c��UI�̎��
	enum LineGaugeUIType{
		LINEGAUGE_METER = 1,		//���[�^�[
		LINEGAUGE_METERFRAME		//���[�^�[�̘g
	};

	void SetLineGaugeMeterUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		meterTrans_.position_ = position;
		meterTrans_.rotate_ = rotate;
		meterTrans_.scale_ = scale;
	}

	void SetLineGaugeMeterFrameUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		meterFrameTrans_.position_ = position;
		meterFrameTrans_.rotate_ = rotate;
		meterFrameTrans_.scale_ = scale;
	}

	//���[�^�[�܂��̓��[�^�[�̘g��Transform���ꊇ�Őݒ�
	void SetLineGaugeUITransform(LineGaugeUIType lineGaugeUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (lineGaugeUIType) {
		case LINEGAUGE_METER:
			SetLineGaugeMeterUITransform(position, rotate, scale);
			meterDefalutScaleY_ = meterTrans_.scale_.y;
			break;
		case LINEGAUGE_METERFRAME:
			SetLineGaugeMeterFrameUITransform(position, rotate, scale);
			break;
		default:
			break;
		}
	}

	bool isDisplay_; //���[�^�[���\������Ă��邩�ǂ����̃t���O
public:
	//���[�^�[�̍ő�l�ƍŏ��l��ݒ�
	void SetDisplay(bool display) { isDisplay_ = display; }
};

