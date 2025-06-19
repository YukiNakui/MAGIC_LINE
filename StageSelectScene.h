#pragma once
#include "Engine/GameObject.h"
class StageSelectScene :
	public GameObject
{
	int hStageSelectPict_; //StageSelect�摜�̃n���h��
	int hStagePict_{}; //�X�e�[�W�摜�̃n���h��
	int hStageFrameGrayPict_; //�X�e�[�W�t���[���摜(�D�F)�̃n���h��
	int hStageFrameYellowPict_; //�X�e�[�W�t���[���摜(���F)�̃n���h��
	int hExplUIGrayPict_; //����E���[�������摜(�D�F)�̃n���h��
	int hExplUIYellowPict_; //����E���[�������摜(���F)�̃n���h��
	std::string stageFileNames{}; // �X�e�[�W�t�@�C�����̔z��
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

