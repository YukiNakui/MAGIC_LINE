#include "HeightMeter.h"
#include"Engine/Image.h"

HeightMeter::HeightMeter(GameObject* parent)
	:GameObject(parent, "HeightMeter"), hMeterPict_(-1), hTrianglePict_(-1)
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
}

void HeightMeter::Update()
{
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
