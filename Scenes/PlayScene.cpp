#include "PlayScene.h"
#include "../Objects/Player.h"
#include"../Objects/Stage.h"
#include"../Objects/Ball.h"
#include"../Engine/CsvReader.h"
#include"../Wall.h"
#include"../Objects/Line.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), hModel_(-1)
{
}

void PlayScene::Initialize()
{
	CsvReader* csvStage = new CsvReader("CSV/StageData/stage01.csv");
	int lines = csvStage->GetLines();//�s��

    for (int y = 1; y < lines; y++) { // 1�s�ڂ̓w�b�_�[
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
	lines = csvUI->GetLines();//�s��
	for (int y = 1; y < lines; y++) { // 1�s�ڂ̓w�b�_�[
		std::string uiName = csvUI->GetString(y, 0);
		int kind = csvUI->GetInt(y, 1);
		XMFLOAT3 position = { csvUI->GetFloat(y, 2), csvUI->GetFloat(y, 3), csvUI->GetFloat(y, 4) };
		XMFLOAT3 rotation = { csvUI->GetFloat(y, 5), csvUI->GetFloat(y, 6), csvUI->GetFloat(y, 7) };
		XMFLOAT3 scale = { csvUI->GetFloat(y, 8), csvUI->GetFloat(y, 9), csvUI->GetFloat(y, 10) };
		if (uiName == "PlayUI") {
			if (pPlayUI_ == nullptr)
				pPlayUI_ = Instantiate<PlayUI>(this);
			pPlayUI_->SetPlayUITransform((PlayUI::PlayUIType)kind, position, rotation, scale);
		}
		else if (uiName == "Compass") {
			if (pCompass_ == nullptr)
				pCompass_ = Instantiate<Compass>(this);
			pCompass_->SetCompassUITransform((Compass::CompassUIType)kind,position, rotation, scale);
		}
		else if (uiName == "Arrow") {
			if (pArrow_ == nullptr)
				pArrow_ = Instantiate<Arrow>(this);
			pArrow_->SetTransformFloat3(position, rotation, scale);
		}
		else if (uiName == "LineGauge") {
			if (pLineGauge_ == nullptr)
				pLineGauge_ = Instantiate<LineGauge>(this);
			pLineGauge_->SetLineGaugeUITransform((LineGauge::LineGaugeUIType)kind, position, rotation, scale);
		}
		else if (uiName == "HeightMeter") {
			if (pHeightMeter_ == nullptr)
				pHeightMeter_ = Instantiate<HeightMeter>(this);
			pHeightMeter_->SetHeightMeterUITransform((HeightMeter::HeightMeterUIType)kind, position, rotation, scale);
		}
		else if (uiName == "ThemeDisplay") {
			if (pThemeDisplay_ == nullptr)
				pThemeDisplay_ = Instantiate<ThemeDisplay>(this);
			pThemeDisplay_->SetThemeDisplayTransform((ThemeDisplay::ThemeDisplayType)kind, position, rotation, scale);
		}
		else if (uiName == "MiniMap") {
			if (pMiniMap_ == nullptr)
				pMiniMap_ = Instantiate<MiniMap>(this);
			pMiniMap_->SetMiniMapUITransform((MiniMap::MiniMapUIType)kind, position, rotation, scale);
		}
	}
}

void PlayScene::Update()
{
	//�v���C���[�̏�Ԏ擾
	Player* pPlayer = (Player*)FindObject("Player");
	if (!pPlayer) return;
	int playerState = pPlayer->GetPlayerState();

	if (playerState == 0 && pThemeDisplay_->IsStartVisible()) {
		pPlayUI_->SetDisplay(true);
		pCompass_->SetDisplay(true);
		pArrow_->SetDisplay(true);
		pLineGauge_->SetDisplay(true);
		pHeightMeter_->SetDisplay(true);
		pMiniMap_->SetDisplay(true);
	}
	else if (playerState == 1) {
		pPlayUI_->SetDisplay(true);
		pCompass_->SetDisplay(true);
		pArrow_->SetDisplay(true);
		pLineGauge_->SetDisplay(true);
		pHeightMeter_->SetDisplay(true);
		pMiniMap_->SetDisplay(true);
	}
	else if (playerState == 2) {
		pPlayUI_->SetDisplay(false);
		pCompass_->SetDisplay(false);
		pArrow_->SetDisplay(false);
		pLineGauge_->SetDisplay(false);
		pHeightMeter_->SetDisplay(false);
		pMiniMap_->SetDisplay(false);
	}
}

void PlayScene::Draw()
{
	Player* pPlayer = (Player*)FindObject("Player");
	const auto& capsuleList = pPlayer->GetCapsuleList();
	for (auto& capsule : capsuleList) {
		capsule->Draw();
	}


	//UI�̕`��
	if (pPlayUI_) pPlayUI_->Draw();
	if (pCompass_) pCompass_->Draw();
	if (pArrow_) pArrow_->Draw();
	if (pLineGauge_) pLineGauge_->Draw();
	if (pHeightMeter_) pHeightMeter_->Draw();
	if (pThemeDisplay_) pThemeDisplay_->Draw();
	if (pMiniMap_) pMiniMap_->Draw();
}

void PlayScene::Release()
{
}
