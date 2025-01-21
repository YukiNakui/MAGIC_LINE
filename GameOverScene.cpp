#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hPict_(-1)
{
}

void GameOverScene::Initialize()
{
	hPict_ = Image::Load("GameOver.png");
	assert(hPict_ >= 0);
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void GameOverScene::Release()
{
}
