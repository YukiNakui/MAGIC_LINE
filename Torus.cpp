#include "Torus.h"
#include"Engine/Model.h"

namespace {
	const float DEFAULT_MAIN_RADIUS = 1.0f; // �f�t�H���g�̃g�[���X�̒��S����`���[�u�̒��S�܂ł̋���(�h�[�i�c�̌��̑傫���{�`���[�u�̔��a)
	const float DEFAULT_TUBE_RADIUS = 0.2f; // �f�t�H���g�̃g�[���X�̃`���[�u�̔��a(�h�[�i�c�̑���)
	const XMFLOAT3 DEFAULT_AXIS = { 0.0f, 1.0f, 0.0f }; // �f�t�H���g�̃g�[���X�̎�����(���̋󂢂Ă������)
}


Torus::Torus(GameObject* parent)
	:GameObject(parent, "Torus"), hModel_(-1)
{
}

void Torus::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Torus.fbx");
	assert(hModel_ >= 0);

	mainRadius_ = DEFAULT_MAIN_RADIUS;
	tubeRadius_ = DEFAULT_TUBE_RADIUS;
	torusAxis_ = DEFAULT_AXIS;
}

void Torus::Update()
{
}

void Torus::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Torus::Release()
{
}

void Torus::SetCollider(XMFLOAT3 rotation, XMFLOAT3 scale)
{
	mainRadius_ = DEFAULT_MAIN_RADIUS * scale.x; // �X�P�[���Ɋ�Â��ă��C�����a�𒲐�
	tubeRadius_ = DEFAULT_TUBE_RADIUS * scale.y; // �X�P�[���Ɋ�Â��ă`���[�u���a�𒲐�
    //���f���̉�]�ɉ����Ď�����]������
	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
    XMVECTOR axis = XMVector3TransformNormal(XMLoadFloat3(&DEFAULT_AXIS), rotMatrix);
	XMStoreFloat3(&torusAxis_, axis);
}

bool Torus::CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius)
{
    XMFLOAT3 centerToSphere = {
       sphereTrans.position_.x - transform_.position_.x,
       sphereTrans.position_.y - transform_.position_.y,
       sphereTrans.position_.z - transform_.position_.z
    };
    XMVECTOR v = XMLoadFloat3(&centerToSphere);
    XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torusAxis_));
    float dot = XMVectorGetX(XMVector3Dot(v, axis));
    XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot));
    float d = XMVectorGetX(XMVector3Length(v_proj));
    float holeRadius = mainRadius_ - tubeRadius_;

    // �Ǖ\�ʂƂ̏Փ˔���i����ȊO�ł���ɗD��j
    XMVECTOR nearestOnCircle = XMVectorScale(XMVector3Normalize(v_proj), mainRadius_);
    XMVECTOR tubeCenter = XMVectorAdd(XMLoadFloat3(&transform_.position_), nearestOnCircle);
    XMVECTOR sphereToTube = XMVectorSubtract(XMLoadFloat3(&sphereTrans.position_), tubeCenter);
    float distToTubeCenter = XMVectorGetX(XMVector3Length(sphereToTube));
    if (distToTubeCenter <= (tubeRadius_ + sphereRadius)) {
        return true;
    }

    return false;
}


