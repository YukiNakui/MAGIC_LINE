#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
//#include"PlayerCamera.h"
#include"Engine/Camera.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1),cdTimer_(nullptr), lookTarget_{ 0,0,0 },front_{0,0,1,0}
{
}

void Player::Initialize()
{
	cdTimer_ = Instantiate<CDTimer>(this);
	hModel_ = Model::Load("Model/Player.fbx");
	assert(hModel_ >= 0);
}

void Player::Update()
{
	//PlayerCamera* pPlayerCam = GetParent()->FindGameObject<PlayerCamera>();
	//XMFLOAT3 camRot = pPlayerCam->GetRotate();

	//XMVECTOR inputDirection = XMVectorZero();
	//inputDirection += XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//float moveValue = XMVectorGetX(XMVector3Length(inputDirection));
	//if (moveValue > 1.0f) {//入力値が規定値オーバー
	//	inputDirection = XMVector3Normalize(inputDirection);
	//}
	////if (moveValue > 0.0f) {
	////	XMVECTOR move = inputDirection * cdTimer_->GetDeltaTime()/100 * moveValue; //動くスピードによってプレイヤーがカメラの中心からずれてしまう
	////	XMVECTOR addMove = XMVector3TransformCoord(move, XMMatrixRotationX(camRot.x) * XMMatrixRotationY(camRot.y)/* * XMMatrixRotationZ(camRot.z)*/);
	////	XMVECTOR playerPos = XMVectorZero();
	////	playerPos = XMLoadFloat3(&transform_.position_);
	////	playerPos += addMove;
	////	XMStoreFloat3(&transform_.position_, playerPos);
	////}

	//if (Input::IsKey(DIK_UP)) {
	//	transform_.rotate_.x += XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	//	if (transform_.rotate_.x >= XMConvertToRadians(70.0f)) {
	//		transform_.rotate_.x = XMConvertToRadians(70.0f);
	//	}
	//}
	//if (Input::IsKey(DIK_DOWN)) {
	//	transform_.rotate_.x -= XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	//	if (transform_.rotate_.x <= XMConvertToRadians(-60.0f)) {
	//		transform_.rotate_.x = XMConvertToRadians(-60.0f);
	//	}
	//}
	//if (Input::IsKey(DIK_LEFT)) {
	//	transform_.rotate_.y += XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	//}
	//if (Input::IsKey(DIK_RIGHT)) {
	//	transform_.rotate_.y -= XMConvertToRadians(90.0f) * (float)cdTimer_->GetDeltaTime() / 1000;
	//}

	//XMVECTOR playerPosition = XMLoadFloat3(&transform_.position_);

	////カメラ操作の時の中心座標を求める
	//XMVECTOR center = playerPosition;
	//XMVECTOR defaultCamPos = { 0,10,-15 };
	//XMVECTOR defaultTargetPos = { 0,0,15 };
	//XMVECTOR cameraPosition, targetPosition;
	//cameraPosition = XMVector3TransformCoord(defaultCamPos, XMMatrixRotationX(transform_.rotate_.x) * XMMatrixRotationY(transform_.rotate_.y));
	//targetPosition = XMVector3TransformCoord(defaultTargetPos, XMMatrixRotationX(transform_.rotate_.x) * XMMatrixRotationY(transform_.rotate_.y));
	//XMFLOAT3 camPos, lookTargetPos;
	//XMStoreFloat3(&camPos, cameraPosition + center);
	//XMStoreFloat3(&lookTargetPos, targetPosition + center);
	//Camera::SetPosition(camPos);
	//Camera::SetTarget(lookTargetPos);
	//if (moveValue > 0.0f) {
	//	XMVECTOR move = inputDirection * cdTimer_->GetDeltaTime() / 100 * moveValue;
	//	XMVECTOR addMove = XMVector3TransformCoord(move, XMMatrixRotationX(transform_.rotate_.x) * XMMatrixRotationY(transform_.rotate_.y));
	//	XMVECTOR playerPos = XMVectorZero();
	//	playerPos = XMLoadFloat3(&transform_.position_);
	//	playerPos += addMove;
	//	XMStoreFloat3(&transform_.position_, playerPos);
	//}

	XMMATRIX rotX = XMMatrixIdentity();//行列の1 単位行列
	XMMATRIX rotY = XMMatrixIdentity();//行列の1 単位行列
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVec{ 0,0,0,0 };
	float dir = 1.0f;
	if (Input::IsKey(DIK_UP))
	{
		transform_.rotate_.x += 0.5f;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.rotate_.x -= 0.5f;
	}
	if (Input::IsKey(DIK_LEFT)) {
		transform_.rotate_.y -= 0.5f;
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y += 0.5f;
	}

	//回転行列を求める
	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	//ベクトルの回転結果を求める
	rotVec = XMVector3TransformCoord(front_, rotX * rotY);
	move = 0.3f * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));//XMVECTORに合わせる
	pos = pos + dir * move;//pos = pos + speed * front_ 実際に位置を移動させる
	XMStoreFloat3(&(transform_.position_), pos);

	Camera::SetTarget(transform_.position_);
	XMVECTOR vEye{ 0,10,-15,0 };
	vEye = XMVector3TransformCoord(vEye, rotX * rotY);
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, pos + vEye);
	Camera::SetPosition(camPos);
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
