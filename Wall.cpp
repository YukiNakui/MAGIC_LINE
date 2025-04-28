#include "Wall.h"
#include"Engine/Model.h"

Wall::Wall(GameObject* parent)
	:GameObject(parent, "Wall")
{
}

void Wall::Initialize()
{
	hModel_ = Model::Load("Models/Objects/RainbowWall.fbx");
	assert(hModel_ >= 0);
	transform_.position_.y = -100.0f;
	SetScale(XMFLOAT3(30.0f, 50.0f, 30.0f));
}

void Wall::Update()
{
}

void Wall::Draw()
{
	Model::SetTransform(hModel_,transform_);
	Model::Draw(hModel_);
}

void Wall::Release()
{
}
