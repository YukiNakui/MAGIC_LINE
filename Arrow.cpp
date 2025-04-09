#include "Arrow.h"
#include"Engine/Image.h"
#include"Engine/Model.h"
#include"Ball.h"
#include"Player.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), hModel_(-1),
	targetPos_({ 0.0f,0.0f,0.0f }), playerPos_({ 0.0f,0.0f,0.0f }),
	direction_({ 0.0f,0.0f,0.0f }), forwardVec_({ 0.0f,0.0f,1.0f }),
	directionVec_({ 0.0f,0.0f,0.0f }), playerPosVec_({ 0.0f,0.0f,0.0f }),targetPosVec_({ 0.0f,0.0f,0.0f })
{
}

void Arrow::Initialize()
{
	hModel_ = Model::Load("Model/SquarePyramid.fbx");
	assert(hModel_ >= 0);
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

	//プレイヤーからボールの方向へモデルが向くように回転
	XMVECTOR axis = XMVector3Cross(forwardVec_, directionVec_);  // 2つのベクトルの外積
	float angle = acosf(XMVectorGetX(XMVector3Dot(forwardVec_, directionVec_))); // 角度計算

}

void Arrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
