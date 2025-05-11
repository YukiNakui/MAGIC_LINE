#pragma once
#include "../Engine/GameObject.h"
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
private:
    float maxHeight_;       // 表示する最大の高さ
    float minHeight_;       // 表示する最小の高さ
    float currentHeight_;   // 現在の高さ

public:
    //高さメーターUIの種類
    enum HeightMeterUIType {
        HEIGHT_METER = 1,    //メーター
        HEIGHT_TRIANGLE      //三角形カーソル
    };

    //メーターのTransformを設定
    void SetHeightMeterUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        meterUITrs_.position_ = position;
        meterUITrs_.rotate_ = rotate;
        meterUITrs_.scale_ = scale;
    }

    //三角形カーソルのTransformを設定
    void SetHeightTriangleUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        triangleUITrs_.position_ = position;
        triangleUITrs_.rotate_ = rotate;
        triangleUITrs_.scale_ = scale;
    }

    //メーターまたは三角形カーソルのTransformを一括で設定
    void SetHeightMeterUITransform(HeightMeterUIType heightMeterUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        switch (heightMeterUIType) {
        case HEIGHT_METER:
            SetHeightMeterUITransform(position, rotate, scale);
            break;
        case HEIGHT_TRIANGLE:
            SetHeightTriangleUITransform(position, rotate, scale);
            break;
        default:
            break;
        }
    }

    //メーターの最大値と最小値を設定
    void SetMaxAndMinHeight(float maxHeight, float minHeight) {
        maxHeight_ = maxHeight;
        minHeight_ = minHeight;
    }

private:
    bool isDisplay_; //高さメーターが表示されているかどうかのフラグ

public:
    //表示状態を設定
    void SetDisplay(bool display) { isDisplay_ = display; }
};

