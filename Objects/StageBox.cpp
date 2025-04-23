#include "StageBox.h"
#include"../Engine/Model.h"

StageBox::StageBox(GameObject* parent)
	:GameObject(parent,"StageBox"),hModel_(-1)
{
}

void StageBox::Initialize()
{
	hModel_ = Model::Load("Models/Obejcts/BoxDefault.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = { 5.0,5.0,5.0 };
}

void StageBox::Update()
{
}

void StageBox::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void StageBox::Release()
{
}
