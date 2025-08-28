#include "Torus.h"
#include"Engine/Model.h"

namespace {
	const float DEFAULT_MAIN_RADIUS = 1.0f; // デフォルトのトーラスの中心からチューブの中心までの距離(ドーナツの穴の大きさ＋チューブの半径)
	const float DEFAULT_TUBE_RADIUS = 0.2f; // デフォルトのトーラスのチューブの半径(ドーナツの太さ)
	const XMFLOAT3 DEFAULT_AXIS = { 0.0f, 1.0f, 0.0f }; // デフォルトのトーラスの軸方向(穴の空いている向き)
}


Torus::Torus(GameObject* parent)
	:GameObject(parent, "Torus"), hModel_(-1)
{
}

void Torus::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Torus.fbx");
	assert(hModel_ >= 0);

	mainRadius_ = DEFAULT_MAIN_RADIUS;
	tubeRadius_ = DEFAULT_TUBE_RADIUS;
	torusAxis_ = DEFAULT_AXIS;
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

void Torus::SetCollider(XMFLOAT3 rotation, XMFLOAT3 scale)
{
	mainRadius_ = DEFAULT_MAIN_RADIUS * scale.x; // スケールに基づいてメイン半径を調整
	tubeRadius_ = DEFAULT_TUBE_RADIUS * scale.y; // スケールに基づいてチューブ半径を調整
    //モデルの回転に応じて軸を回転させる
	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
    XMVECTOR axis = XMVector3TransformNormal(XMLoadFloat3(&DEFAULT_AXIS), rotMatrix);
	XMStoreFloat3(&torusAxis_, axis);
}

Torus::TorusHitType Torus::CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius)
{
    XMFLOAT3 centerToSphere = {
       sphereTrans.position_.x - transform_.position_.x,
       sphereTrans.position_.y - transform_.position_.y,
       sphereTrans.position_.z - transform_.position_.z
    };
    XMVECTOR v = XMLoadFloat3(&centerToSphere);
    XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torusAxis_));
    float dot = XMVectorGetX(XMVector3Dot(v, axis));
    XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot));
    float d = XMVectorGetX(XMVector3Length(v_proj));
    float holeRadius = mainRadius_ - tubeRadius_;

    // 管表面との衝突判定（軸上以外でも常に優先）
    XMVECTOR nearestOnCircle = XMVectorScale(
        d > 1e-6f ? XMVector3Normalize(v_proj) : XMVectorZero(), mainRadius_);
    XMVECTOR tubeCenter = XMVectorAdd(XMLoadFloat3(&transform_.position_), nearestOnCircle);
    XMVECTOR sphereToTube = XMVectorSubtract(XMLoadFloat3(&sphereTrans.position_), tubeCenter);
    float distToTubeCenter = XMVectorGetX(XMVector3Length(sphereToTube));
    if (distToTubeCenter <= (tubeRadius_ + sphereRadius)) {
        return TorusHitType::TubeCollision;
    }

    // 軸上判定（管に当たっていない場合のみ）
    //if (d < 1e-6f) {
        // 「穴より大きい」だけでなく「蓋に接しているか」も判定
        // 蓋の位置は torus の中心から mainRadius の距離
        float centerToBallLen = XMVectorGetX(XMVector3Length(v));
        float capSurfaceDist = fabs(centerToBallLen - mainRadius_);
        if (sphereRadius >= holeRadius && capSurfaceDist <= sphereRadius) {
            return TorusHitType::CapCollision; // 蓋に接触
        }
        else {
            return TorusHitType::None; // 穴通過
        }
    //}現在ボールが穴の大きさより小さいのに通り抜けずに当たってしまうバグがある

    return TorusHitType::None;
}


