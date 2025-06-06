#include "PlayScene.h"
#include "../Objects/Player.h"
#include"../Objects/Stage.h"
#include"../Objects/Ball.h"
#include"../Engine/CsvReader.h"
#include"../Objects/Wall.h"
#include"../Objects/Line.h"
#include"../Engine/VFX.h"
#include"../UI/CountdownNumber.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
{
}

void PlayScene::Initialize()
{
	//ステージデータを読み込む
	CsvReader* csvStage = new CsvReader("CSV/StageData/stage01.csv");
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
			pStage->SetRenderOrder(0);
		}
        else if (objectName == "Player") {
            pPlayer_ = Instantiate<Player>(this);
			pPlayer_->SetTransformFloat3(position, rotation, scale);
			pPlayer_->SetRenderOrder(0);
		}
        else if (objectName == "Ball") {
            Ball* pBall = Instantiate<Ball>(this);
			pBall->SetTransformFloat3(position, rotation, scale);
			pBall->SetRenderOrder(0);
		}
		else if (objectName == "Wall") {
			Wall* pWall = Instantiate<Wall>(this);
			pWall->SetTransformFloat3(position, rotation, scale);
			pWall->SetRenderOrder(0);
		}
    }

	//UIデータを読み込む
	CsvReader* csvUI = new CsvReader("CSV/UIData/UI.csv");
	lines = csvUI->GetLines();

	//UIデータに基づいてオブジェクトを生成
	for (int y = 1; y < lines; y++) {
		std::string uiName = csvUI->GetString(y, 0);
		int kind = csvUI->GetInt(y, 1);
		XMFLOAT3 position = { csvUI->GetFloat(y, 2), csvUI->GetFloat(y, 3), csvUI->GetFloat(y, 4) };
		XMFLOAT3 rotation = { csvUI->GetFloat(y, 5), csvUI->GetFloat(y, 6), csvUI->GetFloat(y, 7) };
		XMFLOAT3 scale = { csvUI->GetFloat(y, 8), csvUI->GetFloat(y, 9), csvUI->GetFloat(y, 10) };
		if (uiName == "PlayUI") {
			if (pPlayUI_ == nullptr)
				pPlayUI_ = Instantiate<PlayUI>(this);
			pPlayUI_->SetPlayUITransform((PlayUI::PlayUIType)kind, position, rotation, scale);
			pPlayUI_->SetRenderOrder(10);
		}
		else if (uiName == "Compass") {
			if (pCompass_ == nullptr)
				pCompass_ = Instantiate<Compass>(this);
			pCompass_->SetCompassUITransform((Compass::CompassUIType)kind,position, rotation, scale);
			pCompass_->SetRenderOrder(10);
		}
		else if (uiName == "Arrow") {
			if (pArrow_ == nullptr)
				pArrow_ = Instantiate<Arrow>(this);
			pArrow_->SetTransformFloat3(position, rotation, scale);
			pArrow_->SetRenderOrder(10);
		}
		else if (uiName == "LineGauge") {
			if (pLineGauge_ == nullptr)
				pLineGauge_ = Instantiate<LineGauge>(this);
			pLineGauge_->SetLineGaugeUITransform((LineGauge::LineGaugeUIType)kind, position, rotation, scale);
			pLineGauge_->SetRenderOrder(10);
		}
		else if (uiName == "HeightMeter") {
			if (pHeightMeter_ == nullptr)
				pHeightMeter_ = Instantiate<HeightMeter>(this);
			pHeightMeter_->SetHeightMeterUITransform((HeightMeter::HeightMeterUIType)kind, position, rotation, scale);
			pHeightMeter_->SetRenderOrder(10);
		}
		else if (uiName == "ThemeDisplay") {
			if (pThemeDisplay_ == nullptr)
				pThemeDisplay_ = Instantiate<ThemeDisplay>(this);
			pThemeDisplay_->SetThemeDisplayTransform((ThemeDisplay::ThemeDisplayType)kind, position, rotation, scale);
			pThemeDisplay_->SetRenderOrder(10);
		}
		else if (uiName == "MiniMap") {
			if (pMiniMap_ == nullptr)
				pMiniMap_ = Instantiate<MiniMap>(this);
			pMiniMap_->SetMiniMapUITransform((MiniMap::MiniMapUIType)kind, position, rotation, scale);
			pMiniMap_->SetRenderOrder(10);
		}
	}
}

void PlayScene::Update()
{
	//プレイヤーの状態取得
	pPlayer_ = (Player*)FindObject("Player");
	if (!pPlayer_) return;
	int playerState = pPlayer_->GetPlayerState();

	CountdownNumber* pCountDown = (CountdownNumber*)FindObject("CountdownNumber");
	if (pCountDown) {
		pCountDown->SetRenderOrder(11);
	}

	//プレイヤーの状態に応じてUIを表示・非表示に切り替え
	if (playerState == 0 && pThemeDisplay_->IsStartVisible()) {
        SetUIVisibility(true);
    }
    else if (playerState == 1) {
        SetUIVisibility(true);
    }
    else if (playerState == 2) {
        SetUIVisibility(false);
    }
}

void PlayScene::Draw()
{
	//子オブジェクトの描画順をソート
	SortChildrenByRenderOrder();

	//子オブジェクトの描画
	DrawChildren();
}

void PlayScene::Release()
{
}

void PlayScene::SetUIVisibility(bool isVisible)
{
	// 全UIコンポーネントの表示・非表示を一括で設定
	if (pPlayUI_) pPlayUI_->SetDisplay(isVisible);
	if (pCompass_) pCompass_->SetDisplay(isVisible);
	if (pArrow_) pArrow_->SetDisplay(isVisible);
	if (pLineGauge_) pLineGauge_->SetDisplay(isVisible);
	if (pHeightMeter_) pHeightMeter_->SetDisplay(isVisible);
	if (pMiniMap_) pMiniMap_->SetDisplay(isVisible);
}
