#include "GameOverScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hPict_(-1), hBGM_(-1)
{
}

void GameOverScene::Initialize()
{
	hPict_ = Image::Load("GAMEOVER.png");
	assert(hPict_ >= 0);
	hBGM_ = Audio::Load("Sound/GameOverBGM.wav", true);
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void GameOverScene::Release()
{
}
