#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include"Functions/CDTimer.h"
#include"Functions/CameraOrbit.h"

class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //ステージセレクト画像のハンドル
	int hStageFrameGrayPict_; //ステージフレーム画像(灰色)のハンドル
	int hStageFrameYellowPict_; //ステージフレーム画像(黄色)のハンドル
	int hExplUIGrayPict_; //操作・ルール説明画像(灰色)のハンドル
	int hExplUIYellowPict_; //操作・ルール説明画像(黄色)のハンドル
	int hWhiteScreenPict_; //白い画面のハンドル

	Transform stageSelectPictTrs_; // ステージ選択画面の画像のトランスフォーム
	Transform stageFrameTrs_; // ステージフレームのトランスフォーム
	Transform explUITrs_; // 操作・ルール説明UIのトランスフォーム

	//ステージ情報をまとめた構造体
	struct StageInfo {
		std::string fileName_; // ステージのCSVファイル名
		int hThemePict_; // ステージのテーマ画像ハンドル
		int hStagePict_; // ステージの画像ハンドル
		Transform stageImgTrs_; // ステージ画像の表示位置
		Transform themeImgTrs_; // お題画像の表示位置
		bool isSelected_; // ステージが選択されているかどうか
	};
	
	int currentStageIndex_; // 現在選択中のステージインデックス
	std::vector<StageInfo> stageInfos_; // ステージ情報のリスト
	bool isExplUISelected_; // 操作・ルール説明UIが選択されているか

	CDTimer* cdTimer_;         //タイマーオブジェクト
	float deltaTime_;          //フレーム間の経過時間
	CameraOrbit* pCameraOrbit_;                    //カメラの軌道制御オブジェクト
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

