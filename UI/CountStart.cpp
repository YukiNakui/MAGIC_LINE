#include "CountStart.h"
#include"../Engine/Image.h"
#include<algorithm>
#include<cmath>
#include"../Engine/Audio.h"

namespace {
	const float MAX_SCALE = 3.0f; //�J�n���̃X�P�[��
	const float MIN_SCALE = 1.0f; //�ʏ�T�C�Y
    const float COUNTDOWN_INTERVAL = 1.0f; // �J�E���g�_�E���Ԋu�i�b�j
    const float START_DISPLAY_DURATION = 0.5f; // �uSTART�v�̕\�����ԁi�b�j
}

CountStart::CountStart(GameObject* parent)
    : GameObject(parent, "CountStart"), hPictCount_{}, hPictStart_(-1), hCountSound_(-1), hStartSound_(-1), cdTimer_(nullptr),
    deltaTime_(0.0f), currentCount_(0), elapsedTime_(0.0f), isCounting_(false), finished_(false), scale_(MAX_SCALE),
    startElapsedTime_(0.0f), isStartVisible_(true), isDisplay_(true)
{
}

CountStart::~CountStart()
{
}

void CountStart::Initialize()
{
    for (int i = 0; i < COUNT_IMAGES; i++) {
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
    cdTimer_->SetInitTime(COUNTDOWN_INTERVAL);

    currentCount_ = COUNT_IMAGES;
    elapsedTime_ = 0.0f;
    isCounting_ = true;
    finished_ = false;
    scale_ = MAX_SCALE;

    Audio::Play(hCountSound_);
}

void CountStart::Update()
{
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_;

    if (!isCounting_ && !finished_) return; // �J�E���g�I�����Update���p��

    // COUNTDOWN_INTERVAL���ƂɃJ�E���g�_�E��
    if (elapsedTime_ >= COUNTDOWN_INTERVAL && currentCount_ >= 0)
    {
        elapsedTime_ = 0.0f;
        currentCount_--;

        if (currentCount_ <= 0)
        {
            finished_ = true;
            startElapsedTime_ = 0.0f; // �uSTART�v�̕\�����ԃ��Z�b�g
            Audio::Play(hStartSound_);
        }
        else {
            Audio::Play(hCountSound_);
        }

        scale_ = MAX_SCALE;
        cdTimer_->ResetTimer();
    }

    // �uSTART�v�̕\�����ԃJ�E���g
    if (finished_)
    {
        startElapsedTime_ += deltaTime_;

        if (startElapsedTime_ >= START_DISPLAY_DURATION)
        {
            isStartVisible_ = false;
        }
    }
    else // �J�E���g���̂݃X�P�[����Ԃ�K�p
    {
        float t = std::clamp(elapsedTime_, 0.0f, COUNTDOWN_INTERVAL);
        float smoothT = 0.5f * (1.0f - cos(t * XM_PI));
        scale_ = MAX_SCALE - smoothT * (MAX_SCALE - MIN_SCALE);
        transform_.scale_ = { scale_, scale_, scale_ };
    }
}

void CountStart::Draw()
{
    if (!isDisplay_) return;
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
