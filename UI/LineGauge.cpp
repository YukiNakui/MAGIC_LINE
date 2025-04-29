#include "LineGauge.h"
#include"../Engine/Image.h"

namespace {
	float METER_DEFAULT_SCALE = 100.0f;
}

LineGauge::LineGauge(GameObject* parent)
	:GameObject(parent,"LineGauge"), hMeterPict_(-1), hMeterFramePict_(-1),
	meterMaxVal_(0), meterCurrentVal_(0), meterScaleRate_(METER_DEFAULT_SCALE), isDisplay_(false)
{
}

void LineGauge::Initialize()
{
	hMeterPict_ = Image::Load("UI/PlayerUI/Meter.png");
	assert(hMeterPict_ >= 0);
	hMeterFramePict_ = Image::Load("UI/PlayerUI/MeterFrame.png");
	assert(hMeterFramePict_ >= 0);
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
