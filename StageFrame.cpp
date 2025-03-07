#include "StageFrame.h"
#include"Engine/Model.h"

StageFrame::StageFrame(GameObject* parent)
	:GameObject(parent, "StageFrame"), hModel1_(-1), hModel2_(-1), hModel3_(-1)
{
}

void StageFrame::Initialize()
{
	hModel1_ = Model::Load("Model/Locomotive.fbx");
	assert(hModel1_ >= 0);
	hModel2_ = Model::Load("Model/FreightCar.fbx");
	assert(hModel2_ >= 0);
	hModel3_ = Model::Load("Model/PassengerCar.fbx");
	assert(hModel3_ >= 0);
	loco1_.position_ = { 25.0f,-5.0f,0.0f };
	loco1_.rotate_ = { 0.0f,180.0f,0.0f };
	loco1_.scale_ = { 1.5f,1.5f,1.5f };
	loco2_.position_ = { 0.0f,-5.0f,0.0f };
	loco2_.rotate_ = { 0.0f,180.0f,0.0f };
	loco2_.scale_ = { 1.0f,1.2f,1.5f };
	loco3_.position_ = { -20.0f,-5.0f,0.0f };
	loco3_.rotate_ = { 0.0f,180.0f,0.0f };
	loco3_.scale_ = { 1.0f,1.2f,1.5f };
}

void StageFrame::Update()
{
}

void StageFrame::Draw()
{
	Model::SetTransform(hModel1_, loco1_);
	Model::Draw(hModel1_);
	Model::SetTransform(hModel2_, loco2_);
	Model::Draw(hModel2_);
	Model::SetTransform(hModel3_, loco3_);
	Model::Draw(hModel3_);
}

void StageFrame::Release()
{
}
