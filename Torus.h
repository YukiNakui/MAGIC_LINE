#pragma once
#include "Engine/GameObject.h"

class Torus :
    public GameObject
{
	int hModel_;
public:
	Torus(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	
	bool CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius);
private:
	float holeRadius_; // �g�[���X�̌��̔��a
	float tubeRadius_; // �g�[���X�̃`���[�u�̔��a
};

