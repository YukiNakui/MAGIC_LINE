#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include <algorithm>

//コンストラクタ
Collider::Collider():
	pGameObject_(nullptr)
{
}

//デストラクタ
Collider::~Collider()
{
}

//箱型同士の衝突判定
//引数：boxA	１つ目の箱型判定
//引数：boxB	２つ目の箱型判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB)
{
	XMFLOAT3 boxPosA = Transform::Float3Add(boxA->pGameObject_->GetWorldPosition(), boxA->center_);
	XMFLOAT3 boxPosB = Transform::Float3Add(boxB->pGameObject_->GetWorldPosition(), boxB->center_);

	if ((boxPosA.x + boxA->size_.x / 2) > (boxPosB.x - boxB->size_.x / 2) &&
		(boxPosA.x - boxA->size_.x / 2) < (boxPosB.x + boxB->size_.x / 2) &&
		(boxPosA.y + boxA->size_.y / 2) > (boxPosB.y - boxB->size_.y / 2) &&
		(boxPosA.y - boxA->size_.y / 2) < (boxPosB.y + boxB->size_.y / 2) &&
		(boxPosA.z + boxA->size_.z / 2) > (boxPosB.z - boxB->size_.z / 2) &&
		(boxPosA.z - boxA->size_.z / 2) < (boxPosB.z + boxB->size_.z / 2))
	{
		return true;
	}
	return false;
}

//箱型と球体の衝突判定
//引数：box	    箱型判定
//引数：sphere	球体判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere)
{
	XMFLOAT3 circlePos = Transform::Float3Add(sphere->pGameObject_->GetWorldPosition(), sphere->center_);
	XMFLOAT3 boxPos = Transform::Float3Add(box->pGameObject_->GetWorldPosition(), box->center_);

	if (circlePos.x > boxPos.x - box->size_.x - sphere->size_.x &&
		circlePos.x < boxPos.x + box->size_.x + sphere->size_.x &&
		circlePos.y > boxPos.y - box->size_.y - sphere->size_.x &&
		circlePos.y < boxPos.y + box->size_.y + sphere->size_.x &&
		circlePos.z > boxPos.z - box->size_.z - sphere->size_.x &&
		circlePos.z < boxPos.z + box->size_.z + sphere->size_.x )
	{
		return true;
	}

	return false;
}

//球体同士の衝突判定
//引数：circleA	１つ目の球体判定
//引数：circleB	２つ目の球体判定
//戻値：接触していればtrue
bool Collider::IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB)
{
	XMFLOAT3 centerA = circleA->center_;
	XMFLOAT3 positionA = circleA->pGameObject_->GetWorldPosition();
	XMFLOAT3 centerB = circleB->center_;
	XMFLOAT3 positionB = circleB->pGameObject_->GetWorldPosition();

	XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
		- (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

	if (XMVector3Length(v).m128_f32[0] <= circleA->size_.x + circleB->size_.x)
	{
		return true;
	}

	return false;
}

//カプセルと球体の衝突判定
//引数：capsule　カプセル判定
//引数：Sphere　 球体判定
//戻値：接触していればtrue
bool Collider::IsHitCapsuleVsCircle(CapsuleCollider* capsule, SphereCollider* sphere)
{
	XMFLOAT3 capsuleStart = Transform::Float3Add(capsule->pGameObject_->GetWorldPosition(), capsule->start_);
	XMFLOAT3 capsuleEnd = Transform::Float3Add(capsule->pGameObject_->GetWorldPosition(), capsule->end_);
	XMFLOAT3 sphereCenter = Transform::Float3Add(sphere->pGameObject_->GetWorldPosition(), sphere->center_);

	XMVECTOR start = XMLoadFloat3(&capsuleStart);
	XMVECTOR end = XMLoadFloat3(&capsuleEnd);
	XMVECTOR center = XMLoadFloat3(&sphereCenter);

	XMVECTOR lineDir = end - start;
	XMVECTOR pointDir = center - start;
	float t = XMVector3Dot(pointDir, lineDir).m128_f32[0] / XMVector3Dot(lineDir, lineDir).m128_f32[0];
	t = std::clamp(t, 0.0f, 1.0f);
	XMVECTOR closestPoint = start + t * lineDir;

	float distance = XMVector3Length(center - closestPoint).m128_f32[0];
	return distance <= (capsule->radius_ + sphere->size_.x);
}

//カプセルと箱型の衝突判定
//引数：capsule　カプセル判定
//引数：box　    球体判定
//戻値：接触していればtrue
bool Collider::IsHitCapsuleVsBox(CapsuleCollider* capsule, BoxCollider* box)
{
	XMFLOAT3 capsuleStart = Transform::Float3Add(capsule->pGameObject_->GetWorldPosition(), capsule->start_);
	XMFLOAT3 capsuleEnd = Transform::Float3Add(capsule->pGameObject_->GetWorldPosition(), capsule->end_);

	XMFLOAT3 boxPos = Transform::Float3Add(box->pGameObject_->GetWorldPosition(), box->center_);

	XMVECTOR start = XMLoadFloat3(&capsuleStart);
	XMVECTOR end = XMLoadFloat3(&capsuleEnd);

	XMFLOAT3 boxMin = { boxPos.x - box->size_.x / 2, boxPos.y - box->size_.y / 2, boxPos.z - box->size_.z / 2 };
	XMFLOAT3 boxMax = { boxPos.x + box->size_.x / 2, boxPos.y + box->size_.y / 2, boxPos.z + box->size_.z / 2 };

	if (IsPointInsideBox(capsuleStart, boxMin, boxMax) || IsPointInsideBox(capsuleEnd, boxMin, boxMax))
	{
		return true;
	}

	std::vector<XMFLOAT3> boxVertices = {
		{boxMin.x, boxMin.y, boxMin.z},
		{boxMax.x, boxMin.y, boxMin.z},
		{boxMin.x, boxMax.y, boxMin.z},
		{boxMax.x, boxMax.y, boxMin.z},
		{boxMin.x, boxMin.y, boxMax.z},
		{boxMax.x, boxMin.y, boxMax.z},
		{boxMin.x, boxMax.y, boxMax.z},
		{boxMax.x, boxMax.y, boxMax.z},
	};

	std::vector<std::pair<int, int>> boxEdges = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0},
		{4, 5}, {5, 7}, {7, 6}, {6, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};

	for (auto edge : boxEdges)
	{
		XMVECTOR boxEdgeStart = XMLoadFloat3(&boxVertices[edge.first]);
		XMVECTOR boxEdgeEnd = XMLoadFloat3(&boxVertices[edge.second]);

		float distance = SegmentToSegmentDistance(start, end, boxEdgeStart, boxEdgeEnd);

		if (distance <= capsule->radius_)
		{
			return true;
		}
	}

	return false;
}

