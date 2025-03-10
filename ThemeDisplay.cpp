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

    // 初期位置
    transform_.position_ = { 0.0f, 0.0f, 0 };  // 画面中央

    // 目標位置を左上に設定
    targetTrans_.position_ = { -0.7f, 0.9f, 0 };  // 左上の座標
}

void ThemeDisplay::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;  // 累積時間を加算

    if (hasMoved_)
        return;

    // 一定時間経過したら移動を開始
    if (!isMoving_ && elapsedTime_ >= displayDuration_) {
        isMoving_ = true;
        elapsedTime_ = displayDuration_;  // 時間を正確に設定
    }

    if (isMoving_)
    {
        float t = (elapsedTime_ - displayDuration_) / transitionDuration_;
        t = std::clamp(t, 0.0f, 1.0f); // tを0.0～1.0に制限

        // t を cos 関数で変換
        float smoothT = 1.0f - cos(t * XM_PI * 0.5f);  // cos 関数を使った加速・減速

        // 位置の補間
        transform_.position_.x = smoothT * targetTrans_.position_.x;
        transform_.position_.y = smoothT * targetTrans_.position_.y;

        // スケールの補間 (0.3 以下にならないようにする)
        float scale = (1 - smoothT) * 1.0f + smoothT * targetScale_;
        scale = max(scale, 0.3f);
        transform_.scale_ = { scale, scale, scale };

        // t が 1.0 に達したら最終位置を固定
        if (t >= 1.0f) {
            transform_.position_ = targetTrans_.position_;
            transform_.scale_ = { targetScale_, targetScale_, targetScale_ };
            isMoving_ = false; // これ以上更新しない
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
