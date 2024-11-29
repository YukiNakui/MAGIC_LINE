#include "PlayerCamera.h"
#include"Player.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"

PlayerCamera::PlayerCamera(GameObject* parent)
	:GameObject(parent, "PlayerCamera"), lookTarget_{0,0,0}
{
}

void PlayerCamera::Initialize()
{
	cdTimer_ = Instantiate<CDTimer>(this);
	Player* pPlayer = GetParent()->FindGameObject<Player>();
	XMFLOAT3 pPos = pPlayer->GetPosition();//プレイヤーの位置
	XMVECTOR playerPosition = XMLoadFloat3(&pPos);
	float rotY = pPlayer->GetRotate().y;//プレイヤーのY回転
	XMVECTOR firstCamPos = { 0,10,-15 };
	XMVECTOR fistTargetPos = { 0,0,15 };
	XMVECTOR cameraPosition = XMVector3TransformCoord(firstCamPos, XMMatrixRotationY(rotY)) + playerPosition;
	XMStoreFloat3(&transform_.position_, cameraPosition);
	XMVECTOR targetPosition = XMVector3TransformCoord(fistTargetPos, XMMatrixRotationY(rotY)) + playerPosition;
	XMStoreFloat3(&lookTarget_, targetPosition);
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
		transform_.rotate_.y -= XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y += XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	}

	Player* pPlayer = GetParent()->FindGameObject<Player>();
	XMFLOAT3 pPos = pPlayer->GetPosition();//プレイヤーの位置
	XMVECTOR playerPosition = XMLoadFloat3(&pPos);
	float rotY = pPlayer->GetRotate().y;//プレイヤーのY回転

	//カメラ操作の時の中心座標を求める 今回は首の位置が中心
	XMVECTOR headPos = { 0,1,0 };
	XMVECTOR center = playerPosition + headPos;//首の高さ
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
