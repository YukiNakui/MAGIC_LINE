#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // 最大ステージ数
	float STAGE_IMAGE_WIDTH = 0.5f; // ステージ画像の幅
	float STAGE_IMAGE_HEIGHT = 0.5f; // ステージ画像の高さ
	float STAGE_IMAGE_SPACING = 0.1f; // ステージ画像の間隔
	float STAGE_IMAGE_X_OFFSET = 0.7f; // ステージ画像のX方向オフセット位置
	float STAGE_IMAGE_Y_OFFSET = 0.0f; // ステージ画像のY方向オフセット位置
	float THEME_IMAGE_Y_OFFSET = 0.5f; // お題画像のY方向オフセット位置
}

StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"),
	hStageSelectPict_(-1),
	currentStageIndex_(0), isExplUISelected_(false),
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

	stageSelectPictTrs_.position_ = { 0.0f, 0.7f, 0.0f }; // ステージ選択画面の画像の位置
	stageSelectPictTrs_.scale_ = { 0.5f, 0.5f, 0.0f }; // ステージ選択画面の画像のスケール
	stageFrameTrs_.scale_ = { 0.53f, 0.53f, 0.0f }; // ステージフレームのスケール
	explUITrs_.position_ = { 0.0f, -0.7f, 0.0f }; // 操作・ルール説明UIの位置
	explUITrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // 操作・ルール説明UIのスケール

	//ステージ情報一覧を読み込む
	CsvReader* csvStage = new CsvReader("CSV/StageData/StageInfoList.csv");
	int lines = csvStage->GetLines();
	for (int i = 1;i < lines;i++) {//1行目はヘッダーなのでスキップ
		stageInfos_.push_back(StageInfo());
		//0-indexから始まるので、i-1で調整
		int stageIndex = i - 1;
		//ステージファイル名を取得
		stageInfos_[stageIndex].fileName_ = "CSV/StageData/" + csvStage->GetString(i, 0) + ".csv";
		//ステージのテーマ画像とステージ画像をロード
		stageInfos_[stageIndex].hThemePict_ = Image::Load("UI/Theme/" + csvStage->GetString(i, 1) + ".png");
		assert(stageInfos_[stageIndex].hThemePict_ >= 0);
		stageInfos_[stageIndex].hStagePict_ = Image::Load("UI/StageImage/" + csvStage->GetString(i, 2) + ".png");
		assert(stageInfos_[stageIndex].hStagePict_ >= 0);
		//ステージ画像の表示位置を計算
		stageInfos_[stageIndex].stageImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].stageImgTrs_.position_.y = 0;
		stageInfos_[stageIndex].stageImgTrs_.scale_ = { 0.25f, 0.25f, 0.25f }; // ステージ画像のスケール
		//お題画像の表示位置を計算
		stageInfos_[stageIndex].themeImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].themeImgTrs_.position_.y = -THEME_IMAGE_Y_OFFSET;
		stageInfos_[stageIndex].themeImgTrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // お題画像のスケール
		stageInfos_[stageIndex].isSelected_ = false; // 初期は選択されていない
	}
	stageInfos_[0].isSelected_ = true; // 最初のステージを選択状態にする
	isExplUISelected_ = false; // 操作・ルール説明UIは初期は選択されていない
}

void StageSelectScene::Update()
{
	if (Input::IsKeyDown(DIK_RIGHT)) {
		// 右キーが押されたら次のステージへ
		if (!isExplUISelected_) {
			if (currentStageIndex_ < stageInfos_.size() - 1) {
				stageInfos_[currentStageIndex_].isSelected_ = false; // 現在のステージを非選択にする
				currentStageIndex_++;
				stageInfos_[currentStageIndex_].isSelected_ = true; // 次のステージを選択状態にする
			}
		}
	}
	else if (Input::IsKeyDown(DIK_LEFT)) {
		// 左キーが押されたら前のステージへ
		if (!isExplUISelected_) {
			if (currentStageIndex_ > 0) {
				stageInfos_[currentStageIndex_].isSelected_ = false; // 現在のステージを非選択にする
				currentStageIndex_--;
				stageInfos_[currentStageIndex_].isSelected_ = true; // 前のステージを選択状態にする
			}
		}
	}
	else if (Input::IsKeyDown(DIK_UP)) {
		// 上キーが押されたら操作・ルール説明UIを非選択
		isExplUISelected_ = false;
		stageInfos_[currentStageIndex_].isSelected_ = true;
	}
	else if (Input::IsKeyDown(DIK_DOWN)) {
		// 下キーが押されたら操作・ルール説明UIを選択
		isExplUISelected_ = true;
		stageInfos_[currentStageIndex_].isSelected_ = false;
	}
	
	if (Input::IsKeyDown(DIK_RETURN)) {
		if (isExplUISelected_) {
			// 操作・ルール説明UIが選択されている場合
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_EXPLANATION); // 説明シーンに遷移
		}
		else {
			// ステージ選択決定時
			std::string selectedStageFile = stageInfos_[currentStageIndex_].fileName_; // 選んだファイル名
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeToPlaySceneWithFile(selectedStageFile);
		}
	}
}

void StageSelectScene::Draw()
{
	Image::SetTransform(hStageSelectPict_, stageSelectPictTrs_);
	Image::Draw(hStageSelectPict_);

	// 操作・ルール説明UIの選択状態に応じて描画
	if (isExplUISelected_) {
		Image::SetTransform(hExplUIYellowPict_, explUITrs_);
		Image::Draw(hExplUIYellowPict_);
	}
	else {
		Image::SetTransform(hExplUIGrayPict_, explUITrs_);
		Image::Draw(hExplUIGrayPict_);
	}

	// 選択されているステージのフレームを描画
	for (int i = 0; i < stageInfos_.size(); i++) {
		stageFrameTrs_.position_ = stageInfos_[i].stageImgTrs_.position_;
		if (stageInfos_[i].isSelected_) {
			Image::SetTransform(hStageFrameYellowPict_, stageFrameTrs_);
			Image::Draw(hStageFrameYellowPict_);
		}
		else {
			Image::SetTransform(hStageFrameGrayPict_, stageFrameTrs_);
			Image::Draw(hStageFrameGrayPict_);
		}
	}
	// ステージ情報を描画
	for (int i = 0; i < stageInfos_.size(); i++) {
		// ステージのテーマ画像を描画
		Image::SetTransform(stageInfos_[i].hThemePict_, stageInfos_[i].themeImgTrs_);
		Image::Draw(stageInfos_[i].hThemePict_);
		
		// ステージの画像を描画
		Image::SetTransform(stageInfos_[i].hStagePict_, stageInfos_[i].stageImgTrs_);
		Image::Draw(stageInfos_[i].hStagePict_);
	}
}

void StageSelectScene::Release()
{
}
