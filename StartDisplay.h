#pragma once
#include "CountdownBase.h"
#include"CountdownNumber.h"

class StartDisplay : public CountdownBase {
public:
    StartDisplay(GameObject* parent);
    virtual ~StartDisplay();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    bool IsStartVisible() const { return isStartVisible_; }

private:
    int hPictStart_;         // 「START」画像のハンドル
    int hStartSound_;        // 「START」音のハンドル
    bool isStartVisible_;    // 「START」が表示中か
    float startElapsedTime_; // 「START」の経過時間
};

