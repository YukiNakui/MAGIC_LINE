#pragma once
#include "Engine/GameObject.h"
#include "CountdownNumber.h"
#include "StartDisplay.h"

class StartCountdownManager : public GameObject {
public:
    StartCountdownManager(GameObject* parent);
    ~StartCountdownManager();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

	//カウントダウンが終了したかどうか
    bool IsFinished() const { return isFinished_; }

private:
	CountdownNumber* countdownNumber_;//カウントダウン表示
	StartDisplay* startDisplay_; //スタート表示
    bool isCounting_;  //現在カウントダウン中かどうか
    bool isStartActive_; //現在スタート表示中かどうか
    bool isFinished_;  //全ての処理が終了したかどうか
};


