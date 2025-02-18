#pragma once
#include "Collider.h"

//-----------------------------------------------------------
//カプセルの当たり判定
//-----------------------------------------------------------
class CapsuleCollider : public Collider
{
	// Colliderクラスのprivateメンバにアクセスできるようにする
		friend class Collider;

		XMFLOAT3 start_;
		XMFLOAT3 end_;
		float radius_;

public:
	//コンストラクタ（当たり判定の作成）
	//引数：center	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：start	当たり判定の始点位置（ゲームオブジェクトの原点から見た位置）
	//引数：end   	当たり判定の終点位置（ゲームオブジェクトの原点から見た位置）
	//引数：radius	当たり判定のサイズ（半径）
	CapsuleCollider(XMFLOAT3 center, XMFLOAT3 start, XMFLOAT3 end, float radius);

private:
	//接触判定
	//引数：target	相手の当たり判定
	//戻値：接触してればtrue
	bool IsHit(Collider* target) override;
};

