#pragma once
#include "Engine/GameObject.h"

class Capsule :
    public GameObject
{
	int hModel_;
public:
	Capsule(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	int GetModelHandle() { return hModel_; }
private:
	float radius_;    //半径
	XMFLOAT3 start_;  //始点
	XMFLOAT3 end_;    //終点
public:
	float GetRadius() { return radius_; }
	XMFLOAT3 GetStartPos() { return start_; }
	XMFLOAT3 GetEndPos() { return end_; }
};

