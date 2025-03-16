#include "Capsule.h"
#include"Engine/Model.h"

Capsule::Capsule(GameObject* parent)
	:GameObject(parent,"Capsule"),hModel_(-1),radius_(1.0f),start_({0,0,1}),end_({0,0,-1}),isActive_(false)
{
}

void Capsule::Initialize()
{
	hModel_ = Model::Load("Model/capsule.fbx");
	assert(hModel_ >= 0);
}

void Capsule::Update()
{
	//�J�v�Z���̉�]�s���K�p���āA�n�_�ƏI�_���X�V
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(transform_.rotate_.x),
		XMConvertToRadians(transform_.rotate_.y),
		XMConvertToRadians(transform_.rotate_.z)
	);

	//���̎n�_�E�I�_����]�s��ŕϊ�
	XMFLOAT3 startOringin = {0,0,1};
	XMFLOAT3 endOringin = { 0,0,-1 };
	XMVECTOR originalStart = XMLoadFloat3(&startOringin); //�����̎n�_
	XMVECTOR originalEnd = XMLoadFloat3(&endOringin); //�����̏I�_

	XMVECTOR rotatedStart = XMVector3Transform(originalStart, rotationMatrix);
	XMVECTOR rotatedEnd = XMVector3Transform(originalEnd, rotationMatrix);

	//�ϊ���̍��W��ۑ�
	XMStoreFloat3(&start_, rotatedStart);
	XMStoreFloat3(&end_, rotatedEnd);

	//���[���h���W�ɓK�p
	XMVECTOR worldPos = XMLoadFloat3(&transform_.position_);
	XMStoreFloat3(&start_, XMVectorAdd(worldPos, rotatedStart));
	XMStoreFloat3(&end_, XMVectorAdd(worldPos, rotatedEnd));
}

void Capsule::Draw()
{
	if (!IsActive()) return; // ��A�N�e�B�u�Ȃ�`�悵�Ȃ�

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Capsule::Release()
{
}
