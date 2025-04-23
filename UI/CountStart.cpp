#include "CountStart.h"
#include"../Engine/Image.h"
#include<algorithm>
#include<cmath>
#include"../Engine/Audio.h"

CountStart::CountStart(GameObject* parent)
    :GameObject(parent, "CountStart"), hPictCount_{}, hPictStart_(-1), hCountSound_(-1), hStartSound_(-1), cdTimer_(nullptr),
	deltaTime_(0.0f),currentCount_(0), elapsedTime_(0.0f), isCounting_(false), finished_(false), scale_(maxScale_),
    startElapsedTime_(0.0f),isStartVisible_(true)
{
}

CountStart::~CountStart()
{
}

void CountStart::Initialize()
{
    for (int i = 0; i < 3; i++) {
        hPictCount_[i] = Image::Load(("UI/CountDown/count_" + std::to_string(i + 1) + ".png").c_str());
        assert(hPictCount_[i] >= 0);
    }
	hPictStart_ = Image::Load("UI/CountDown/start.png");
	assert(hPictStart_ >= 0);

    hCountSound_ = Audio::Load("Sounds/SoundEffect/CountSound.wav");
    assert(hCountSound_ >= 0);
    hStartSound_ = Audio::Load("Sounds/SoundEffect/StartSound.wav");
    assert(hStartSound_ >= 0);

    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(1.0f);

	currentCount_ = 3;
	elapsedTime_ = 0.0f;
	isCounting_ = true;
	finished_ = false;
	scale_ = maxScale_;

    Audio::Play(hCountSound_);
}

void CountStart::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;

    if (!isCounting_ && !finished_) return; //カウント終了後もUpdateを継続

    //1秒ごとにカウントダウン
    if (elapsedTime_ >= 1.0f && currentCount_ >= 0)
    {
        elapsedTime_ = 0.0f;
        currentCount_--;

        if (currentCount_ <= 0)
        {
            finished_ = true;
            startElapsedTime_ = 0.0f; //「START」の表示時間リセット
            Audio::Play(hStartSound_);
        }
        else {
            Audio::Play(hCountSound_);
        }

        scale_ = maxScale_;
        cdTimer_->ResetTimer();
    }

    //「START」の表示時間カウント
    if (finished_)
    {
        startElapsedTime_ += deltaTime_;

        if (startElapsedTime_ >= 0.5f)
        {
            isStartVisible_ = false;
        }
    }
    else //カウント中のみスケール補間を適用
    {
        float t = std::clamp(elapsedTime_, 0.0f, 1.0f);
        float smoothT = 0.5f * (1.0f - cos(t * XM_PI));
        scale_ = maxScale_ - smoothT * (maxScale_ - minScale_);
        transform_.scale_ = { scale_, scale_, scale_ };
    }
}

void CountStart::Draw()
{
    if (currentCount_ > 0) {
        Image::SetTransform(hPictCount_[currentCount_ - 1], transform_);
        Image::Draw(hPictCount_[currentCount_ - 1]);
    }
    else if (isStartVisible_)
    {
        Image::SetTransform(hPictStart_, transform_);
        Image::Draw(hPictStart_);
    }
}

void CountStart::Release()
{
}
