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

    countdownNumber_->SetDisplay(false); // 初期状態では非表示
    startDisplay_->SetDisplay(false);   // 初期状態では非表示
}

void StartCountdownManager::Update()
{
    if (isCounting_) {
        // カウントダウン中の処理
        countdownNumber_->Update();

        if (countdownNumber_->IsFinished()) {
            // カウントダウン終了後、スタート表示を開始
            isCounting_ = false;
            isStartActive_ = true;
            countdownNumber_->SetDisplay(false); // カウントダウン非表示
            startDisplay_->SetDisplay(true);    // スタート表示を有効化
        }
    }

    if (isStartActive_) {
        // スタート表示中の処理
        startDisplay_->Update();

        if (!startDisplay_->IsStartVisible()) {
            // スタートの表示が終了
            isStartActive_ = false;
            isFinished_ = true; // 全ての処理が完了
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
    // カウントダウンを開始
    isCounting_ = true;
    isStartActive_ = false;
    isFinished_ = false;

    countdownNumber_->SetDisplay(true); // カウントダウンを表示
    startDisplay_->SetDisplay(false);  // スタートを非表示
    countdownNumber_->Initialize();    // カウントダウンを再初期化
}