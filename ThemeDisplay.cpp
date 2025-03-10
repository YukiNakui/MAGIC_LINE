#include "ThemeDisplay.h"
#include"Engine/Image.h"
#include<algorithm>
#include<cmath>

ThemeDisplay::ThemeDisplay(GameObject* parent)
	:GameObject(parent,"ThemeDisplay"),hPict_(-1),cdTimer_(nullptr),pCountStart_(nullptr),
    deltaTime_(0.0f),elapsedTime_(0.0f), displayDuration_(2.0f), transitionDuration_(1.0f), isMoving_(false), hasMoved_(false),
    targetScale_(0.3f)
{
}

ThemeDisplay::~ThemeDisplay()
{
}

void ThemeDisplay::Initialize()
{
    hPict_ = Image::Load("Theme_Don'tDrop.png");
    assert(hPict_ >= 0);

    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(3.0f);

    // �����ʒu
    transform_.position_ = { 0.0f, 0.0f, 0 };  // ��ʒ���

    // �ڕW�ʒu������ɐݒ�
    targetTrans_.position_ = { -0.7f, 0.9f, 0 };  // ����̍��W
}

void ThemeDisplay::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;  // �ݐώ��Ԃ����Z

    if (hasMoved_)
        return;

    // ��莞�Ԍo�߂�����ړ����J�n
    if (!isMoving_ && elapsedTime_ >= displayDuration_) {
        isMoving_ = true;
        elapsedTime_ = displayDuration_;  // ���Ԃ𐳊m�ɐݒ�
    }

    if (isMoving_)
    {
        float t = (elapsedTime_ - displayDuration_) / transitionDuration_;
        t = std::clamp(t, 0.0f, 1.0f); // t��0.0�`1.0�ɐ���

        // t �� cos �֐��ŕϊ�
        float smoothT = 1.0f - cos(t * XM_PI * 0.5f);  // cos �֐����g���������E����

        // �ʒu�̕��
        transform_.position_.x = smoothT * targetTrans_.position_.x;
        transform_.position_.y = smoothT * targetTrans_.position_.y;

        // �X�P�[���̕�� (0.3 �ȉ��ɂȂ�Ȃ��悤�ɂ���)
        float scale = (1 - smoothT) * 1.0f + smoothT * targetScale_;
        scale = max(scale, 0.3f);
        transform_.scale_ = { scale, scale, scale };

        // t �� 1.0 �ɒB������ŏI�ʒu���Œ�
        if (t >= 1.0f) {
            transform_.position_ = targetTrans_.position_;
            transform_.scale_ = { targetScale_, targetScale_, targetScale_ };
            isMoving_ = false; // ����ȏ�X�V���Ȃ�
            hasMoved_ = true;

            if (pCountStart_ == nullptr) {
                pCountStart_ = Instantiate<CountStart>(this);
                //countStart_->Initialize();
            }
        }
    }
}

void ThemeDisplay::Draw()
{
    Image::SetTransform(hPict_, transform_);
    Image::Draw(hPict_);
}

void ThemeDisplay::Release()
{
	
}
