#pragma once
#include "Engine/GameObject.h"
class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //StageSelect画像のハンドル
	int hStagePict_{}; //ステージ画像のハンドル
	int hStageFrameGrayPict_; //ステージフレーム画像(灰色)のハンドル
	int hStageFrameYellowPict_; //ステージフレーム画像(黄色)のハンドル
	int hExplUIGrayPict_; //操作・ルール説明画像(灰色)のハンドル
	int hExplUIYellowPict_; //操作・ルール説明画像(黄色)のハンドル
	std::string stageFileNames{}; // ステージファイル名の配列
public:
	StageSelectScene(GameObject* parent);
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//終了処理
	void Release() override;
};

