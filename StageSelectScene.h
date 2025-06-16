#pragma once
#include "Engine/GameObject.h"
class StageSelectScene :
    public GameObject
{
	int hBGPict_; //背景画像のハンドル
	int hStagePict_; //ステージ画像のハンドル
	
public:
	StageSelectScene();
	~StageSelectScene();
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//終了処理
	void Release() override;

private:

};

