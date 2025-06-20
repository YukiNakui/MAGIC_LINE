#pragma once
#include "Engine/GameObject.h"
class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //StageSelect画像のハンドル
	int hStageFrameGrayPict_; //ステージフレーム画像(灰色)のハンドル
	int hStageFrameYellowPict_; //ステージフレーム画像(黄色)のハンドル
	int hExplUIGrayPict_; //操作・ルール説明画像(灰色)のハンドル
	int hExplUIYellowPict_; //操作・ルール説明画像(黄色)のハンドル

	//ステージ情報をまとめた構造体
	struct StageInfo {
		std::string fileName; // ステージのCSVファイル名
		int stageNum; // ステージ番号
		int hThemePict; // ステージのテーマ画像ハンドル
		int hStagePict; // ステージの画像ハンドル
		int x, y; // 画像の表示位置
		bool isSelected; // ステージが選択されているかどうか
	};
	
	int currentStageIndex_; // 現在選択中のステージインデックス
	std::vector<StageInfo> stageInfos_; // ステージ情報のリスト
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

