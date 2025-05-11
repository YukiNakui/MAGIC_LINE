#pragma once
#include "../Engine/GameObject.h"


class CameraOrbit :
    public GameObject
{
public:
    CameraOrbit(GameObject* parent);
    ~CameraOrbit();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //�J�����̎����ݒ肷��
    void SetOrbit(const XMFLOAT3& focusPoint, float radius, float speed);

    //������~����
    void StopOrbit() { isOrbiting_ = false; }

	//���񒆂��ǂ������擾����
	bool IsStopOrbit() { return isOrbiting_; }

    //���񏈗����X�V����
    void Update(float deltaTime);

private:
    bool isOrbiting_;            //���񒆂ł��邩�ǂ���
    XMFLOAT3 focusPoint_;        //�J�����̒����_
    float orbitRadius_;          //����̔��a
    float orbitSpeed_;           //����̑��x
    float orbitAngle_;           //���݂̎���p�x
};

