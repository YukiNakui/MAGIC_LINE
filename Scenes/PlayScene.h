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

//�v���C�V�[��
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
    //�Q�[���I�u�W�F�N�g
	Player* pPlayer_;   //�v���C���[

    //UI�v�f
    LineGauge* pLineGauge_;          //���C���c��UI
    ThemeDisplay* pThemeDisplay_;   //����\��UI
    PlayUI* pPlayUI_;               //������@UI
    Compass* pCompass_;             //�R���p�XUI
    Arrow* pArrow_;                 //���UI
    HeightMeter* pHeightMeter_;     //���x�vUI
    MiniMap* pMiniMap_;             //�~�j�}�b�v

    //�X�e�[�W�t�@�C���֘A
    std::string stageFileName_;
};

