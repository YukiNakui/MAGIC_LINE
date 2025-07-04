#include "Line.h"
#include"../Engine/Model.h"

Line::Line(GameObject* parent)
	:GameObject(parent,"Line"),hModel_(-1),radius_(1.0f),start_({0,0,1}),end_({0,0,-1}),isActive_(false)
{
}

void Line::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Capsule.fbx");
	assert(hModel_ >= 0);
}

void Line::Update()
{
	//カプセルの回転行列を適用して、始点と終点を更新
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(transform_.rotate_.x),
		XMConvertToRadians(transform_.rotate_.y),
		XMConvertToRadians(transform_.rotate_.z)
	);

	//元の始点・終点を回転行列で変換
	XMFLOAT3 startOringin = {0,0,1};
	XMFLOAT3 endOringin = { 0,0,-1 };
	XMVECTOR originalStart = XMLoadFloat3(&startOringin); //初期の始点
	XMVECTOR originalEnd = XMLoadFloat3(&endOringin); //初期の終点

	XMVECTOR rotatedStart = XMVector3Transform(originalStart, rotationMatrix);
	XMVECTOR rotatedEnd = XMVector3Transform(originalEnd, rotationMatrix);

	//変換後の座標を保存
	XMStoreFloat3(&start_, rotatedStart);
	XMStoreFloat3(&end_, rotatedEnd);

	//ワールド座標に適用
	XMVECTOR worldPos = XMLoadFloat3(&transform_.position_);
	XMStoreFloat3(&start_, XMVectorAdd(worldPos, rotatedStart));
	XMStoreFloat3(&end_, XMVectorAdd(worldPos, rotatedEnd));
}

void Line::Draw()
{
	if (!IsActive()) return; // 非アクティブなら描画しない

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Line::Release()
{
}

void Line::DrawShadowMap(const XMMATRIX& lightViewProj)
{
	// モデルにワールド変換をセット
	Model::SetTransform(hModel_, transform_);
	// シャドウマップ用描画
	Model::DrawShadow(hModel_, lightViewProj);
}
