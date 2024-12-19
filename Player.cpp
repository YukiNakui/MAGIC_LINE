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
	XMMATRIX rotX = XMMatrixIdentity();
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0,0,0,0 };
	XMVECTOR rotVec{ 0,0,0,0 };
	float dir = 1.0f;
	float deltaTime = cdTimer_->GetDeltaTime();
	if (Input::IsKey(DIK_UP))
	{
		transform_.rotate_.x += 30.0f * deltaTime;
	}
	if (Input::IsKey(DIK_DOWN))
	{
		transform_.rotate_.x -= 30.0f * deltaTime;
	}
	if (Input::IsKey(DIK_LEFT)) {
		transform_.rotate_.y -= 30.0f * deltaTime;
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y += 30.0f * deltaTime;
	}


	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

	rotVec = XMVector3TransformCoord(front_, rotX * rotY);
	move = 10.0f * rotVec;
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	pos += dir * move * deltaTime;
	XMStoreFloat3(&(transform_.position_), pos);

	XMVECTOR vTarget{ 0,0,15,0 };
	vTarget = XMVector3TransformCoord(vTarget, rotX * rotY);
	XMFLOAT3 targetPos;
	XMStoreFloat3(&targetPos, pos + vTarget);
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
