#include "Wall.h"
#include"Engine/Model.h"

Wall::Wall(GameObject* parent)
	:GameObject(parent, "Wall")
{
}

void Wall::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Wall.fbx");
	assert(hModel_ >= 0);
}

void Wall::Update()
{
}

void Wall::Draw()
{
}

void Wall::Release()
{
}
