#pragma once
#include "../Engine/GameObject.h"
class Compass :
    public GameObject
{
	int hCompassFramePict_;
	int hNorthPict_;
	int hSouthPict_;
	int hEastPict_;
	int hWestPict_;

	Transform compassFrameUITrs_;
	Transform northUITrs_;
	Transform southUITrs_;
	Transform eastUITrs_;
	Transform westUITrs_;
public:
	Compass(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//コンパスUIの種類
	enum CompassUIType {
		COMPASS_FRAME = 1, //コンパスのフレーム
		NORTH,             //北のUI
		SOUTH,             //南のUI
		EAST,              //東のUI
		WEST               //西のUI
	};

	void SetCompassFrameUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		compassFrameUITrs_.position_ = position;
		compassFrameUITrs_.rotate_ = rotate;
		compassFrameUITrs_.scale_ = scale;
	}
	void SetNorthUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		northUITrs_.position_ = position;
		northUITrs_.rotate_ = rotate;
		northUITrs_.scale_ = scale;
	}
	void SetSouthUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		southUITrs_.position_ = position;
		southUITrs_.rotate_ = rotate;
		southUITrs_.scale_ = scale;
	}
	void SetEastUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		eastUITrs_.position_ = position;
		eastUITrs_.rotate_ = rotate;
		eastUITrs_.scale_ = scale;
	}
	void SetWestUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		westUITrs_.position_ = position;
		westUITrs_.rotate_ = rotate;
		westUITrs_.scale_ = scale;
	}

	//コンパスUIのTransformを一括で設定
	void SetCompassUITransform(CompassUIType compassUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (compassUIType) {
		case COMPASS_FRAME:
			SetCompassFrameUITransform(position, rotate, scale);
			break;
		case NORTH:
			SetNorthUITransform(position, rotate, scale);
			break;
		case SOUTH:
			SetSouthUITransform(position, rotate, scale);
			break;
		case EAST:
			SetEastUITransform(position, rotate, scale);
			break;
		case WEST:
			SetWestUITransform(position, rotate, scale);
			break;
		default:
			break;
		}
	}

private:
	//矢印の角度計算
	XMVECTOR CalculatePositionByAngle(float yawRad, float angleOffsetRad, XMVECTOR center, float radius);

	//コンパスの表示状態
	bool isDisplay_;

public:
	//コンパスの表示状態を設定
	void SetDisplay(bool display) { isDisplay_ = display; }
};

