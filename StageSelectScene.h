#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include"Functions/CDTimer.h"
#include"Functions/CameraOrbit.h"

class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //�X�e�[�W�Z���N�g�摜�̃n���h��
	int hStageFrameGrayPict_; //�X�e�[�W�t���[���摜(�D�F)�̃n���h��
	int hStageFrameYellowPict_; //�X�e�[�W�t���[���摜(���F)�̃n���h��
	int hExplUIGrayPict_; //����E���[�������摜(�D�F)�̃n���h��
	int hExplUIYellowPict_; //����E���[�������摜(���F)�̃n���h��
	int hWhiteScreenPict_; //������ʂ̃n���h��

	Transform stageSelectPictTrs_; // �X�e�[�W�I����ʂ̉摜�̃g�����X�t�H�[��
	Transform stageFrameTrs_; // �X�e�[�W�t���[���̃g�����X�t�H�[��
	Transform explUITrs_; // ����E���[������UI�̃g�����X�t�H�[��

	//�X�e�[�W�����܂Ƃ߂��\����
	struct StageInfo {
		std::string fileName_; // �X�e�[�W��CSV�t�@�C����
		int hThemePict_; // �X�e�[�W�̃e�[�}�摜�n���h��
		int hStagePict_; // �X�e�[�W�̉摜�n���h��
		Transform stageImgTrs_; // �X�e�[�W�摜�̕\���ʒu
		Transform themeImgTrs_; // ����摜�̕\���ʒu
		bool isSelected_; // �X�e�[�W���I������Ă��邩�ǂ���
	};
	
	int currentStageIndex_; // ���ݑI�𒆂̃X�e�[�W�C���f�b�N�X
	std::vector<StageInfo> stageInfos_; // �X�e�[�W���̃��X�g
	bool isExplUISelected_; // ����E���[������UI���I������Ă��邩

	CDTimer* cdTimer_;         //�^�C�}�[�I�u�W�F�N�g
	float deltaTime_;          //�t���[���Ԃ̌o�ߎ���
	CameraOrbit* pCameraOrbit_;                    //�J�����̋O������I�u�W�F�N�g
public:
	StageSelectScene(GameObject* parent);
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�I������
	void Release() override;
};

