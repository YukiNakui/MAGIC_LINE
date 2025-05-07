#include "StartCountdownManager.h"
#include "Engine/Audio.h"
#include <cassert>

StartCountdownManager::StartCountdownManager(GameObject* parent)
    : GameObject(parent, "StartCountdownManager"), countdownNumber_(nullptr), startDisplay_(nullptr),
    isCounting_(false), isStartActive_(false), isFinished_(false)
{
}

StartCountdownManager::~StartCountdownManager()
{
}

void StartCountdownManager::Initialize()
{
    countdownNumber_ = Instantiate<CountdownNumber>(this);
    startDisplay_ = Instantiate<StartDisplay>(this);

    countdownNumber_->Initialize();
    startDisplay_->Initialize();

    countdownNumber_->SetDisplay(false); // ������Ԃł͔�\��
    startDisplay_->SetDisplay(false);   // ������Ԃł͔�\��
}

void StartCountdownManager::Update()
{
    if (isCounting_) {
        // �J�E���g�_�E�����̏���
        countdownNumber_->Update();

        if (countdownNumber_->IsFinished()) {
            // �J�E���g�_�E���I����A�X�^�[�g�\�����J�n
            isCounting_ = false;
            isStartActive_ = true;
            countdownNumber_->SetDisplay(false); // �J�E���g�_�E����\��
            startDisplay_->SetDisplay(true);    // �X�^�[�g�\����L����
        }
    }

    if (isStartActive_) {
        // �X�^�[�g�\�����̏���
        startDisplay_->Update();

        if (!startDisplay_->IsStartVisible()) {
            // �X�^�[�g�̕\�����I��
            isStartActive_ = false;
            isFinished_ = true; // �S�Ă̏���������
        }
    }
}

void StartCountdownManager::Draw()
{
    if (isCounting_) {
        countdownNumber_->Draw();
    }

    if (isStartActive_) {
        startDisplay_->Draw();
    }
}

void StartCountdownManager::Release()
{
}

void StartCountdownManager::StartCountdown()
{
    // �J�E���g�_�E�����J�n
    isCounting_ = true;
    isStartActive_ = false;
    isFinished_ = false;

    countdownNumber_->SetDisplay(true); // �J�E���g�_�E����\��
    startDisplay_->SetDisplay(false);  // �X�^�[�g���\��
    countdownNumber_->Initialize();    // �J�E���g�_�E�����ď�����
}