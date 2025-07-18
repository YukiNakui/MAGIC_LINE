#include "Torus.h"
#include"Engine/Model.h"


Torus::Torus(GameObject* parent)
	:GameObject(parent, "Torus"), hModel_(-1)
{
}

void Torus::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Ring.fbx");
	assert(hModel_ >= 0);
}

void Torus::Update()
{
}

void Torus::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Torus::Release()
{
}

bool Torus::CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius)
{
	XMFLOAT3 relativePos =
	{
		transform_.position_.x - sphereTrans.position_.x,
		transform_.position_.y - sphereTrans.position_.y,
		transform_.position_.z - sphereTrans.position_.z
	};


	return false;
}


