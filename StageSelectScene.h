#pragma once
#include "Engine/GameObject.h"
#include <vector>

class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //ステージセレクト画像のハンドル
	int hStageFrameGrayPict_; //ステージフレーム画像(灰色)のハンドル
	int hStageFrameYellowPict_; //ステージフレーム画像(黄色)のハンドル
	int hExplUIGrayPict_; //操作・ルール説明画像(灰色)のハンドル
	int hExplUIYellowPict_; //操作・ルール説明画像(黄色)のハンドル

	//ステージ情報をまとめた構造体
	struct StageInfo {
		std::string fileName; // ステージのCSVファイル名
		int hThemePict; // ステージのテーマ画像ハンドル
		int hStagePict; // ステージの画像ハンドル
		Transform stageImgTrs; // ステージ画像の表示位置
		Transform themeImgTrs; // お題画像の表示位置
		bool isSelected; // ステージが選択されているかどうか
	};
	
	int currentStageIndex_; // 現在選択中のステージインデックス
	std::vector<StageInfo> stageInfos_; // ステージ情報のリスト

	Transform stageSelectPictTrs_; // ステージ選択画面の画像のトランスフォーム
	Transform stageFrameTrs_; // ステージフレームのトランスフォーム
	Transform explUITrs_; // 操作・ルール説明UIのトランスフォーム
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

