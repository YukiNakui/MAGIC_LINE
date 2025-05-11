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

    //�\������Ă��邩
    bool IsStartVisible() const { return isStartVisible_; }

private:
    int hPictStart_;         //�摜�̃n���h��
    int hStartSound_;        //���̃n���h��
    bool isStartVisible_;    //�\������
    float startElapsedTime_; //�o�ߎ���
};

