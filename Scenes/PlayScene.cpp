#include "PlayScene.h"
#include "../Objects/Player.h"
#include"../Objects/Stage.h"
#include"../Objects/Ball.h"
#include"../Engine/CsvReader.h"
#include"../Wall.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
{
}

void PlayScene::Initialize()
{
	CsvReader* csvStage = new CsvReader("CSV/StageData/stage01.csv");
	int lines = csvStage->GetLines();//行数

    for (int y = 1; y < lines; y++) { // 1行目はヘッダー
        std::string objectName = csvStage->GetString(y, 0);
		XMFLOAT3 position = { csvStage->GetFloat(y, 1), csvStage->GetFloat(y, 2), csvStage->GetFloat(y, 3) };
		XMFLOAT3 rotation = { csvStage->GetFloat(y, 4), csvStage->GetFloat(y, 5), csvStage->GetFloat(y, 6) };
		XMFLOAT3 scale = { csvStage->GetFloat(y, 7), csvStage->GetFloat(y, 8), csvStage->GetFloat(y, 9) };

		if (objectName == "Stage") {
			Stage* pStage = Instantiate<Stage>(this);
			pStage->SetTransformFloat3(position, rotation, scale);
		}
        else if (objectName == "Player") {
            Player* pPlayer = Instantiate<Player>(this);
			pPlayer->SetTransformFloat3(position, rotation, scale);
		}
        else if (objectName == "Ball") {
            Ball* pBall = Instantiate<Ball>(this);
			pBall->SetTransformFloat3(position, rotation, scale);
		}
    }
	Instantiate<Wall>(this);

	CsvReader* csvUI = new CsvReader("CSV/UIData/UI.csv");
	lines = csvUI->GetLines();//行数
	for (int y = 1; y < lines; y++) { // 1行目はヘッダー
		std::string uiName = csvUI->GetString(y, 0);
		int kind = csvUI->GetInt(y, 1);
		XMFLOAT3 position = { csvUI->GetFloat(y, 2), csvUI->GetFloat(y, 3), csvUI->GetFloat(y, 4) };
		XMFLOAT3 rotation = { csvUI->GetFloat(y, 5), csvUI->GetFloat(y, 6), csvUI->GetFloat(y, 7) };
		XMFLOAT3 scale = { csvUI->GetFloat(y, 8), csvUI->GetFloat(y, 9), csvUI->GetFloat(y, 10) };
		if (uiName == "PlayUI") {
			pPlayUI_ = Instantiate<PlayUI>(this);
			if (kind == 1)	
				pPlayUI_->SetLineUITransform(position, rotation, scale);
			else if (kind == 2)
				pPlayUI_->SetDeleteUITransform(position, rotation, scale);
			else if (kind == 3)
				pPlayUI_->SetFinishUITransform(position, rotation, scale);
		}
		else if (uiName == "Compass") {
			pCompass_ = Instantiate<Compass>(this);
			if (kind == 1)
				pCompass_->SetCompassFrameUITransform(position, rotation, scale);
			else if (kind == 2)
				pCompass_->SetNorthUITransform(position, rotation, scale);
			else if(kind == 3)
				pCompass_->SetSouthUITransform(position, rotation, scale);
			else if(kind == 4)
				pCompass_->SetEastUITransform(position, rotation, scale);
			else if (kind == 5)
				pCompass_->SetWestUITransform(position, rotation, scale);
		}
	}

	//pPlayUI_ = Instantiate<PlayUI>(this);
	pCompass_ = Instantiate<Compass>(this);
	pArrow_ = Instantiate<Arrow>(this);
	pLineGauge_ = Instantiate<LineGauge>(this);
	pHeightMeter_ = Instantiate<HeightMeter>(this);
	pThemeDisplay_ = Instantiate<ThemeDisplay>(this);
}

void PlayScene::Update()
{
	//プレイヤーの状態取得
	Player* pPlayer = (Player*)FindObject("Player");
	if (!pPlayer) return;
	int playerState = pPlayer->GetPlayerState();

	if (playerState == 0 && pThemeDisplay_->IsStartVisible()) {
		pPlayUI_->SetDisplay(true);
		pCompass_->SetDisplay(true);
		pArrow_->SetDisplay(true);
		pLineGauge_->SetDisplay(true);
		pHeightMeter_->SetDisplay(true);
	}
	else if (playerState == 1) {
		pPlayUI_->SetDisplay(true);
		pCompass_->SetDisplay(true);
		pArrow_->SetDisplay(true);
		pLineGauge_->SetDisplay(true);
		pHeightMeter_->SetDisplay(true);
	}
	else if (playerState == 2) {
		pPlayUI_->SetDisplay(false);
		pCompass_->SetDisplay(false);
		pArrow_->SetDisplay(false);
		pLineGauge_->SetDisplay(false);
		pHeightMeter_->SetDisplay(false);
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
