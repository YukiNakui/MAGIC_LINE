#pragma once
#include "../Engine/GameObject.h"

class Line :
    public GameObject
{
	int hModel_;
public:
	Line(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	int GetModelHandle() { return hModel_; }




    // �V���h�E�}�b�v�`��
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;
private:
    float radius_;    //���C���̔��a
    XMFLOAT3 start_;  //���C���̎n�_
    XMFLOAT3 end_;    //���C���̏I�_
    bool isActive_;   //���C�����A�N�e�B�u���ǂ���

public:
    //���C���̔��a���擾
    float GetRadius() { return radius_; }
    //���C���̎n�_���擾
    XMFLOAT3 GetStartPos() { return start_; }
    //���C���̏I�_���擾
    XMFLOAT3 GetEndPos() { return end_; }
    //���C���̃A�N�e�B�u��Ԃ�ݒ�
    void SetActive(bool active) { isActive_ = active; }
    //���C�����A�N�e�B�u���ǂ����𔻒�
    bool IsActive() const { return isActive_; }
};

