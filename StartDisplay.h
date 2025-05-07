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
    int hPictStart_;         // �uSTART�v�摜�̃n���h��
    int hStartSound_;        // �uSTART�v���̃n���h��
    bool isStartVisible_;    // �uSTART�v���\������
    float startElapsedTime_; // �uSTART�v�̌o�ߎ���
};

