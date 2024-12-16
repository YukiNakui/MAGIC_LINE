#include "PlayerCamera.h"
#include"Player.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"

PlayerCamera::PlayerCamera(GameObject* parent)
	:GameObject(parent, "PlayerCamera"), lookTarget_{0,0,0},cdTimer_(nullptr)
{
}

void PlayerCamera::Initialize()
{
	cdTimer_ = Instantiate<CDTimer>(this);
	pPlayer_ = GetParent()->FindGameObject<Player>();
	transform_.rotate_ = { 0,0,0 };
}

void PlayerCamera::Update()
{
	if (Input::IsKey(DIK_UP)) {
		transform_.rotate_.x += XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime()/1000;
		if (transform_.rotate_.x >= XMConvertToRadians(70.0f)) {
			transform_.rotate_.x = XMConvertToRadians(70.0f);
		}
	}
	if (Input::IsKey(DIK_DOWN)) {
		transform_.rotate_.x -= XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime()/1000;
		if (transform_.rotate_.x <= XMConvertToRadians(-60.0f)) {
			transform_.rotate_.x = XMConvertToRadians(-60.0f);
		}
	}
	if (Input::IsKey(DIK_LEFT)) {
		transform_.rotate_.y += XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
		pPlayer_->SetRotateY(-90.0f);
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y -= XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
		pPlayer_->SetRotateY(90.0f);
	}

	XMFLOAT3 pPos = pPlayer_->GetPosition();//プレイヤーの位置
	XMVECTOR playerPosition = XMLoadFloat3(&pPos);

	//カメラ操作の時の中心座標を求める
	XMVECTOR center = playerPosition;
	XMVECTOR defaultCamPos = { 0,10,-15 };
	XMVECTOR defaultTargetPos = { 0,0,15 };
	XMVECTOR cameraPosition;
	XMVECTOR targetPosition;
	cameraPosition = XMVector3TransformCoord(defaultCamPos, XMMatrixRotationX(transform_.rotate_.x) * XMMatrixRotationY(transform_.rotate_.y));
	targetPosition = XMVector3TransformCoord(defaultTargetPos, XMMatrixRotationX(transform_.rotate_.x) * XMMatrixRotationY(transform_.rotate_.y));
	XMStoreFloat3(&transform_.position_, cameraPosition + center);
	XMStoreFloat3(&lookTarget_, targetPosition + center);
}

void PlayerCamera::Draw()
{
	Camera::SetPosition(transform_.position_);
	Camera::SetTarget(lookTarget_);
}

void PlayerCamera::Release()
{
}
