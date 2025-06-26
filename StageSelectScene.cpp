#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"
#include"Engine/Model.h"
#include"PlayerAndLine.h"
#include"Objects/Stage.h"
#include"Objects/Ball.h"
#include"Objects/Wall.h"

namespace {
	float STAGE_IMAGE_WIDTH = 0.5f; // ステージ画像の幅
	float STAGE_IMAGE_HEIGHT = 0.5f; // ステージ画像の高さ
	float STAGE_IMAGE_SPACING = 0.1f; // ステージ画像の間隔
	float STAGE_IMAGE_X_OFFSET = 0.7f; // ステージ画像のX方向オフセット位置
	float STAGE_IMAGE_Y_OFFSET = 0.0f; // ステージ画像のY方向オフセット位置
	float THEME_IMAGE_Y_OFFSET = 0.4f; // お題画像のY方向オフセット位置
	float CAMERA_ORBIT_RADIUS = 100.0f;       //カメラの軌道半径
	float CAMERA_ORBIT_SPEED = 1.0f;          //カメラの軌道速度
	float CAMERA_ORBIT_HEIGHT = 25.0f;        //カメラの軌道高さ
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
	hWhiteScreenPict_ = Image::Load("UI/WhiteScreen.png");
	assert(hWhiteScreenPict_ >= 0);
	Image::SetAlpha(hWhiteScreenPict_, 128);

	stageSelectPictTrs_.position_ = { 0.0f, 0.7f, 0.0f }; // ステージ選択画面の画像の位置
	stageSelectPictTrs_.scale_ = { 0.5f, 0.5f, 0.0f }; // ステージ選択画面の画像のスケール
	stageFrameTrs_.scale_ = { 0.43f, 0.43f, 0.0f }; // ステージフレームのスケール
	explUITrs_.position_ = { 0.0f, -0.7f, 0.0f }; // 操作・ルール説明UIの位置
	explUITrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // 操作・ルール説明UIのスケール

	//ステージデータを読み込む
	CsvReader* csvStage = new CsvReader("CSV/StageData/StageSelect.csv");
	int lines = csvStage->GetLines();
	//ステージデータに基づいてオブジェクトを生成
	for (int y = 1; y < lines; y++) {
		std::string objectName = csvStage->GetString(y, 0);
		XMFLOAT3 position = { csvStage->GetFloat(y, 1), csvStage->GetFloat(y, 2), csvStage->GetFloat(y, 3) };
		XMFLOAT3 rotation = { csvStage->GetFloat(y, 4), csvStage->GetFloat(y, 5), csvStage->GetFloat(y, 6) };
		XMFLOAT3 scale = { csvStage->GetFloat(y, 7), csvStage->GetFloat(y, 8), csvStage->GetFloat(y, 9) };

		if (objectName == "Stage") {
			Stage* pStage = Instantiate<Stage>(this);
			pStage->SetTransformFloat3(position, rotation, scale);
		}
		else if (objectName == "PlayerAndLine") {
			PlayerAndLine* pPandL = Instantiate<PlayerAndLine>(this);
			pPandL->SetTransformFloat3(position, rotation, scale);
		}
		else if (objectName == "Ball") {
			Ball* pBall = Instantiate<Ball>(this);
			pBall->SetTransformFloat3(position, rotation, scale);
		}
		else if (objectName == "Wall") {
			Wall* pWall = Instantiate<Wall>(this);
			pWall->SetTransformFloat3(position, rotation, scale);
		}
	}

	//ステージ情報一覧を読み込む
	CsvReader* csvStageInfo = new CsvReader("CSV/StageData/StageInfoList.csv");
	lines = csvStageInfo->GetLines();
	for (int i = 1;i < lines;i++) {//1行目はヘッダーなのでスキップ
		stageInfos_.push_back(StageInfo());
		//0-indexから始まるので、i-1で調整
		int stageIndex = i - 1;
		//ステージファイル名を取得
		std::string stageFileName = csvStageInfo->GetString(i, 0);
		if (stageFileName == "StageNothing") {
			stageInfos_[stageIndex].fileName_ = "null";
		}
		stageInfos_[stageIndex].fileName_ = "CSV/StageData/" + stageFileName + ".csv";
		//ステージのテーマ画像とステージ画像をロード
		stageInfos_[stageIndex].hThemePict_ = Image::Load("UI/Theme/" + csvStageInfo->GetString(i, 1) + ".png");
		assert(stageInfos_[stageIndex].hThemePict_ >= 0);
		stageInfos_[stageIndex].hStagePict_ = Image::Load("UI/StageImage/" + csvStageInfo->GetString(i, 2) + ".png");
		assert(stageInfos_[stageIndex].hStagePict_ >= 0);
		//ステージ画像の表示位置を計算
		stageInfos_[stageIndex].stageImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].stageImgTrs_.position_.y = 0;
		stageInfos_[stageIndex].stageImgTrs_.scale_ = { 0.2f, 0.2f, 0.0f }; // ステージ画像のスケール
		//お題画像の表示位置を計算
		stageInfos_[stageIndex].themeImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].themeImgTrs_.position_.y = -THEME_IMAGE_Y_OFFSET;
		stageInfos_[stageIndex].themeImgTrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // お題画像のスケール
		stageInfos_[stageIndex].isSelected_ = false; // 初期は選択されていない
	}
	stageInfos_[0].isSelected_ = true; // 最初のステージを選択状態にする
	isExplUISelected_ = false; // 操作・ルール説明UIは初期は選択されていない

	//タイマーの初期化
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(60);
	//カメラの軌道制御オブジェクトの初期化
	pCameraOrbit_ = Instantiate<CameraOrbit>(this);
	pCameraOrbit_->SetOrbit({ 0.0f, CAMERA_ORBIT_HEIGHT, 0.0f }, CAMERA_ORBIT_RADIUS, CAMERA_ORBIT_SPEED);
}

void StageSelectScene::Update()
{
	if (cdTimer_ == nullptr) return; // タイマーが初期化されていない場合は何もしない
	if (cdTimer_->IsTimeOver()) {
		cdTimer_->ResetTimer();
	}
	deltaTime_ = cdTimer_->GetDeltaTime();
	if (pCameraOrbit_ != nullptr)
		pCameraOrbit_->Update(deltaTime_);

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
	//子オブジェクトの描画順をソート
	SortChildrenByRenderOrder();

	//子オブジェクトの描画
	DrawChildren();

	Image::SetTransform(hWhiteScreenPict_, transform_);
	Image::Draw(hWhiteScreenPict_); // 白い画面を描画

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
