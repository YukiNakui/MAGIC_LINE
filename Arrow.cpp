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

	//矢印をプレイヤーからボールへの位置に移動
	XMStoreFloat3(&transform_.position_, playerPosVec_ + directionVec_ * 5.0f);

	//矢印の向きをボールの方向に向ける
	//前方向のベクトルとプレイヤーからボールへのベクトルの外積を計算
	XMVECTOR forwardVec = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f );
	XMVECTOR cross = XMVector3Cross(forwardVec, directionVec_);
	// プレイヤーからボールへの方向ベクトルと前方ベクトルの間で回転軸を計算
}

void Arrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
