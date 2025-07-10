#include "StartCountdownManager.h"
#include "../Engine/Audio.h"
#include <cassert>
#include"../ScriptExecuter.h"

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
    ScriptExecuter* pScriptExecuter = (ScriptExecuter*)FindObject("ScriptExecuter");
	if (countdownNumber_ == nullptr && pScriptExecuter == nullptr)
		countdownNumber_ = Instantiate<CountdownNumber>(this);
}

void StartCountdownManager::Update()
{
    ScriptExecuter* pScriptExecuter = (ScriptExecuter*)FindObject("ScriptExecuter");
    if (pScriptExecuter != nullptr && !pScriptExecuter->IsEnd())//�`���[�g���A�����I����Ă��Ȃ��ꍇ�͉������Ȃ�
        return;
    else {
		if (countdownNumber_ == nullptr) {
			//�J�E���g�_�E�����܂���������Ă��Ȃ��ꍇ�͐���
			countdownNumber_ = Instantiate<CountdownNumber>(this);
		}
    }


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