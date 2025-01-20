#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include<algorithm>
#include "Engine/SceneManager.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1),cdTimer_(nullptr), lookTarget_{ 0,0,0 },front_{0,0,1,0},
	 totalMoveValue_(0.0f),pCapsule_(nullptr),pText_(nullptr)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Model/Player.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0f,10.0f,-30.0f };
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(0.1f);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.5f);
	AddCollider(collision);

	pText_ = new Text;
	pText_ ->Initialize();
}

void Player::Update()
{
	XMMATRIX rotX = XMMatrixIdentity();
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0, 0, 0, 0 };
	XMVECTOR rotVec{ 0, 0, 0, 0 };
	float dir = 1.0f;
	float deltaTime = cdTimer_->GetDeltaTime();

	if (Input::IsKey(DIK_UP)) {
		transform_.rotate_.x += 30.0f * deltaTime;
	}
	if (Input::IsKey(DIK_DOWN)) {
		transform_.rotate_.x -= 30.0f * deltaTime;
	}
	transform_.rotate_.x = std::clamp(transform_.rotate_.x, -45.0f, 45.0f);

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
	XMVECTOR addMove = dir * move * deltaTime;
	pos += addMove;
	XMStoreFloat3(&(transform_.position_), pos);

	XMVECTOR vTarget{ 0, 0, 15, 0 };
	vTarget = XMVector3TransformCoord(vTarget, rotY);
	XMFLOAT3 targetPos;
	XMStoreFloat3(&targetPos, pos + vTarget);
	Camera::SetTarget(transform_.position_);
	XMVECTOR vEye{ 0, 10, -15, 0 };
	vEye = XMVector3TransformCoord(vEye, rotY);
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, pos + vEye);
	Camera::SetPosition(camPos);

	if (Input::IsKey(DIK_SPACE)) {
		totalMoveValue_ += XMVectorGetX(XMVector3Length(addMove));

		if (totalMoveValue_ <= 100.0f) {
			if (cdTimer_->IsTimeOver()) {
				pCapsule_ = Instantiate<Capsule>(this->GetParent());
				pCapsule_->SetPosition(transform_.position_);
				pCapsule_->SetRotate(transform_.rotate_);
				cdTimer_->ResetTimer();
			}
		}
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	pText_->Draw(30, 30, totalMoveValue_);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
	/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	pSceneManager->ChangeScene(SCENE_ID_TITLE);*/
}
