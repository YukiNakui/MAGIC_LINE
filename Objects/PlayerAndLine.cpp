#include "PlayerAndLine.h"
#include"../Engine/Model.h"

PlayerAndLine::PlayerAndLine(GameObject* parent)
{
}

void PlayerAndLine::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Player&Line.fbx");
	assert(hModel_ >= 0);
}

void PlayerAndLine::Update()
{
}

void PlayerAndLine::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void PlayerAndLine::Release()
{
}
