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
        //カウントダウン中の処理
        if (countdownNumber_->IsFinished()) {
            //カウントダウン終了後、スタート表示を開始
            if (startDisplay_ == nullptr)
                startDisplay_ = Instantiate<StartDisplay>(this);
            isCounting_ = false;
            isStartActive_ = true;
        }
    }

    if (isStartActive_) {
        //スタート表示中の処理
        if (!startDisplay_->IsStartVisible()) {
            //スタートの表示が終了
            isStartActive_ = false;
            isFinished_ = true; //全ての処理が完了
        }
    }
}

void StartCountdownManager::Draw()
{
}

void StartCountdownManager::Release()
{
}