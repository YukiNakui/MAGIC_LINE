#include "Ball.h"
#include"Engine/Model.h"

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball"),hModel_(-1)
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Model/Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0,5.0,5.0 };
	transform_.scale_ = { 5.0f,5.0f,5.0f };
}

void Ball::Update()
{
	
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}
