#include "Compass.h"
#include"../Engine/Image.h"
#include"../Objects/Player.h"

Compass::Compass(GameObject* parent)
	:GameObject(parent, "PlayUI"), hCirclePict_(-1), hNorthPict_(-1), hSouthPict_(-1), hEastPict_(-1), hWestPict_(-1),
	northVec_(XMVectorSet(0.0f, -0.22f, 0.0f, 0.0f)), southVec_(XMVectorSet(0.0f, 0.22f, 0.0f, 0.0f)), 
	eastVec_(XMVectorSet(-0.12f, 0.0f, 0.0f, 0.0f)), westVec_(XMVectorSet(0.12f, 0.0f, 0.0f, 0.0f))
{
}

void Compass::Initialize()
{
	hCirclePict_ = Image::Load("UI/PlayerUI/CompassFrame.png");
	assert(hCirclePict_ >= 0);
	hNorthPict_ = Image::Load("UI/PlayerUI/N.png");
	assert(hNorthPict_ >= 0);
	hSouthPict_ = Image::Load("UI/PlayerUI/S.png");
	assert(hSouthPict_ >= 0);
	hEastPict_ = Image::Load("UI/PlayerUI/E.png");
	assert(hEastPict_ >= 0);
	hWestPict_ = Image::Load("UI/PlayerUI/W.png");
	assert(hWestPict_ >= 0);

	circleUITrs_.position_ = { 0.0f, 1.0f, 0.0f };
	circleUITrs_.scale_ = { 0.8f, 0.45f, 0.5f };
	circleUITrs_.rotate_ = { 00.0f, 0.0f, 180.0f };
	XMVECTOR circleVec_;
	circleVec_ = XMLoadFloat3(&circleUITrs_.position_);
	XMFLOAT3 northPos, southPos, eastPos, westPos;
	XMStoreFloat3(&northPos, circleVec_ + northVec_);
	XMStoreFloat3(&southPos, circleVec_ + southVec_);
	XMStoreFloat3(&eastPos, circleVec_ + eastVec_);
	XMStoreFloat3(&westPos, circleVec_ + westVec_);
	northUITrs_.position_ = northPos;
	southUITrs_.position_ = southPos;
	eastUITrs_.position_ = eastPos;
	westUITrs_.position_ = westPos;
	northUITrs_.scale_ = { 0.3f, 0.3f, 0.3f };
	southUITrs_.scale_ = { 0.3f, 0.3f, 0.3f };
	eastUITrs_.scale_ = { 0.3f, 0.3f, 0.3f };
	westUITrs_.scale_ = { 0.3f, 0.3f, 0.3f };
}

void Compass::Update()
{
	Player* player = (Player*)FindObject("Player");
	if (!player) return;
	float yawDeg = player->GetRotate().y; // Y����]�i�x���j
	float yawRad = XMConvertToRadians(yawDeg); // ���W�A���ɕϊ�

	XMVECTOR center = XMLoadFloat3(&circleUITrs_.position_);
	float radius = 0.3f * circleUITrs_.scale_.x; // �R���p�X�T�C�Y�ɍ��킹�����a

	auto GetPositionByAngle = [&](float angleOffsetRad) {
		float angle = yawRad + angleOffsetRad;
		float x = radius * cosf(angle);
		float y = radius * sinf(angle);
		return center + XMVectorSet(x, y, 0.0f, 0.0f);
	};

	// NSEW���ꂼ��90�����炵�Ĕz�u
	XMStoreFloat3(&northUITrs_.position_, GetPositionByAngle(-XM_PIDIV2));  // -90��
	XMStoreFloat3(&eastUITrs_.position_, GetPositionByAngle(XM_PI));       // 180��
	XMStoreFloat3(&southUITrs_.position_, GetPositionByAngle(XM_PIDIV2)); // 90��
	XMStoreFloat3(&westUITrs_.position_, GetPositionByAngle(0.0f));      // 0��
}

void Compass::Draw()
{
	Image::SetTransform(hCirclePict_, circleUITrs_);
	Image::Draw(hCirclePict_);
	Image::SetTransform(hNorthPict_, northUITrs_);
	Image::Draw(hNorthPict_);
	Image::SetTransform(hSouthPict_, southUITrs_);
	Image::Draw(hSouthPict_);
	Image::SetTransform(hEastPict_, eastUITrs_);
	Image::Draw(hEastPict_);
	Image::SetTransform(hWestPict_, westUITrs_);
	Image::Draw(hWestPict_);
}

void Compass::Release()
{
}
