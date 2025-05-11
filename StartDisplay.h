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

    //表示されているか
    bool IsStartVisible() const { return isStartVisible_; }

private:
    int hPictStart_;         //画像のハンドル
    int hStartSound_;        //音のハンドル
    bool isStartVisible_;    //表示中か
    float startElapsedTime_; //経過時間
};

