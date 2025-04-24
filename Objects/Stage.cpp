#include "Stage.h"
#include"../Engine/Model.h"
#include"../Engine/BoxCollider.h"
#include<vector>

static const int STAGE_WIDTH = 20;  //ステージのx方向最大
static const int STAGE_HEIGHT = 1;  //ステージのy方向最大
static const int STAGE_LENGTH = 20; //ステージのz方向最大

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),hModel_(-1)
{
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Box.fbx");
	assert(hModel_ >= 0);
	BoxCollider* collision = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(STAGE_WIDTH * 5, STAGE_HEIGHT * 5, STAGE_LENGTH * 5));
	AddCollider(collision);
}

void Stage::Update()
{

}

void Stage::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Stage::Release()
{
}
