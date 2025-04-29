#include "ThemeDisplay.h"
#include"../Engine/Image.h"
#include<algorithm>
#include<cmath>

namespace {
    const float DEFAULT_DISPLAY_DURATION = 5.0f;  // 表示時間
    const float DEFAULT_TRANSITION_DURATION = 1.0f; // トランジション時間
    const float CAMERA_ORBIT_RADIUS = 100.0f;       // カメラの軌道半径
    const float CAMERA_ORBIT_SPEED = 1.0f;          // カメラの軌道速度
    const float CAMERA_ORBIT_HEIGHT = 25.0f;        // カメラの軌道高さ
    const float COUNTDOWN_INIT_TIME = 3.0f;         // カウントダウンの初期時間
    const float MINIMUM_SCALE = 0.3f;               // スケールの最小値
    const float MAX_SCALE = 1.0f;                   // スケールの最大値
}

ThemeDisplay::ThemeDisplay(GameObject* parent)
    : GameObject(parent, "ThemeDisplay"), hPict_(-1), cdTimer_(nullptr), pCountStart_(nullptr),
    deltaTime_(0.0f), elapsedTime_(0.0f), displayDuration_(DEFAULT_DISPLAY_DURATION),
    transitionDuration_(DEFAULT_TRANSITION_DURATION), isMoving_(false), hasMoved_(false),
    pCameraOrbit_(nullptr), isDisplay_(true), isStartVisible_(false)
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
    cdTimer_->SetInitTime(COUNTDOWN_INIT_TIME);

    pCameraOrbit_ = Instantiate<CameraOrbit>(this);
    pCameraOrbit_->SetOrbit({ 0.0f, CAMERA_ORBIT_HEIGHT, 0.0f }, CAMERA_ORBIT_RADIUS, CAMERA_ORBIT_SPEED);
}

void ThemeDisplay::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;  // 累積時間を加算

    if (pCameraOrbit_ != nullptr)
        pCameraOrbit_->Update(deltaTime_);

    if (hasMoved_)
        return;

    // 一定時間経過したら移動を開始
    if (!isMoving_ && elapsedTime_ >= displayDuration_) {
        isMoving_ = true;
        elapsedTime_ = displayDuration_;
    }

    if (isMoving_)
    {
        float t = (elapsedTime_ - displayDuration_) / transitionDuration_;
        t = std::clamp(t, 0.0f, 1.0f);

        float smoothT = 1.0f - cos(t * XM_PIDIV2);

        // 位置補間
        transform_.position_.x = smoothT * targetTrans_.position_.x;
        transform_.position_.y = smoothT * targetTrans_.position_.y;

        // スケールの補間
        float scale = (1 - smoothT) * MAX_SCALE + smoothT * targetTrans_.scale_.x;
        scale = max(scale, MINIMUM_SCALE);
        transform_.scale_ = { scale, scale, scale };

        // 最終位置を固定
        if (t >= 1.0f) {
            transform_.position_ = targetTrans_.position_;
            transform_.scale_ = targetTrans_.scale_;
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