#pragma once
#include "CountdownBase.h"

class CountdownNumber : public CountdownBase {
public:
    static constexpr int COUNT_IMAGES = 3;

    CountdownNumber(GameObject* parent);
    virtual ~CountdownNumber();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool IsFinished() const { return finished_; }

private:
    int hPictCount_[COUNT_IMAGES]; // カウントダウン画像のハンドル
    int hCountSound_;              // カウントダウン音のハンドル
    int currentCount_;             // 現在のカウント
    bool isCounting_;              // カウントダウン中か
    bool finished_;                // カウントダウンが終了したか
};

