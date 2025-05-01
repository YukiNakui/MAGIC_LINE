#include "MiniMap.h"
#include"Engine/Image.h"
#include"Objects/Player.h"
#include"Objects/Ball.h"
#include"Objects/Line.h"

namespace {
	const float CORRECT_POSITION_X = 0.0025f;
	const float CORRECT_POSITION_Y = 0.003f;
}

MiniMap::MiniMap(GameObject* parent)
	: GameObject(parent, "MiniMap"), hMiniMapFramePict_(-1), hMiniMapBallPict_(-1), hMiniMapPlayerPict_(-1), hMiniMapLinePict_(-1),
	isDisplay_(false)
{
}

void MiniMap::Initialize()
{
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
	Player* pPlayer = (Player*)FindObject("Player");
	if (!pPlayer) return;
	Ball* pBall = (Ball*)FindObject("Ball");
	if (!pBall) return;

	miniMapPlayerTrs_.position_.x = defaultPlayerPosition_.x + pPlayer->GetPosition().x * CORRECT_POSITION_X;
	miniMapPlayerTrs_.position_.y = defaultPlayerPosition_.y + pPlayer->GetPosition().z * CORRECT_POSITION_Y;
	miniMapPlayerTrs_.rotate_.z = -pPlayer->GetRotate().y;
	miniMapBallTrs_.position_.x = defaultBallPosition_.x + pBall->GetPosition().x * CORRECT_POSITION_X;
	miniMapBallTrs_.position_.y = defaultBallPosition_.y + pBall->GetPosition().z * CORRECT_POSITION_Y;

	// カプセルリストを取得
	const auto& capsuleList = pPlayer->GetCapsuleList();
	miniMapCapsuleTrs_.clear();
	for (auto& capsule : capsuleList) {
		Transform capsuleTrs;
		capsuleTrs.position_.x = defaultLinePosition_.x + capsule->GetPosition().x * CORRECT_POSITION_X;
		capsuleTrs.position_.y = defaultLinePosition_.y + capsule->GetPosition().z * CORRECT_POSITION_Y;
		capsuleTrs.rotate_.x = -capsule->GetRotate().x;
		capsuleTrs.rotate_.z = -capsule->GetRotate().y;
		capsuleTrs.scale_ = miniMapLineTrs_.scale_;
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

	Image::SetTransform(hMiniMapBallPict_, miniMapBallTrs_);
	Image::Draw(hMiniMapBallPict_);
	Image::SetTransform(hMiniMapPlayerPict_, miniMapPlayerTrs_);
	Image::Draw(hMiniMapPlayerPict_);
}

void MiniMap::Release()
{
}
