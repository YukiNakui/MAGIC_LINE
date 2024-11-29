#include "PlayerCamera.h"
#include"Player.h"

PlayerCamera::PlayerCamera(GameObject* parent)
	:GameObject(parent,"PlayerCamera")
{
}

void PlayerCamera::Initialize()
{
	Player* pPlayer = GetParent()->FindGameObject<Player>();
	XMVECTOR pPos = pPlayer->GetPosition();//�v���C���[�̈ʒu
	float rotY = pPlayer->GetRotate().y;//�v���C���[��Y��]
	//�����lposition�́A�v���C���[����(0,200,-300),lookTarget�̓v���C���[����(0,100,300)
	position = VGet(0, 200, -300) * MGetRotY(rotY) + pPos;
	lookTarget = VGet(0, 100, 300) * MGetRotY(rotY) + pPos;
	rotation = VGet(0, 0, 0);
}

void PlayerCamera::Update()
{
}

void PlayerCamera::Draw()
{
}

void PlayerCamera::Release()
{
}
