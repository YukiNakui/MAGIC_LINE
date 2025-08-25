#include "Torus.h"
#include"Engine/Model.h"


Torus::Torus(GameObject* parent)
	:GameObject(parent, "Torus"), hModel_(-1)
{
}

void Torus::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Ring.fbx");
	assert(hModel_ >= 0);

	mainRadius_ = 3.0f; // トーラスの中心からチューブの中心までの距離
	tubeRadius_ = 1.0f; // トーラスのチューブの半径
	torusAxis_ = { 0.0f, 1.0f, 0.0f }; // トーラスの軸方向（Y軸方向）
}

void Torus::Update()
{
}

void Torus::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Torus::Release()
{
}

bool Torus::CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius)
{
    // 球の中心からトーラス中心へのベクトル
    XMFLOAT3 centerToSphere = {
        sphereTrans.position_.x - transform_.position_.x,
        sphereTrans.position_.y - transform_.position_.y,
        sphereTrans.position_.z - transform_.position_.z
    };
    XMVECTOR v = XMLoadFloat3(&centerToSphere);

    // トーラス軸ベクトル（正規化）
    XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torusAxis_));

    // v を軸方向に投影（回転面に落とす）
    float dot = XMVectorGetX(XMVector3Dot(v, axis));
    XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot)); // 回転面上

    float d = XMVectorGetX(XMVector3Length(v_proj)); // 回転面上での距離

    if (d == 0.0f) {
        // 球が軸上に位置する場合、最近接点は任意
        // 円周上のどこか（このケースではfalse推奨）
        return false;
    }

    // 回転円上の最近接点（トーラス管中心）
    XMVECTOR nearestOnCircle = XMVectorScale(XMVector3Normalize(v_proj), mainRadius_);

    // 球の中心と最近接点の距離
    XMVECTOR diff = XMVectorSubtract(v_proj, nearestOnCircle);
    float distToTubeCenter = XMVectorGetX(XMVector3Length(diff));

    // 判定：管半径＋球半径以内なら当たり
    return distToTubeCenter <= (tubeRadius_ + sphereRadius);
}


