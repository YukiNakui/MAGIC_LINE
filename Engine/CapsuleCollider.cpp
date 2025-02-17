#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

// �R���X�g���N�^
CapsuleCollider::CapsuleCollider(XMFLOAT3 start, XMFLOAT3 end, float radius)
{
    start_ = start;  // �J�v�Z���̎n�_
    end_ = end;      // �J�v�Z���̏I�_
    radius_ = radius;
    type_ = COLLIDER_CAPSULE;

    // �f�o�b�O���ɃJ�v�Z���̔���g��\��
#ifdef _DEBUG
    hDebugModel_ = Model::Load("DebugCollision/CapsuleCollider.fbx");
#endif
}

// �J�v�Z�����m�̓����蔻��
bool CapsuleCollider::IsHit(Collider* target)
{
    if (target->type_ == COLLIDER_BOX)
        return IsHitCapsuleVsBox(this, (BoxCollider*)target);
    else if (target->type_ == COLLIDER_CIRCLE)
        return IsHitCapsuleVsCircle(this, (SphereCollider*)target);
    else if (target->type_ == COLLIDER_CAPSULE)
        return IsHitCapsuleVsCapsule(this, (CapsuleCollider*)target);
    return false;
}
