#include "Meter.h"
#include"Engine/Image.h"

Meter::Meter(GameObject* parent)
	:GameObject(parent,"Meter"), hMeterPict_(-1), hMeterFramePict_(-1),
	meterMaxVal_(0), meterCurrentVal_(0),currentMeterScaleRate_(100), previousMeterScaleRate_(100)
{
}

void Meter::Initialize()
{
	hMeterPict_ = Image::Load("Meter.png");
	assert(hMeterPict_ >= 0);
	MeterTrans_.position_ = { 0.901f,0.504f,0.0f };
	MeterTrans_.scale_ = { 0.3f,0.61f,0.3f };
	hMeterFramePict_ = Image::Load("MeterFrame.png");
	assert(hMeterFramePict_ >= 0);
	MeterFrameTrans_.position_ = { 0.9f,0.0f,0.0f };
	MeterFrameTrans_.scale_ = { 0.1f,0.2f,0.1f };
}

void Meter::Update()
{
	currentMeterScaleRate_ = (float)meterCurrentVal_ / (float)meterMaxVal_;

	if (previousMeterScaleRate_ != currentMeterScaleRate_) {
		MeterTrans_.scale_.y *= currentMeterScaleRate_;
		previousMeterScaleRate_ = currentMeterScaleRate_;
	}
}

void Meter::Draw()
{
	Image::SetTransform(hMeterFramePict_, MeterFrameTrans_);
	Image::Draw(hMeterFramePict_);
	Image::SetTransform(hMeterPict_, MeterTrans_);
	Image::Draw(hMeterPict_);
}

void Meter::Release()
{
}
