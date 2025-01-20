#include "Capsule.h"
#include"Engine/Model.h"
#include"Engine/CapsuleCollider.h"

Capsule::Capsule(GameObject* parent)
	:GameObject(parent,"Capsule"),hModel_(-1)
{
}

void Capsule::Initialize()
{
	hModel_ = Model::Load("Model/capsule.fbx");
	assert(hModel_ >= 0);
	CapsuleCollider* collision = new CapsuleCollider(XMFLOAT3{0,0,1}, XMFLOAT3{0,0,-1}, 1.0f);
	AddCollider(collision);
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
