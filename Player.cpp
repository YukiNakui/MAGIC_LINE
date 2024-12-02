#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"PlayerCamera.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1),cdTimer_(nullptr)
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
	PlayerCamera* pPlayerCam = GetParent()->FindGameObject<PlayerCamera>();
	XMFLOAT3 camRot = pPlayerCam->GetRotate();

	XMVECTOR inputDirection = XMVectorZero();
	
	if (Input::IsKey(DIK_W)) {
		//transform_.position_.y += 0.1f;
		XMVECTOR upVec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		inputDirection += upVec;
	}
	if (Input::IsKey(DIK_A)) {
		//transform_.position_.x -= 0.1f;
		XMVECTOR leftVec = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		inputDirection += leftVec;
	}
	if (Input::IsKey(DIK_S)) {
		//transform_.position_.y -= 0.1f;
		XMVECTOR downVec = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		inputDirection += downVec;
	}
	if (Input::IsKey(DIK_D)) {
		//transform_.position_.x += 0.1f;
		XMVECTOR rightVec = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		inputDirection += rightVec;
	}
	float moveValue = XMVectorGetX(XMVector3Length(inputDirection));
	if (moveValue > 1.0f) {//入力値が規定値オーバー
		inputDirection = XMVector3Normalize(inputDirection);
	}
	if (moveValue > 0.0f) {
		XMVECTOR move = inputDirection * cdTimer_->GetDeltaTime()/100 * moveValue;
		//XMVECTOR addX = XMVector3TransformCoord(move, XMMatrixRotationX(camRot.x));
		XMVECTOR addY = XMVector3TransformCoord(move, XMMatrixRotationY(camRot.y));
		//XMVECTOR addZ = XMVector3TransformCoord(move, XMMatrixRotationZ(camRot.z));
		XMVECTOR playerPos = XMVectorZero();
		playerPos = XMLoadFloat3(&transform_.position_);
		//XMVECTOR add = addX + addY + addZ;
		XMVECTOR add = addY;
		playerPos += add;
		XMStoreFloat3(&transform_.position_, playerPos);
		XMFLOAT3 playerRot = { 0.0f,0.0f,0.0f };
		XMStoreFloat3(&playerRot, add);
		//transform_.rotate_.y = atan2(playerRot.x, playerRot.z);
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
