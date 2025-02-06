#include "Meter.h"
#include"Engine/Image.h"

Meter::Meter(GameObject* parent)
	:GameObject(parent,"Meter"), hMeterPict_(-1), hMeterFramePict_(-1),
	meterMaxVal_(0), meterCurrentVal_(0),meterScaleRate_(100)
{
}

void Meter::Initialize()
{
	hMeterPict_ = Image::Load("Meter.png");
	assert(hMeterPict_ >= 0);
	meterTrans_.position_ = { 0.899f,-0.498f,0.0f };
	meterTrans_.scale_ = { 0.3f,0.6f,0.3f };
	meterTrans_.rotate_ = { 0.0f,0.0f,180.0f };
	meterDefalutScaleY_ = meterTrans_.scale_.y;
	hMeterFramePict_ = Image::Load("MeterFrame.png");
	assert(hMeterFramePict_ >= 0);
	meterFrameTrans_.position_ = { 0.9f,0.0f,0.0f };
	meterFrameTrans_.scale_ = { 0.1f,0.2f,0.1f };
}

void Meter::Update()
{
	meterScaleRate_ = 1 - (float)meterCurrentVal_ / (float)meterMaxVal_;
	meterTrans_.scale_.y = meterDefalutScaleY_ * meterScaleRate_;
}

void Meter::Draw()
{
	Image::SetTransform(hMeterFramePict_, meterFrameTrans_);
	Image::Draw(hMeterFramePict_);
	Image::SetTransform(hMeterPict_, meterTrans_);
	Image::Draw(hMeterPict_);
}

void Meter::Release()
{
}
