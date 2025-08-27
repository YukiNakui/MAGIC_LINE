#include "Torus.h"
#include"Engine/Model.h"


Torus::Torus(GameObject* parent)
	:GameObject(parent, "Torus"), hModel_(-1)
{
}

void Torus::Initialize()
{
	hModel_ = Model::Load("Models/Objects/Ring.fbx");
	assert(hModel_ >= 0);

	mainRadius_ = 1.0f; // �g�[���X�̒��S����`���[�u�̒��S�܂ł̋���(�h�[�i�c�̌��̑傫���{�`���[�u�̔��a)
	tubeRadius_ = 0.1f; // �g�[���X�̃`���[�u�̔��a(�h�[�i�c�̑���)
	torusAxis_ = { 0.0f, 1.0f, 0.0f }; // �g�[���X�̎�����(���̋󂢂Ă������)
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

bool Torus::CheckHitTorusToSphere(const Transform& sphereTrans, float sphereRadius)
{
    // ���̒��S����g�[���X���S�ւ̃x�N�g��
    XMFLOAT3 centerToSphere = {
        sphereTrans.position_.x - transform_.position_.x,
        sphereTrans.position_.y - transform_.position_.y,
        sphereTrans.position_.z - transform_.position_.z
    };
    XMVECTOR v = XMLoadFloat3(&centerToSphere);

    // �g�[���X���x�N�g���i���K���j
    XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torusAxis_));

    // v ���������ɓ��e�i��]�ʂɗ��Ƃ��j
    float dot = XMVectorGetX(XMVector3Dot(v, axis));
    XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot)); // ��]�ʏ�

    float d = XMVectorGetX(XMVector3Length(v_proj)); // ��]�ʏ�ł̋���

    if (d == 0.0f) {
        // ��������Ɉʒu����ꍇ
        //float holeRadius = mainRadius_ - tubeRadius_;
        //if (sphereRadius < holeRadius) {
        //    return false; // ����苅���������������炸�ɒʂ蔲����
        //}
        return false;
    }//�Ȃ����{�[��������Ɉʒu����Ƃ��Ƀ{�[�����g�[���X�ɓ�����ʒu�܂ŏu�Ԉړ����čd������(������y����̂Ƃ��ɋN����)
	//����ɂ��鎞��false�ɂȂ�悤�ɂ��Ȃ��Əu�Ԉړ�����݂����A�ł�false�ɂȂ�悤�ɂ���ƃg�[���X�̌���ʂ蔲����


    // ��]�~��̍ŋߐړ_�i�g�[���X�ǒ��S�j
    XMVECTOR nearestOnCircle = XMVectorScale(XMVector3Normalize(v_proj), mainRadius_);

    // ���̒��S�ƍŋߐړ_�̋���
    XMVECTOR diff = XMVectorSubtract(v_proj, nearestOnCircle);
    float distToTubeCenter = XMVectorGetX(XMVector3Length(diff));

    // ����F�ǔ��a�{�����a�ȓ��Ȃ瓖����
    return distToTubeCenter <= (tubeRadius_ + sphereRadius);
}


