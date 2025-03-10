#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include<algorithm>
#include "Engine/SceneManager.h"
#include"Stage.h"
#include"Meter.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1),cdTimer_(nullptr), lookTarget_{ 0,0,0 },front_{0,0,1,0},
	 maxLineValue_(100.0f),currentLineValue_(0.0f),pCapsule_(nullptr),pText_(nullptr),pCountStart_(nullptr)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Model/Player.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0f,5.0f,-30.0f };
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(0.1f);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.5f);
	AddCollider(collision);

	//pCountStart_ = (CountStart*)FindObject("CountStart");
	canMove_ = false;

	pText_ = new Text;
	pText_ ->Initialize();
}

void Player::Update()
{
	if (pCountStart_ == nullptr) {
		pCountStart_ = (CountStart*)FindObject("CountStart");
		//return; // **まだカウント開始前なら動かない**
	}

	if (!canMove_)
	{
		if (pCountStart_ != nullptr && !pCountStart_->IsStartVisible())
		{
			canMove_ = true;  // **START が消えたら移動開始**
		}
		//else
		//{
		//	return; // **まだ動かない**
		//}
	}

	
	XMMATRIX rotX = XMMatrixIdentity();
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0, 0, 0, 0 };
	XMVECTOR rotVec{ 0, 0, 0, 0 };
	float dir = 1.0f;
	float deltaTime = cdTimer_->GetDeltaTime();
	if (canMove_) {
		if (Input::IsKey(DIK_UP)) {
			transform_.rotate_.x += 60.0f * deltaTime;
		}
		if (Input::IsKey(DIK_DOWN)) {
			transform_.rotate_.x -= 60.0f * deltaTime;
		}
		transform_.rotate_.x = std::clamp(transform_.rotate_.x, -45.0f, 45.0f);

		if (Input::IsKey(DIK_LEFT)) {
			transform_.rotate_.y -= 60.0f * deltaTime;
		}
		if (Input::IsKey(DIK_RIGHT)) {
			transform_.rotate_.y += 60.0f * deltaTime;
		}

		//プレイヤー移動処理
		rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
		rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
		rotVec = XMVector3TransformCoord(front_, rotX * rotY);
		move = 10.0f * rotVec;
	}
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	XMVECTOR addMove = dir * move * deltaTime;
	pos += addMove;
	XMStoreFloat3(&(transform_.position_), pos);


	//カメラ移動処理
	XMVECTOR vTarget{ 0, 0, 15, 0 };
	vTarget = XMVector3TransformCoord(vTarget, rotY);
	XMFLOAT3 targetPos;
	XMStoreFloat3(&targetPos, pos + vTarget);
	Camera::SetTarget(transform_.position_);
	XMVECTOR vEye{ 0, 10, -20, 0 };
	vEye = XMVector3TransformCoord(vEye, rotY);
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, pos + vEye);
	Camera::SetPosition(camPos);

	if (Input::IsKey(DIK_SPACE)) {
		if (currentLineValue_ <= maxLineValue_) {
			currentLineValue_ += XMVectorGetX(XMVector3Length(addMove));
			if (cdTimer_->IsTimeOver()) {
				pCapsule_ = Instantiate<Capsule>(this->GetParent());
				pCapsule_->SetPosition(transform_.position_);
				pCapsule_->SetRotate(transform_.rotate_);
				cdTimer_->ResetTimer();
			}
		}
	}

	Meter* pMeter_ = (Meter*)FindObject("Meter");
	pMeter_->SetMeterVal(maxLineValue_, currentLineValue_);

	Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
	int hGroundModel = pStage->GetModelHandle();    //モデル番号を取得

	RayCastData data;
	data.start = transform_.position_;   //レイの発射位置
	data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
	Model::RayCast(hGroundModel, &data); //レイを発射
	
	if (data.dist < 1.0f) {
		transform_.position_.y += 1.0f - data.dist;
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//pText_->Draw(30, 30, maxLineValue_ - currentLineValue_);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
}
