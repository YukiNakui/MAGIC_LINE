#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"


StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"), hBGPict_(-1), hStagePict_(-1)
{
}

void StageSelectScene::Initialize()
{
	hBGPict_ = Image::Load("Scenes/stageselect.png");
	assert(hBGPict_ >= 0);
}

void StageSelectScene::Update()
{
	// ステージ選択決定時
	std::string selectedStageFile = "CSV/StageData/stage01.csv"; // 選んだファイル名
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeToPlaySceneWithFile(selectedStageFile);
	}
}

void StageSelectScene::Draw()
{
	Image::SetTransform(hBGPict_, transform_);
	Image::Draw(hBGPict_);
}

void StageSelectScene::Release()
{
}
