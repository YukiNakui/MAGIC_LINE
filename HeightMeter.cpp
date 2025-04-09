#include "HeightMeter.h"
#include"Engine/Image.h"
#include"Player.h"

HeightMeter::HeightMeter(GameObject* parent)
	:GameObject(parent, "HeightMeter"), hMeterPict_(-1), hTrianglePict_(-1),
	maxHeight_(0.0f), minHeight_(0.0f), currentHeight_(0.0f)
{
}

void HeightMeter::Initialize()
{
	hMeterPict_ = Image::Load("HeightMeter.png");
	assert(hMeterPict_ >= 0);
	hTrianglePict_ = Image::Load("Triangle.png");
	assert(hTrianglePict_ >= 0);

	meterUITrs_.position_ = { 0.9f,0.0f,0.0f };
	meterUITrs_.scale_ = { 0.4f,0.6f,0.6f };
	triangleUITrs_.position_ = { 0.75f,0.0f,0.0f };
	triangleUITrs_.scale_ = { 0.4f,0.4f,0.4f };

	maxHeight_ = 50.0f;
	minHeight_ = 0.0f;
}

void HeightMeter::Update()
{
	Player* player = (Player*)FindObject("Player");
	if(!player) return;
	currentHeight_ = player->GetPosition().y;
	triangleUITrs_.position_.y = (currentHeight_ - (maxHeight_ - minHeight_)) * 0.02 + 0.47f;
}

void HeightMeter::Draw()
{
	Image::SetTransform(hMeterPict_, meterUITrs_);
	Image::Draw(hMeterPict_);
	Image::SetTransform(hTrianglePict_, triangleUITrs_);
	Image::Draw(hTrianglePict_);
}

void HeightMeter::Release()
{
}
