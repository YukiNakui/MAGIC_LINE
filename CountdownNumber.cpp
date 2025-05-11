#include "CountdownNumber.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include <cmath>
#include <cassert>
#include<algorithm>

namespace {
    const float MAX_SCALE = 3.0f;             //カウント表示の最大スケール
    const float MIN_SCALE = 1.0f;             //カウント表示の最小スケール
    const float COUNTDOWN_INTERVAL = 1.0f;    //カウントダウン間隔
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
    //カウントダウン用の画像をロード
    for (int i = 0; i < COUNT_IMAGES; i++) {
        hPictCount_[i] = Image::Load(("UI/CountDown/count_" + std::to_string(i + 1) + ".png").c_str());
        assert(hPictCount_[i] >= 0);
    }

    //サウンドをロード
    hCountSound_ = Audio::Load("Sounds/SoundEffect/CountSound.wav");
    assert(hCountSound_ >= 0);

    //タイマーオブジェクトを生成して初期時間を設定
    cdTimer_ = Instantiate<CDTimer>(this);
    cdTimer_->SetInitTime(COUNTDOWN_INTERVAL);

    //初回のカウントサウンドを再生
    Audio::Play(hCountSound_);
}

void CountdownNumber::Update()
{
    //フレーム間の経過時間を取得
    deltaTime_ = cdTimer_->GetDeltaTime();
    elapsedTime_ += deltaTime_; //累積経過時間を更新

    if (elapsedTime_ >= COUNTDOWN_INTERVAL && currentCount_ >= 0) {
        //カウントダウンを進行
        elapsedTime_ = 0.0f; //経過時間をリセット
        currentCount_--;     //カウントをデクリメント

        if (currentCount_ <= 0) {
            //カウントダウンが終了した場合
            isFinished_ = true;
        }
        else {
            //カウントダウンサウンドを再生
            Audio::Play(hCountSound_);
        }

        //スケールをリセットし、タイマーもリセット
        scale_ = MAX_SCALE;
        cdTimer_->ResetTimer();
    }

    //スケール補間処理
    if (isFinished_) return; //カウントダウン終了時は補間をスキップ
    float t = std::clamp(elapsedTime_, 0.0f, COUNTDOWN_INTERVAL); //経過時間をクランプ
    float smoothT = 0.5f * (1.0f - cos(t * XM_PI));               //イージングを適用
    scale_ = MAX_SCALE - smoothT * (MAX_SCALE - MIN_SCALE);       //スケールを補間
    transform_.scale_ = { scale_, scale_, scale_ };               //Transformのスケールを更新
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
    currentCount_ = COUNT_IMAGES;  //カウントを初期値に戻す
    elapsedTime_ = 0.0f;           //経過時間をリセット
    isCounting_ = true;            //カウントダウン中フラグを有効化
    isFinished_ = false;           //終了フラグを無効化
    scale_ = MAX_SCALE;            //スケールを初期値に戻す
    Audio::Play(hCountSound_);     //サウンドを再生
}
