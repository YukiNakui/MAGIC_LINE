#include "MiniMap.h"
#include"../Engine/Image.h"
#include"../Objects/Player.h"
#include"../Objects/Ball.h"
#include"../Objects/Line.h"

namespace {
	const float CORRECT_POSITION_X = 0.0025f; //X���W�̕␳�W��
	const float CORRECT_POSITION_Y = 0.003f;  //Y���W�̕␳�W��
}

MiniMap::MiniMap(GameObject* parent)
	: GameObject(parent, "MiniMap"), hMiniMapFramePict_(-1), hMiniMapBallPict_(-1), hMiniMapPlayerPict_(-1), hMiniMapLinePict_(-1),
	isDisplay_(false)
{
}

void MiniMap::Initialize()
{
	//�摜�̃��[�h
	hMiniMapFramePict_ = Image::Load("UI/MiniMap/MiniMapFrame.png");
	assert(hMiniMapFramePict_ >= 0);
	hMiniMapBallPict_ = Image::Load("UI/MiniMap/MiniMapBall.png");
	assert(hMiniMapBallPict_ >= 0);
	hMiniMapPlayerPict_ = Image::Load("UI/MiniMap/MiniMapPlayer.png");
	assert(hMiniMapPlayerPict_ >= 0);
	hMiniMapLinePict_ = Image::Load("UI/MiniMap/MiniMapLine.png");
	assert(hMiniMapLinePict_ >= 0);
}

void MiniMap::Update()
{
	//�v���C���[�I�u�W�F�N�g���擾
	Player* pPlayer = (Player*)FindObject("Player");
	
	//�{�[���I�u�W�F�N�g���擾
	Ball* pBall = (Ball*)FindObject("Ball");
	
	//�v���C���[�̈ʒu�Ɖ�]���~�j�}�b�v�ɔ��f
	if (pPlayer != nullptr) {
		miniMapPlayerTrs_.position_.x = defaultPlayerPosition_.x + pPlayer->GetPosition().x * CORRECT_POSITION_X;
		miniMapPlayerTrs_.position_.y = defaultPlayerPosition_.y + pPlayer->GetPosition().z * CORRECT_POSITION_Y;
		miniMapPlayerTrs_.rotate_.z = -pPlayer->GetRotate().y;
	}
	//�{�[���̈ʒu���~�j�}�b�v�ɔ��f
	if (pBall != nullptr) {
		miniMapBallTrs_.position_.x = defaultBallPosition_.x + pBall->GetPosition().x * CORRECT_POSITION_X;
		miniMapBallTrs_.position_.y = defaultBallPosition_.y + pBall->GetPosition().z * CORRECT_POSITION_Y;
	}

	//�J�v�Z�����X�g���擾���A�~�j�}�b�v��ɔ��f
	const auto& capsuleList = pPlayer->GetCapsuleList();
	miniMapCapsuleTrs_.clear(); //�O��̃J�v�Z�����X�g���N���A
	for (auto& capsule : capsuleList) {
		Transform capsuleTrs;

		//�J�v�Z���̈ʒu�Ɖ�]���~�j�}�b�v�ɔ��f
		capsuleTrs.position_.x = defaultLinePosition_.x + capsule->GetPosition().x * CORRECT_POSITION_X;
		capsuleTrs.position_.y = defaultLinePosition_.y + capsule->GetPosition().z * CORRECT_POSITION_Y;
		capsuleTrs.rotate_.x = -capsule->GetRotate().x;
		capsuleTrs.rotate_.z = -capsule->GetRotate().y;

		//�X�P�[����ݒ�
		capsuleTrs.scale_ = miniMapLineTrs_.scale_;

		//�J�v�Z����Transform�����X�g�ɒǉ�
		miniMapCapsuleTrs_.push_back(capsuleTrs);
	}
}

void MiniMap::Draw()
{
	if (!isDisplay_) return;
	Image::SetTransform(hMiniMapFramePict_, miniMapFrameTrs_);
	Image::Draw(hMiniMapFramePict_);

	for (auto& capsuleTrs : miniMapCapsuleTrs_) {
		Image::SetTransform(hMiniMapLinePict_, capsuleTrs);
		Image::Draw(hMiniMapLinePict_);
	}

	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall != nullptr) {
		Image::SetTransform(hMiniMapBallPict_, miniMapBallTrs_);
		Image::Draw(hMiniMapBallPict_);
	}
	Image::SetTransform(hMiniMapPlayerPict_, miniMapPlayerTrs_);
	Image::Draw(hMiniMapPlayerPict_);
}

void MiniMap::Release()
{
}
