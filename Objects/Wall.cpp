#include "Wall.h"
#include"../Engine/Model.h"

Wall::Wall(GameObject* parent)
	:GameObject(parent, "Wall")
{
}

void Wall::Initialize()
{
	hModel_ = Model::Load("Models/Objects/RainbowWall.fbx");
	assert(hModel_ >= 0);
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
