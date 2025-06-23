#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // 最大ステージ数
	int STAGE_IMAGE_WIDTH = 3; // ステージ画像の幅
	int STAGE_IMAGE_HEIGHT = 3; // ステージ画像の高さ
	int STAGE_IMAGE_SPACING = 2; // ステージ画像の間隔
	int STAGE_IMAGE_X_OFFSET = 5; // ステージ画像のX方向オフセット位置
	int STAGE_IMAGE_Y_OFFSET = 5; // ステージ画像のY方向オフセット位置
	int THEME_IMAGE_Y_OFFSET = 3; // お題画像のY方向オフセット位置
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
	//画像をロード
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
		//ステージファイル名を取得
		stageInfos_[i].fileName = "CSV/StageData/" + csvStage->GetString(i, 0) + ".csv";
		//ステージのテーマ画像とステージ画像をロード
		stageInfos_[i].hThemePict = Image::Load("UI/ThemeImage/" + csvStage->GetString(i, 1) + ".png");
		assert(stageInfos_[i].hThemePict >= 0);
		stageInfos_[i].hStagePict = Image::Load("UI/StageImage/" + csvStage->GetString(i, 2) + ".png");
		assert(stageInfos_[i].hStagePict >= 0);
		//ステージ画像の表示位置を計算
		stageInfos_[i].stageImgTrs.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * i + STAGE_IMAGE_SPACING * i;
		stageInfos_[i].stageImgTrs.position_.y = 0;
		//お題画像の表示位置を計算
		stageInfos_[i].themeImgTrs.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * i + STAGE_IMAGE_SPACING * i;
		stageInfos_[i].themeImgTrs.position_.y = THEME_IMAGE_Y_OFFSET;
		stageInfos_[i].isSelected = false; // 初期は選択されていない
	}
	stageInfos_[0].isSelected = true; // 最初のステージを選択状態にする
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
	Image::SetTransform(hStageSelectPict_, stageSelectPictTrs_);
	Image::Draw(hStageSelectPict_);
	Image::SetTransform(hExplUIGrayPict_, explUITrs_);
	Image::Draw(hExplUIGrayPict_);
	Image::SetTransform(hExplUIYellowPict_, explUITrs_);
	Image::Draw(hExplUIYellowPict_);
	// ステージ情報を描画
	for (int i = 0; i < stageInfos_.size(); i++) {
		// ステージのテーマ画像を描画
		Image::SetTransform(stageInfos_[i].hThemePict, stageInfos_[i].themeImgTrs);
		Image::Draw(stageInfos_[i].hThemePict);
		// 選択されているステージのフレームを描画
		if (stageInfos_[i].isSelected) {
			Image::SetTransform(hStageFrameYellowPict_, stageFrameTrs_);
			Image::Draw(hStageFrameYellowPict_);
		}
		else {
			Image::SetTransform(hStageFrameGrayPict_, stageFrameTrs_);
			Image::Draw(hStageFrameGrayPict_);
		}
		// ステージの画像を描画
		Image::SetTransform(stageInfos_[i].hStagePict, stageInfos_[i].stageImgTrs);
		Image::Draw(stageInfos_[i].hStagePict);
	}
}

void StageSelectScene::Release()
{
}
