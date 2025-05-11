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

	//�R���p�XUI�̎��
	enum CompassUIType {
		COMPASS_FRAME = 1, //�R���p�X�̃t���[��
		NORTH,             //�k��UI
		SOUTH,             //���UI
		EAST,              //����UI
		WEST               //����UI
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

	//�R���p�XUI��Transform���ꊇ�Őݒ�
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
	//���̊p�x�v�Z
	XMVECTOR CalculatePositionByAngle(float yawRad, float angleOffsetRad, XMVECTOR center, float radius);

	//�R���p�X�̕\�����
	bool isDisplay_;

public:
	//�R���p�X�̕\����Ԃ�ݒ�
	void SetDisplay(bool display) { isDisplay_ = display; }
};

