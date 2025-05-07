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
    int hPictCount_[COUNT_IMAGES]; // �J�E���g�_�E���摜�̃n���h��
    int hCountSound_;              // �J�E���g�_�E�����̃n���h��
    int currentCount_;             // ���݂̃J�E���g
    bool isCounting_;              // �J�E���g�_�E������
    bool finished_;                // �J�E���g�_�E�����I��������
};

