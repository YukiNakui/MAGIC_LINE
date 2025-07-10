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
    if (pScriptExecuter != nullptr && !pScriptExecuter->IsEnd())//チュートリアルが終わっていない場合は何もしない
        return;
    else {
		if (countdownNumber_ == nullptr) {
			//カウントダウンがまだ生成されていない場合は生成
			countdownNumber_ = Instantiate<CountdownNumber>(this);
		}
    }


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