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
	float meterMaxVal_;			//メーターの最大値
	float meterCurrentVal_;		//現在の値
	float meterScaleRate_;		//メーターの大きさの割合
	float meterDefalutScaleY_;	//メーターのY方向のデフォルトの大きさ

public:
	//ライン残量UIの種類
	enum LineGaugeUIType{
		LINEGAUGE_METER = 1,		//メーター
		LINEGAUGE_METERFRAME		//メーターの枠
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

	//メーターまたはメーターの枠のTransformを一括で設定
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

	bool isDisplay_; //メーターが表示されているかどうかのフラグ
public:
	//メーターの最大値と最小値を設定
	void SetDisplay(bool display) { isDisplay_ = display; }
};

