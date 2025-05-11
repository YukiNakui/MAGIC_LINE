#include "StartCountdownManager.h"
#include "Engine/Audio.h"
#include <cassert>

StartCountdownManager::StartCountdownManager(GameObject* parent)
    : GameObject(parent, "StartCountdownManager"), countdownNumber_(nullptr), startDisplay_(nullptr),
    isCounting_(true), isStartActive_(false), isFinished_(false)
{
}

StartCountdownManager::~StartCountdownManager()
{
}

void StartCountdownManager::Initialize()
{
	if (countdownNumber_ == nullptr)
		countdownNumber_ = Instantiate<CountdownNumber>(this);
}

void StartCountdownManager::Update()
{
    if (isCounting_) {
        //�J�E���g�_�E�����̏���
        if (countdownNumber_->IsFinished()) {
            //�J�E���g�_�E���I����A�X�^�[�g�\�����J�n
            if (startDisplay_ == nullptr)
                startDisplay_ = Instantiate<StartDisplay>(this);
            isCounting_ = false;
            isStartActive_ = true;
        }
    }

    if (isStartActive_) {
        //�X�^�[�g�\�����̏���
        if (!startDisplay_->IsStartVisible()) {
            //�X�^�[�g�̕\�����I��
            isStartActive_ = false;
            isFinished_ = true; //�S�Ă̏���������
        }
    }
}

void StartCountdownManager::Draw()
{
}

void StartCountdownManager::Release()
{
}