#pragma once
#include "../Engine/GameObject.h"
#include"../Functions/CDTimer.h"

class Arrow :
    public GameObject
{
	int hModel_;
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
    XMFLOAT3 targetPos_;    //���̃^�[�Q�b�g�ʒu
    XMFLOAT3 playerPos_;    //�v���C���[�̈ʒu
    XMVECTOR directionVec_; //���̐i�s�����x�N�g��
    XMVECTOR playerPosVec_; //�v���C���x�N�g��
    XMVECTOR targetPosVec_; //�^�[�Q�b�g�x�N�g��
    CDTimer* cdTimer_;      //�^�C�}�[
    bool isDisplay_;        //���̕\�����

public:
    //���̕\����Ԃ�ݒ�
    void SetDisplay(bool display) { isDisplay_ = display; }
};

