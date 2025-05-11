#pragma once
#include "../Engine/GameObject.h"
#include<vector>

class MiniMap :
    public GameObject
{
	//�~�j�}�b�v��UI�v�f�摜�̃n���h��
	int hMiniMapFramePict_;  //�~�j�}�b�v�t���[���摜�n���h��
	int hMiniMapBallPict_;   //�~�j�}�b�v�{�[���摜�n���h��
	int hMiniMapPlayerPict_; //�~�j�}�b�v�v���C���[�摜�n���h��
	int hMiniMapLinePict_;   //�~�j�}�b�v���C���摜�n���h��

	//�eUI�v�f��Transform���
	Transform miniMapFrameTrs_;  //�~�j�}�b�v�t���[����Transform
	Transform miniMapBallTrs_;   //�~�j�}�b�v�{�[����Transform
	Transform miniMapPlayerTrs_; //�~�j�}�b�v�v���C���[��Transform
	Transform miniMapLineTrs_;   //�~�j�}�b�v���C����Transform

	//�~�j�}�b�v�ɕ\�������J�v�Z����Transform���X�g
	std::vector<Transform> miniMapCapsuleTrs_;

	//�f�t�H���g�̈ʒu���
	XMFLOAT3 defaultPlayerPosition_; //�v���C���[�̃f�t�H���g�ʒu
	XMFLOAT3 defaultBallPosition_;   //�{�[���̃f�t�H���g�ʒu
	XMFLOAT3 defaultLinePosition_;   //���C���̃f�t�H���g�ʒu
public:
	MiniMap(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�~�j�}�b�vUI�̎��
	enum MiniMapUIType {
		MiniMapFrame = 1, //�~�j�}�b�v�t���[��
		MiniMapBall,      //�~�j�}�b�v�{�[��
		MiniMapPlayer,    //�~�j�}�b�v�v���C���[
		MiniMapLine       //�~�j�}�b�v���C��
	};

	void SetMiniMapFrameTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapFrameTrs_.position_ = position;
		miniMapFrameTrs_.rotate_ = rotate;
		miniMapFrameTrs_.scale_ = scale;
	}

	void SetMiniMapBallTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapBallTrs_.position_ = position;
		miniMapBallTrs_.rotate_ = rotate;
		miniMapBallTrs_.scale_ = scale;
		defaultBallPosition_ = position;
	}

	void SetMiniMapPlayerTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapPlayerTrs_.position_ = position;
		miniMapPlayerTrs_.rotate_ = rotate;
		miniMapPlayerTrs_.scale_ = scale;
		defaultPlayerPosition_ = position;
	}

	void SetMiniMapLineTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapLineTrs_.position_ = position;
		miniMapLineTrs_.rotate_ = rotate;
		miniMapLineTrs_.scale_ = scale;
		defaultLinePosition_ = position;
	}

	//�~�j�}�b�vUI��Transform��ݒ肷��֐�
	void SetMiniMapUITransform(MiniMapUIType miniMapUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (miniMapUIType) {
		case MiniMapFrame:
			SetMiniMapFrameTransform(position, rotate, scale);
			break;
		case MiniMapBall:
			SetMiniMapBallTransform(position, rotate, scale);
			break;
		case MiniMapPlayer:
			SetMiniMapPlayerTransform(position, rotate, scale);
			break;
		case MiniMapLine:
			SetMiniMapLineTransform(position, rotate, scale);
			break;
		default:
			break;
		}
	}


private:
	bool isDisplay_; //�~�j�}�b�v�̕\���t���O
public:
	//�~�j�}�b�v�̕\����Ԃ��擾����֐�
	void SetDisplay(bool display) { isDisplay_ = display; }
};