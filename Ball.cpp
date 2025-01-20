#include "Ball.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"

Ball::Ball(GameObject* parent)
	:GameObject(parent, "Ball"), hModel_(-1), cdTimer_(nullptr), speed_(0,0,0)
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Model/Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0,50.0,5.0 };
	transform_.scale_ = { 5.0f,5.0f,5.0f };
	cdTimer_ = Instantiate<CDTimer>(this);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	AddCollider(collision);
}

void Ball::Update()
{
	float deltaTime = cdTimer_->GetDeltaTime();
	const float gravity = -9.8f;   // 重力加速度

	// 重力による速度変化
	speed_.y += gravity * deltaTime;

	// 速度による位置変化
	transform_.position_.y += speed_.y * deltaTime;
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}

void Ball::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Stage") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}
