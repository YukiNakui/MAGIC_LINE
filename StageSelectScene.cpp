#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // 最大ステージ数
	int STAGE_IMAGE_WIDTH = 200; // ステージ画像の幅
	int STAGE_IMAGE_SPACING = 20; // ステージ画像の間隔
}

StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"),
	hStageSelectPict_(-1),
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

	//ステージ情報一覧を読み込む
	CsvReader* csvStage = new CsvReader("CSV/StageData/StageInfoList.csv");
	int lines = csvStage->GetLines();
	for (int i = 1;i < lines;i++) {
		stageInfos_.push_back(StageInfo());
		stageInfos_[i].fileName = csvStage->GetString(i, 0);
		stageInfos_[i].stageNum = csvStage->GetInt(i, 1);
		stageInfos_[i].hThemePict = Image::Load(csvStage->GetString(i, 2));
		assert(stageInfos_[i].hThemePict >= 0);
		stageInfos_[i].hStagePict = Image::Load(csvStage->GetString(i, 3));
		assert(stageInfos_[i].hStagePict >= 0);
		stageInfos_[i].x = csvStage->GetInt(i, 4);
		stageInfos_[i].y = csvStage->GetInt(i, 5);
		stageInfos_[i].isSelected = false; // 初期は選択されていない
	}
}

void StageSelectScene::Update()
{
	// ステージ選択決定時
	std::string selectedStageFile = stageInfos_[0].fileName; // 選んだファイル名
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
