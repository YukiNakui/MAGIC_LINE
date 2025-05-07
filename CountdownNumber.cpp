#include "CountdownNumber.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include <cmath>
#include <cassert>
#include<algorithm>

namespace {
    const float MAX_SCALE = 3.0f;
    const float MIN_SCALE = 1.0f;
    const float COUNTDOWN_INTERVAL = 1.0f;
}

CountdownNumber::CountdownNumber(GameObject* parent)
    : CountdownBase(parent, "CountdownNumber"), currentCount_(COUNT_IMAGES), isCounting_(true), finished_(false)
{
}

CountdownNumber::~CountdownNumber()
{
}

void CountdownNumber::Initialize()
{
    for (int i = 0; i < COUNT_IMAGES; i++) {
        hPictCount_[i] = Image::Load(("UI/CountDown/count_" + std::to_string(i + 1) + ".png").c_str());
        assert(hPictCount_[i] >= 0);
    }

    hCountSound_ = Audio::Load("Sounds/SoundEffect/CountSound.wav");
    assert(hCountSound_ >= 0);

    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(COUNTDOWN_INTERVAL);

    Audio::Play(hCountSound_);
}

void CountdownNumber::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;

    if (elapsedTime_ >= COUNTDOWN_INTERVAL && currentCount_ > 0) {
        elapsedTime_ = 0.0f;
        currentCount_--;

        if (currentCount_ > 0) {
            Audio::Play(hCountSound_);
        }
        else {
            finished_ = true;
        }
    }

    // ÉXÉPÅ[Éãï‚ä‘
    float t = std::clamp(elapsedTime_, 0.0f, COUNTDOWN_INTERVAL);
    float smoothT = 0.5f * (1.0f - cos(t * XM_PI));
    scale_ = MAX_SCALE - smoothT * (MAX_SCALE - MIN_SCALE);
    transform_.scale_ = { scale_, scale_, scale_ };
}

void CountdownNumber::Draw()
{
    if (!isDisplay_ || currentCount_ <= 0) return;

    Image::SetTransform(hPictCount_[currentCount_ - 1], transform_);
    Image::Draw(hPictCount_[currentCount_ - 1]);
}

void CountdownNumber::Release()
{
    CountdownBase::Release();
}