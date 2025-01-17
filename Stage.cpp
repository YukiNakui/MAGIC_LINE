#include "Stage.h"
#include"Engine/Model.h"
#include<vector>

const int STAGE_WIDTH = 20;  //ステージのx方向最大
const int STAGE_HEIGHT = 1;  //ステージのy方向最大
const int STAGE_LENGTH = 20; //ステージのz方向最大

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hModel_(-1),pStageBox_(nullptr)
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Model/BoxDefault.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = { 5.0,5.0,5.0 };
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int l = 0; l < STAGE_LENGTH; l++) {
		for (int w = 0; w < STAGE_WIDTH; w++) {
			for (int h = 0; h < STAGE_HEIGHT; h++) {
				transform_.position_.x = w * 5;
				transform_.position_.y = h * 5;
				transform_.position_.z = l * 5;
				Model::SetTransform(hModel_, transform_);
				Model::Draw(hModel_);
				//pStageBox_ = Instantiate<StageBox>(this->GetParent());
				//pStageBox_->SetPosition(transform_.position_);
			}
		}
	}
}

void Stage::Release()
{
}
