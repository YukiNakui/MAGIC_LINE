#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Model/Player.fbx");
	assert(hModel_ >= 0);
}

void Player::Update()
{
	if (Input::IsKey(DIK_W)) {
		transform_.position_.y += 0.1f;
	}
	if (Input::IsKey(DIK_A)) {
		transform_.position_.x -= 0.1f;
	}
	if (Input::IsKey(DIK_S)) {
		transform_.position_.y -= 0.1f;
	}
	if (Input::IsKey(DIK_D)) {
		transform_.position_.x += 0.1f;
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
}
