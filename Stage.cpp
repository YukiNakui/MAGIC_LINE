#include "Stage.h"
#include"Engine/Model.h"

const int STAGE_LENGTH = 20;
const int STAGE_WIDTH = 20;
const int STAGE_HEIGHT = 1;

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hModel_(-1),stageLength_(STAGE_LENGTH),stageWidth_(STAGE_WIDTH),stageHeight_(STAGE_HEIGHT)
{
}

void Stage::Initialize()
{
	/*hModel_ = Model::Load("Model/BoxDefault.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = { 5.0,5.0,5.0 };*/
	pStageBox_ = Instantiate<StageBox>(this);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int l = 0; l < stageLength_; l++) {
		for (int w = 0; w < stageWidth_; w++) {
			for (int h = 0; h < stageHeight_; h++) {
				transform_.position_.x = w * 5;
				transform_.position_.y = h * 5;
				transform_.position_.z = l * 5;
				/*Model::SetTransform(hModel_, transform_);
				Model::Draw(hModel_);*/
				pStageBox_->SetPosition(transform_.position_);
			}
		}
	}
}

void Stage::Release()
{
}
