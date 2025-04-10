#pragma once
#include "Engine/GameObject.h"
class Arrow :
    public GameObject
{
	int hModel_;
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	XMFLOAT3 targetPos_;
	XMFLOAT3 playerPos_;
	XMVECTOR directionVec_;
	XMVECTOR playerPosVec_;
	XMVECTOR targetPosVec_;
};

