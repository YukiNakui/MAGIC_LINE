#pragma once
#include "../Engine/GameObject.h"
#include"../Objects/Player.h"
#include"../UI/LineGauge.h"
#include"../UI/ThemeDisplay.h"
#include"../UI/PlayUI.h"
#include"../UI/Compass.h"
#include"../UI/Arrow.h"
#include"../UI/HeightMeter.h"
#include"../UI/MiniMap.h"

//プレイシーン
class PlayScene : public GameObject
{
	int hModel_;
public:
	PlayScene(GameObject* parent, const std::string& fileName);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

    void SetUIVisibility(bool isVisible);
private:
    //ゲームオブジェクト
	Player* pPlayer_;   //プレイヤー

    //UI要素
    LineGauge* pLineGauge_;          //ライン残量UI
    ThemeDisplay* pThemeDisplay_;   //お題表示UI
    PlayUI* pPlayUI_;               //操作方法UI
    Compass* pCompass_;             //コンパスUI
    Arrow* pArrow_;                 //矢印UI
    HeightMeter* pHeightMeter_;     //高度計UI
    MiniMap* pMiniMap_;             //ミニマップ

    //ステージファイル関連
    std::string stageFileName_;
};

