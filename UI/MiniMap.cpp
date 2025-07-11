#include "MiniMap.h"
#include"../Engine/Image.h"
#include"../Objects/Player.h"
#include"../Objects/Ball.h"
#include"../Objects/Line.h"

namespace {
	const float CORRECT_POSITION_X = 0.0025f; //X座標の補正係数
	const float CORRECT_POSITION_Y = 0.003f;  //Y座標の補正係数
}

MiniMap::MiniMap(GameObject* parent)
	: GameObject(parent, "MiniMap"), hMiniMapFramePict_(-1), hMiniMapBallPict_(-1), hMiniMapPlayerPict_(-1), hMiniMapLinePict_(-1),
	isDisplay_(false)
{
}

void MiniMap::Initialize()
{
	//画像のロード
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
	//プレイヤーオブジェクトを取得
	Player* pPlayer = (Player*)FindObject("Player");
	
	//ボールオブジェクトを取得
	Ball* pBall = (Ball*)FindObject("Ball");
	
	//プレイヤーの位置と回転をミニマップに反映
	if (pPlayer != nullptr) {
		miniMapPlayerTrs_.position_.x = defaultPlayerPosition_.x + pPlayer->GetPosition().x * CORRECT_POSITION_X;
		miniMapPlayerTrs_.position_.y = defaultPlayerPosition_.y + pPlayer->GetPosition().z * CORRECT_POSITION_Y;
		miniMapPlayerTrs_.rotate_.z = -pPlayer->GetRotate().y;
	}
	//ボールの位置をミニマップに反映
	if (pBall != nullptr) {
		miniMapBallTrs_.position_.x = defaultBallPosition_.x + pBall->GetPosition().x * CORRECT_POSITION_X;
		miniMapBallTrs_.position_.y = defaultBallPosition_.y + pBall->GetPosition().z * CORRECT_POSITION_Y;
	}

	//カプセルリストを取得し、ミニマップ上に反映
	const auto& capsuleList = pPlayer->GetCapsuleList();
	miniMapCapsuleTrs_.clear(); //前回のカプセルリストをクリア
	for (auto& capsule : capsuleList) {
		Transform capsuleTrs;

		//カプセルの位置と回転をミニマップに反映
		capsuleTrs.position_.x = defaultLinePosition_.x + capsule->GetPosition().x * CORRECT_POSITION_X;
		capsuleTrs.position_.y = defaultLinePosition_.y + capsule->GetPosition().z * CORRECT_POSITION_Y;
		capsuleTrs.rotate_.x = -capsule->GetRotate().x;
		capsuleTrs.rotate_.z = -capsule->GetRotate().y;

		//スケールを設定
		capsuleTrs.scale_ = miniMapLineTrs_.scale_;

		//カプセルのTransformをリストに追加
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
