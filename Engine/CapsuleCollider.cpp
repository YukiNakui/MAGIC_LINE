#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

CapsuleCollider::CapsuleCollider(XMFLOAT3 center, XMFLOAT3 start, XMFLOAT3 end, float radius)
{
    center_ = center;//�J�v�Z���̒��S�ʒu
    size_ = XMFLOAT3(radius, radius, radius);
    start_ = start;  // �J�v�Z���̎n�_
    end_ = end;      // �J�v�Z���̏I�_
    radius_ = radius;//�J�v�Z���̔��a
    type_ = COLLIDER_CAPSULE;

    // �f�o�b�O���ɃJ�v�Z���̔���g��\��
#ifdef _DEBUG
    hDebugModel_ = Model::Load("DebugCollision/CapsuleCollider.fbx");
#endif
}


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
