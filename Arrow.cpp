#include "Arrow.h"
#include"Engine/Image.h"
#include"Engine/Model.h"
#include"Ball.h"
#include"Player.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), hModel_(-1),
	targetPos_({ 0.0f,0.0f,0.0f }), playerPos_({ 0.0f,0.0f,0.0f }),
	directionVec_({ 0.0f,0.0f,0.0f }), playerPosVec_({ 0.0f,0.0f,0.0f }),targetPosVec_({ 0.0f,0.0f,0.0f })
{
}

void Arrow::Initialize()
{
	hModel_ = Model::Load("Model/SquarePyramid.fbx");
	assert(hModel_ >= 0);
	transform_.scale_ = { 2.0f,2.0f,2.0f };
}

void Arrow::Update()
{
	Ball* ball = (Ball*)FindObject("Ball");
	if (!ball) return;
	targetPos_ = ball->GetPosition();
	Player* player = (Player*)FindObject("Player");
	if (!player) return;
	playerPos_ = player->GetPosition();

	targetPosVec_ = XMLoadFloat3(&targetPos_);
	playerPosVec_ = XMLoadFloat3(&playerPos_);
	directionVec_ = XMVector3Normalize(targetPosVec_ - playerPosVec_);

	//�����v���C���[����{�[���ւ̈ʒu�Ɉړ�
	XMStoreFloat3(&transform_.position_, playerPosVec_ + directionVec_ * 5.0f);

	//���̌������{�[���̕����Ɍ�����
	//�O�����̃x�N�g���ƃv���C���[����{�[���ւ̃x�N�g���̊O�ς��v�Z
	XMVECTOR forwardVec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f );
	XMVECTOR cross = XMVector3Cross(forwardVec, directionVec_);
	// �v���C���[����{�[���ւ̕����x�N�g���ƑO���x�N�g���̊Ԃŉ�]�����v�Z
}

void Arrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
