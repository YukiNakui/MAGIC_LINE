#pragma once
#include "../Functions/CountdownBase.h"

class CountdownNumber : public CountdownBase {
public:
	//カウントダウン画像の枚数
    static constexpr int COUNT_IMAGES = 3;

    CountdownNumber(GameObject* parent);
    virtual ~CountdownNumber();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

	bool IsFinished() const { return isFinished_; }//カウントダウンが終了したか
    bool IsCounting() { return isCounting_; }//カウントダウンしているか
	void ResetCountdown();//カウントダウンをリセット
	void StartCountdown();//カウントダウンを開始
	void StopCountdown();//カウントダウンを停止
private:
    int hPictCount_[COUNT_IMAGES]; //カウントダウン画像のハンドル
    int hCountSound_;              //カウントダウン音のハンドル
    int currentCount_;             //現在のカウント
    bool isCounting_;              //カウントダウン中か
    bool isFinished_;              //カウントダウンが終了したか
};

