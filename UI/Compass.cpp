#include "Compass.h"
#include"../Engine/Image.h"
#include"../Objects/Player.h"

namespace {
	float RADIUS_CORRECT = 0.3f; // 半径補正値
}

Compass::Compass(GameObject* parent)
	:GameObject(parent, "PlayUI"),
	hCompassFramePict_(-1), hNorthPict_(-1), hSouthPict_(-1), hEastPict_(-1), hWestPict_(-1),
	isDisplay_(false)
{
}

void Compass::Initialize()
{
	hCompassFramePict_ = Image::Load("UI/PlayerUI/CompassFrame.png");
	assert(hCompassFramePict_ >= 0);
	hNorthPict_ = Image::Load("UI/PlayerUI/N.png");
	assert(hNorthPict_ >= 0);
	hSouthPict_ = Image::Load("UI/PlayerUI/S.png");
	assert(hSouthPict_ >= 0);
	hEastPict_ = Image::Load("UI/PlayerUI/E.png");
	assert(hEastPict_ >= 0);
	hWestPict_ = Image::Load("UI/PlayerUI/W.png");
	assert(hWestPict_ >= 0);
}

void Compass::Update()
{
	Player* player = (Player*)FindObject("Player");
	if (!player) return;
	float yawDeg = player->GetRotate().y; // Y軸回転（度数）
	float yawRad = XMConvertToRadians(yawDeg); // ラジアンに変換

	XMVECTOR center = XMLoadFloat3(&compassFrameUITrs_.position_);
	float radius = RADIUS_CORRECT * compassFrameUITrs_.scale_.x; // コンパスサイズに合わせた半径

	// NSEWそれぞれ90°ずらして配置
	XMStoreFloat3(&northUITrs_.position_, CalculatePositionByAngle(yawRad, -XM_PIDIV2, center, radius));  // -90°
	XMStoreFloat3(&eastUITrs_.position_, CalculatePositionByAngle(yawRad, XM_PI, center, radius));       // 180°
	XMStoreFloat3(&southUITrs_.position_, CalculatePositionByAngle(yawRad, XM_PIDIV2, center, radius)); // 90°
	XMStoreFloat3(&westUITrs_.position_, CalculatePositionByAngle(yawRad, 0.0f, center, radius));      // 0°
}

void Compass::Draw()
{
	if (!isDisplay_) return;
	Image::SetTransform(hCompassFramePict_, compassFrameUITrs_);
	Image::Draw(hCompassFramePict_);
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

XMVECTOR Compass::CalculatePositionByAngle(float yawRad, float angleOffsetRad, XMVECTOR center, float radius)
{
	float angle = yawRad + angleOffsetRad;
	float x = radius * cosf(angle);
	float y = radius * sinf(angle);
	return center + XMVectorSet(x, y, 0.0f, 0.0f);
}
