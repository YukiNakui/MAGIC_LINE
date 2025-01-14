#include "Capsule.h"
#include"Engine/Model.h"

Capsule::Capsule(GameObject* parent)
	:GameObject(parent,"Capsule"),hModel_(-1)
{
}

void Capsule::Initialize()
{
	hModel_ = Model::Load("Model/capsule.fbx");
	assert(hModel_ >= 0);
}

void Capsule::Update()
{
}

void Capsule::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Capsule::Release()
{
}
