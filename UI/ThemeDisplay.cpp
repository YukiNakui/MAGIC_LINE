#include "ThemeDisplay.h"
#include"../Engine/Image.h"
#include<algorithm>
#include<cmath>

ThemeDisplay::ThemeDisplay(GameObject* parent)
	:GameObject(parent,"ThemeDisplay"),hPict_(-1),cdTimer_(nullptr),pCountStart_(nullptr),
    deltaTime_(0.0f),elapsedTime_(0.0f), displayDuration_(5.0f), transitionDuration_(1.0f), isMoving_(false), hasMoved_(false),
	targetScale_(0.3f), pCameraOrbit_(nullptr), isDisplay_(true), isStartVisible_(false)
{
}

ThemeDisplay::~ThemeDisplay()
{
}

void ThemeDisplay::Initialize()
{
    hPict_ = Image::Load("UI/Theme/Theme_Catch.png");
    assert(hPict_ >= 0);

    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(3.0f);

    transform_.position_ = { 0.0f, 0.0f, 0 };
    targetTrans_.position_ = { -0.7f, 0.9f, 0 };

    pCameraOrbit_ = Instantiate<CameraOrbit>(this);
	pCameraOrbit_->SetOrbit({ 0.0f, 25.0f, 0.0f }, 100.0f, 1.0f);
}

void ThemeDisplay::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;  //�ݐώ��Ԃ����Z

    if (pCameraOrbit_ != nullptr)
        pCameraOrbit_->Update(deltaTime_);

    if (hasMoved_)
        return;

    //��莞�Ԍo�߂�����ړ����J�n
    if (!isMoving_ && elapsedTime_ >= displayDuration_) {
        isMoving_ = true;
        elapsedTime_ = displayDuration_;
    }

    if (isMoving_)
    {
        float t = (elapsedTime_ - displayDuration_) / transitionDuration_;
        t = std::clamp(t, 0.0f, 1.0f);

        float smoothT = 1.0f - cos(t * XM_PI * 0.5f);

        //�ʒu���
        transform_.position_.x = smoothT * targetTrans_.position_.x;
        transform_.position_.y = smoothT * targetTrans_.position_.y;

        //�X�P�[���̕��
        float scale = (1 - smoothT) * 1.0f + smoothT * targetScale_;
        scale = max(scale, 0.3f);
        transform_.scale_ = { scale, scale, scale };

        //�ŏI�ʒu���Œ�
        if (t >= 1.0f) {
            transform_.position_ = targetTrans_.position_;
            transform_.scale_ = { targetScale_, targetScale_, targetScale_ };
            isMoving_ = false;
            hasMoved_ = true;

            if (pCountStart_ == nullptr) {
                pCountStart_ = Instantiate<CountStart>(this);
                isStartVisible_ = true;
                if (pCameraOrbit_->IsStopOrbit()) {
                    pCameraOrbit_->StopOrbit();
                    pCameraOrbit_ = nullptr;
                }
            }
        }
    }
}

void ThemeDisplay::Draw()
{
	if (!isDisplay_) return;
    Image::SetTransform(hPict_, transform_);
    Image::Draw(hPict_);
}

void ThemeDisplay::Release()
{
}
