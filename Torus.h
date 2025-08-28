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
	
	float GetMainRadius() const { return mainRadius_; }
	float GetTubeRadius() const { return tubeRadius_; }
	const XMFLOAT3& GetAxis() const { return torusAxis_; }
	void SetCollider(XMFLOAT3 rotation, XMFLOAT3 scale);

	enum class TorusHitType {
		None,           // ������Ȃ��i�ʂ蔲����j
		TubeCollision,  // �ǁi�h�[�i�c�����j�ɓ�����
		CapCollision    // ���̊W�ɂԂ���
	};
	TorusHitType CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius);
private:
	float mainRadius_; // �g�[���X�̒��S����`���[�u�̒��S�܂ł̋���
	float tubeRadius_; // �g�[���X�̃`���[�u�̔��a
	XMFLOAT3 torusAxis_; // �g�[���X�̎�����
};

