#include "LineGauge.h"
#include"../Engine/Image.h"

LineGauge::LineGauge(GameObject* parent)
	:GameObject(parent,"LineGauge"), hMeterPict_(-1), hMeterFramePict_(-1),
	meterMaxVal_(0), meterCurrentVal_(0), meterScaleRate_(100), isDisplay_(false)
{
}

void LineGauge::Initialize()
{
	hMeterPict_ = Image::Load("UI/PlayerUI/Meter.png");
	assert(hMeterPict_ >= 0);
	meterTrans_.position_ = { 0.149f,-0.798f,0.0f };
	meterTrans_.scale_ = { 0.3f,0.3f,0.3f };
	meterTrans_.rotate_ = { 0.0f,0.0f,180.0f };
	meterDefalutScaleY_ = meterTrans_.scale_.y;
	hMeterFramePict_ = Image::Load("UI/PlayerUI/MeterFrame.png");
	assert(hMeterFramePict_ >= 0);
	meterFrameTrans_.position_ = { 0.15f,-0.55f,0.0f };
	meterFrameTrans_.scale_ = { 0.1f,0.1f,0.1f };
}

void LineGauge::Update()
{
	meterScaleRate_ = 1 - (float)meterCurrentVal_ / (float)meterMaxVal_;
	meterTrans_.scale_.y = meterDefalutScaleY_ * meterScaleRate_;
}

void LineGauge::Draw()
{
	if (!isDisplay_) return;
	Image::SetTransform(hMeterFramePict_, meterFrameTrans_);
	Image::Draw(hMeterFramePict_);
	Image::SetTransform(hMeterPict_, meterTrans_);
	Image::Draw(hMeterPict_);
}

void LineGauge::Release()
{
}
