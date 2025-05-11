#include "CountdownNumber.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include <cmath>
#include <cassert>
#include<algorithm>

namespace {
    const float MAX_SCALE = 3.0f;             //�J�E���g�\���̍ő�X�P�[��
    const float MIN_SCALE = 1.0f;             //�J�E���g�\���̍ŏ��X�P�[��
    const float COUNTDOWN_INTERVAL = 1.0f;    //�J�E���g�_�E���Ԋu
}

CountdownNumber::CountdownNumber(GameObject* parent)
    : CountdownBase(parent, "CountdownNumber"), currentCount_(COUNT_IMAGES), isCounting_(true), isFinished_(false)
{
}

CountdownNumber::~CountdownNumber()
{
}

void CountdownNumber::Initialize()
{
    //�J�E���g�_�E���p�̉摜�����[�h
    for (int i = 0; i < COUNT_IMAGES; i++) {
        hPictCount_[i] = Image::Load(("UI/CountDown/count_" + std::to_string(i + 1) + ".png").c_str());
        assert(hPictCount_[i] >= 0);
    }

    //�T�E���h�����[�h
    hCountSound_ = Audio::Load("Sounds/SoundEffect/CountSound.wav");
    assert(hCountSound_ >= 0);

    //�^�C�}�[�I�u�W�F�N�g�𐶐����ď������Ԃ�ݒ�
    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(COUNTDOWN_INTERVAL);

    //����̃J�E���g�T�E���h���Đ�
    Audio::Play(hCountSound_);
}

void CountdownNumber::Update()
{
    //�t���[���Ԃ̌o�ߎ��Ԃ��擾
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_; //�ݐόo�ߎ��Ԃ��X�V

    if (elapsedTime_ >= COUNTDOWN_INTERVAL && currentCount_ >= 0) {
        //�J�E���g�_�E����i�s
        elapsedTime_ = 0.0f; //�o�ߎ��Ԃ����Z�b�g
        currentCount_--;     //�J�E���g���f�N�������g

        if (currentCount_ <= 0) {
            //�J�E���g�_�E�����I�������ꍇ
            isFinished_ = true;
        }
        else {
            //�J�E���g�_�E���T�E���h���Đ�
            Audio::Play(hCountSound_);
        }

        //�X�P�[�������Z�b�g���A�^�C�}�[�����Z�b�g
        scale_ = MAX_SCALE;
        cdTimer_->ResetTimer();
    }

    //�X�P�[����ԏ���
    if (isFinished_) return; //�J�E���g�_�E���I�����͕�Ԃ��X�L�b�v
    float t = std::clamp(elapsedTime_, 0.0f, COUNTDOWN_INTERVAL); //�o�ߎ��Ԃ��N�����v
    float smoothT = 0.5f * (1.0f - cos(t * XM_PI));               //�C�[�W���O��K�p
    scale_ = MAX_SCALE - smoothT * (MAX_SCALE - MIN_SCALE);       //�X�P�[������
    transform_.scale_ = { scale_, scale_, scale_ };               //Transform�̃X�P�[�����X�V
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

void CountdownNumber::ResetCountdown()
{
    currentCount_ = COUNT_IMAGES;  //�J�E���g�������l�ɖ߂�
    elapsedTime_ = 0.0f;           //�o�ߎ��Ԃ����Z�b�g
    isCounting_ = true;            //�J�E���g�_�E�����t���O��L����
    isFinished_ = false;           //�I���t���O�𖳌���
    scale_ = MAX_SCALE;            //�X�P�[���������l�ɖ߂�
    Audio::Play(hCountSound_);     //�T�E���h���Đ�
}
