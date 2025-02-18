#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

CapsuleCollider::CapsuleCollider(XMFLOAT3 center, XMFLOAT3 start, XMFLOAT3 end, float radius)
{
    center_ = center;//カプセルの中心位置
    size_ = XMFLOAT3(radius, radius, radius);
    start_ = start;  // カプセルの始点
    end_ = end;      // カプセルの終点
    radius_ = radius;//カプセルの半径
    type_ = COLLIDER_CAPSULE;

    // デバッグ時にカプセルの判定枠を表示
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
