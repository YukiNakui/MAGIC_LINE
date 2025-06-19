#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"

namespace {
	int MAX_STAGE_NUM = 3; // 最大ステージ数
}

StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"),
	hStageSelectPict_(-1), hStagePict_(-1),
	hStageFrameGrayPict_(-1), hStageFrameYellowPict_(-1),
	hExplUIGrayPict_(-1), hExplUIYellowPict_(-1)
{
}

void StageSelectScene::Initialize()
{
	hStageSelectPict_ = Image::Load("UI/StageSelect/StageSelect.png");
	assert(hStageSelectPict_ >= 0);
	
	hStageFrameGrayPict_ = Image::Load("UI/StageSelect/StageFrame_Gray.png");
	assert(hStageFrameGrayPict_ >= 0);
	hStageFrameYellowPict_ = Image::Load("UI/StageSelect/StageFrame_Yellow.png");
	assert(hStageFrameYellowPict_ >= 0);
	hExplUIGrayPict_ = Image::Load("UI/StageSelect/Explanation_Gray.png");
	assert(hExplUIGrayPict_ >= 0);
	hExplUIYellowPict_ = Image::Load("UI/StageSelect/Explanation_Yellow.png");
	assert(hExplUIYellowPict_ >= 0);


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
	Image::SetTransform(hStageSelectPict_, transform_);
	Image::Draw(hStageSelectPict_);
	Image::SetTransform(hStageFrameGrayPict_, transform_);
	Image::Draw(hStageFrameGrayPict_);
	Image::SetTransform(hStageFrameYellowPict_, transform_);
	Image::Draw(hStageFrameYellowPict_);
	Image::SetTransform(hExplUIGrayPict_, transform_);
	Image::Draw(hExplUIGrayPict_);
	Image::SetTransform(hExplUIYellowPict_, transform_);
	Image::Draw(hExplUIYellowPict_);
}

void StageSelectScene::Release()
{
}
