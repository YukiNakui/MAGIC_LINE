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
	bool CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius);
private:
	float mainRadius_; // トーラスの中心からチューブの中心までの距離
	float tubeRadius_; // トーラスのチューブの半径
	XMFLOAT3 torusAxis_; // トーラスの軸方向
};

