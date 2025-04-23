#include "PlayScene.h"
#include "../Objects/Player.h"
#include"../Objects/Stage.h"
#include"../Objects/Ball.h"
#include"../UI/LineGauge.h"
#include"../UI/ThemeDisplay.h"
#include"../UI/CountStart.h"
#include"../UI/PlayUI.h"
#include"../UI/Compass.h"
#include"../UI/Arrow.h"
#include"../UI/HeightMeter.h"
#include"../Engine/CsvReader.h"

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

	Player* pPlayer = Instantiate<Player>(this);
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
