#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
	// Colliderクラスのprivateメンバにアクセスできるようにする
		friend class Collider;

		XMFLOAT3 start_;
		XMFLOAT3 end_;
		float radius_;

public:
	//コンストラクタ（当たり判定の作成）
	CapsuleCollider(XMFLOAT3 start, XMFLOAT3 end, float radius);

private:
	//接触判定
	bool IsHit(Collider* target) override;
};

