#include "ThemeDisplay.h"
#include"../Engine/Image.h"
#include<algorithm>
#include<cmath>

namespace {
    float DEFAULT_DISPLAY_DURATION = 5.0f;    //�\������
    float DEFAULT_TRANSITION_DURATION = 1.0f; //�g�����W�V��������
    float CAMERA_ORBIT_RADIUS = 100.0f;       //�J�����̋O�����a
    float CAMERA_ORBIT_SPEED = 1.0f;          //�J�����̋O�����x
    float CAMERA_ORBIT_HEIGHT = 25.0f;        //�J�����̋O������
    float COUNTDOWN_INIT_TIME = 3.0f;         //�J�E���g�_�E���̏�������
    float MINIMUM_SCALE = 0.3f;               //�X�P�[���̍ŏ��l
    float MAX_SCALE = 1.0f;                   //�X�P�[���̍ő�l
}

ThemeDisplay::ThemeDisplay(GameObject* parent)
	: GameObject(parent, "ThemeDisplay"), hPict_(-1), cdTimer_(nullptr),
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
	//�摜�̃��[�h
    hPict_ = Image::Load("UI/Theme/Theme_Catch.png");
    assert(hPict_ >= 0);

	//�^�C�}�[�̏�����
    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(COUNTDOWN_INIT_TIME);

	//�J�����̋O������I�u�W�F�N�g�̏�����
    pCameraOrbit_ = Instantiate<CameraOrbit>(this);
    pCameraOrbit_->SetRenderOrder(-100);
    pCameraOrbit_->SetOrbit({ 0.0f, CAMERA_ORBIT_HEIGHT, 0.0f }, CAMERA_ORBIT_RADIUS, CAMERA_ORBIT_SPEED);
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

        float smoothT = 1.0f - cos(t * XM_PIDIV2);

        //�ʒu���
        transform_.position_.x = smoothT * targetTrans_.position_.x;
        transform_.position_.y = smoothT * targetTrans_.position_.y;

        //�X�P�[���̕��
        float scale = (1 - smoothT) * MAX_SCALE + smoothT * targetTrans_.scale_.x;
        scale = max(scale, MINIMUM_SCALE);
        transform_.scale_ = { scale, scale, scale };

        //�ŏI�ʒu���Œ�
        if (t >= 1.0f) {
            transform_.position_ = targetTrans_.position_;
            transform_.scale_ = targetTrans_.scale_;
            isMoving_ = false;
            hasMoved_ = true;

            if (pStartCountdownNumber_ == nullptr) {
				pStartCountdownNumber_ = Instantiate<StartCountdownManager>(this);
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

void ThemeDisplay::LoadThemeImage(std::string fileName)
{
	themeImageFileName_ = fileName;
	hPict_ = Image::Load("UI/Theme/" + fileName + ".png");
	if (hPict_ < 0) {
		std::string errorMsg = "Failed to load theme image: " + fileName;
		MessageBox(NULL, errorMsg.c_str(), "Theme Image Load Error", MB_OK);
		assert(false);
	}
}
