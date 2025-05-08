#include "StartDisplay.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include <cassert>

namespace {
    const float START_DISPLAY_DURATION = 0.5f; // uSTARTv‚Ì•\Ž¦ŽžŠÔ
}

StartDisplay::StartDisplay(GameObject* parent)
    : CountdownBase(parent, "StartDisplay"), isStartVisible_(true), startElapsedTime_(0.0f)
{
}

StartDisplay::~StartDisplay()
{
}

void StartDisplay::Initialize()
{
    hPictStart_ = Image::Load("UI/CountDown/start.png");
    assert(hPictStart_ >= 0);

    hStartSound_ = Audio::Load("Sounds/SoundEffect/StartSound.wav");
    assert(hStartSound_ >= 0);

    Audio::Play(hStartSound_);

    cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(START_DISPLAY_DURATION);
}

void StartDisplay::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    startElapsedTime_ += deltaTime_;

    if (startElapsedTime_ >= START_DISPLAY_DURATION) {
        isStartVisible_ = false;
    }
}

void StartDisplay::Draw()
{
    if (!isDisplay_ || !isStartVisible_) return;

    Image::SetTransform(hPictStart_, transform_);
    Image::Draw(hPictStart_);
}

void StartDisplay::Release()
{
    CountdownBase::Release();
}