// 線分同士の最短距離を計算
float Collider::SegmentToSegmentDistance(XMVECTOR p1, XMVECTOR q1, XMVECTOR p2, XMVECTOR q2)
{
	XMVECTOR d1 = q1 - p1;
	XMVECTOR d2 = q2 - p2;
	XMVECTOR r = p1 - p2;

	float a = XMVectorGetX(XMVector3Dot(d1, d1));
	float e = XMVectorGetX(XMVector3Dot(d2, d2));
	float f = XMVectorGetX(XMVector3Dot(d2, r));

	float s, t;

	if (a <= FLT_EPSILON && e <= FLT_EPSILON) {
		return XMVectorGetX(XMVector3Length(r));
	}
	if (a <= FLT_EPSILON) {
		s = 0.0f;
		t = std::clamp(f / e, 0.0f, 1.0f);
	}
	else {
		float c = XMVectorGetX(XMVector3Dot(d1, r));
		if (e <= FLT_EPSILON) {
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else {
			float b = XMVectorGetX(XMVector3Dot(d1, d2));
			float denom = a * e - b * b;

			if (denom != 0.0f) {
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else {
				s = 0.0f;
			}

			t = (b * s + f) / e;

			if (t < 0.0f) {
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	XMVECTOR c1 = p1 + s * d1;
	XMVECTOR c2 = p2 + t * d2;
	return XMVectorGetX(XMVector3Length(c1 - c2));
}

//カプセル同士の衝突判定
//引数：cupsuleA	１つ目のカプセル判定
//引数：cupsuleB	２つ目のカプセル判定
//戻値：接触していればtrue
bool Collider::IsHitCapsuleVsCapsule(CapsuleCollider* capsuleA, CapsuleCollider* capsuleB)
{
	XMFLOAT3 startA = Transform::Float3Add(capsuleA->pGameObject_->GetWorldPosition(), capsuleA->start_);
	XMFLOAT3 endA = Transform::Float3Add(capsuleA->pGameObject_->GetWorldPosition(), capsuleA->end_);
	XMFLOAT3 startB = Transform::Float3Add(capsuleB->pGameObject_->GetWorldPosition(), capsuleB->start_);
	XMFLOAT3 endB = Transform::Float3Add(capsuleB->pGameObject_->GetWorldPosition(), capsuleB->end_);

	XMVECTOR p1 = XMLoadFloat3(&startA);
	XMVECTOR p2 = XMLoadFloat3(&endA);
	XMVECTOR q1 = XMLoadFloat3(&startB);
	XMVECTOR q2 = XMLoadFloat3(&endB);

	XMVECTOR dA = p2 - p1;
	XMVECTOR dB = q2 - q1;
	XMVECTOR r = p1 - q1;

	float a = XMVector3Dot(dA, dA).m128_f32[0];
	float b = XMVector3Dot(dA, dB).m128_f32[0];
	float c = XMVector3Dot(dB, dB).m128_f32[0];
	float d = XMVector3Dot(dA, r).m128_f32[0];
	float e = XMVector3Dot(dB, r).m128_f32[0];

	float denominator = a * c - b * b;
	float s, t;

	if (denominator != 0.0f) {
		s = (b * e - c * d) / denominator;
		t = (a * e - b * d) / denominator;

		s = std::clamp(s, 0.0f, 1.0f);
		t = std::clamp(t, 0.0f, 1.0f);
	}
	else {
		s = 0.0f;
		t = e / c;
		t = std::clamp(t, 0.0f, 1.0f);
	}

	XMVECTOR closestA = p1 + s * dA;
	XMVECTOR closestB = q1 + t * dB;

	float distance = XMVector3Length(closestA - closestB).m128_f32[0];
	return distance <= (capsuleA->radius_ + capsuleB->radius_);
}

// 点がボックス内にあるか判定
bool Collider::IsPointInsideBox(XMFLOAT3 point, XMFLOAT3 boxMin, XMFLOAT3 boxMax)
{
	return (point.x >= boxMin.x && point.x <= boxMax.x &&
		point.y >= boxMin.y && point.y <= boxMax.y &&
		point.z >= boxMin.z && point.z <= boxMax.z);
}

//テスト表示用の枠を描画
//引数：position	オブジェクトの位置
void Collider::Draw(XMFLOAT3 position)
{
	Transform transform;
	transform.position_ = XMFLOAT3(position.x + center_.x, position.y + center_.y, position.z + center_.z);
	transform.scale_ = size_;
	transform.Calclation();
	Model::SetTransform(hDebugModel_, transform);
	Model::Draw(hDebugModel_);
}
