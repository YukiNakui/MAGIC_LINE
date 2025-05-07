#pragma once
#include "../Engine/GameObject.h"
#include"../UI/LineGauge.h"
#include"../UI/ThemeDisplay.h"
#include"../UI/PlayUI.h"
#include"../UI/Compass.h"
#include"../UI/Arrow.h"
#include"../UI/HeightMeter.h"
#include"../MiniMap.h"

class PlayScene : public GameObject
{
	int hModel_;
public:
	PlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
	//UI
	LineGauge* pLineGauge_;
	ThemeDisplay* pThemeDisplay_;
	PlayUI* pPlayUI_;
	Compass* pCompass_;
	Arrow* pArrow_;
	HeightMeter* pHeightMeter_;
	MiniMap* pMiniMap_;
};

