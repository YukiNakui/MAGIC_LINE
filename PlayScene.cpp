#include "PlayScene.h"
#include "Player.h"
#include"Stage.h"
#include"Ball.h"
#include"Meter.h"
#include"StageFrame.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
{
}

void PlayScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
	Instantiate<Ball>(this);
	Instantiate<Meter>(this);
	Instantiate<StageFrame>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
