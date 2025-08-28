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
		None,           // 当たらない（通り抜ける）
		TubeCollision,  // 管（ドーナツ部分）に当たる
		CapCollision    // 穴の蓋にぶつかる
	};
	TorusHitType CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius);
private:
	float mainRadius_; // トーラスの中心からチューブの中心までの距離
	float tubeRadius_; // トーラスのチューブの半径
	XMFLOAT3 torusAxis_; // トーラスの軸方向
};

