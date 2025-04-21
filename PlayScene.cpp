#include "PlayScene.h"
#include "Player.h"
#include"Stage.h"
#include"Ball.h"
#include"LineGauge.h"
#include"ThemeDisplay.h"
#include"CountStart.h"
#include"PlayUI.h"
#include"Compass.h"
#include"Arrow.h"
#include"HeightMeter.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
{
}

void PlayScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Player>(this);
	Instantiate<Ball>(this);
	Instantiate<PlayUI>(this);
	Instantiate<Compass>(this);
	Instantiate<Arrow>(this);
	Instantiate<LineGauge>(this);
	Instantiate<HeightMeter>(this);
	Instantiate<ThemeDisplay>(this);
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
