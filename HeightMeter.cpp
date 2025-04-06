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

	/*meterUITrs_.SetPosition(0, 0);
	meterUITrs_.SetScale(1.0f, 1.0f);
	triangleUITrs_.SetPosition(0, 0);
	triangleUITrs_.SetScale(1.0f, 1.0f);*/
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
