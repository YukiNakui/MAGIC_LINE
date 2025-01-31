#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
#include"Capsule.h"
#include"Engine/Text.h"

class Player :
    public GameObject
{
	int hModel_;
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

private:
	CDTimer* cdTimer_;
	XMFLOAT3 lookTarget_;
	XMVECTOR front_;

	float maxLineValue_;
	float currentLineValue_;//åªç›à¯Ç¢ÇΩê¸ÇÃó 
	Capsule* pCapsule_;

	Text* pText_;
};

