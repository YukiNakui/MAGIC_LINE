#pragma once
#include "Engine/GameObject.h"
class StageSelectScene :
    public GameObject
{
	int hBGPict_; //�w�i�摜�̃n���h��
	int hStagePict_; //�X�e�[�W�摜�̃n���h��
	
public:
	StageSelectScene();
	~StageSelectScene();
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�I������
	void Release() override;

private:

};

