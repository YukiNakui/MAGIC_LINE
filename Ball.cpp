#include "Ball.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Capsule.h"
#include<list>
#include<algorithm>

Ball::Ball(GameObject* parent)
	:GameObject(parent, "Ball"), hModel_(-1), cdTimer_(nullptr), gravity_(0.98f), ballVelocity_{0,0,0,0} ,canMove_(false)
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Model/Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0,50.0,5.0 };
	ballPos_ = XMLoadFloat3(&transform_.position_);
	prevBallPos_ = ballPos_;
	transform_.scale_ = { 5.0f,5.0f,5.0f };
	cdTimer_ = Instantiate<CDTimer>(this);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	AddCollider(collision);
}

void Ball::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		ballVelocity_ = { 0,-1,0,0 };
		canMove_ = true;
	}

	if (canMove_) {
		std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");//����ŕ����I�u�W�F�N�g���擾�ł��Ă���ۂ�
		XMVECTOR capsuleStartPos = XMVectorZero(), capsuleEndPos = XMVectorZero();//�J�v�Z���̎n�_�ƏI�_
		XMVECTOR capsuleDir = XMVectorZero();//�J�v�Z���̎��x�N�g��
		for (auto& pCapsule : pCapsules) {
			if (pCapsule != nullptr) {
				XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
				capsuleStartPos = XMLoadFloat3(&capsuleStart);
				XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
				capsuleEndPos = XMLoadFloat3(&capsuleEnd);
				capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);
			}
		}

		float deltaTime = cdTimer_->GetDeltaTime();
		ballVelocity_ = XMVectorSubtract(ballPos_, prevBallPos_);//�i�s�����x�N�g��

		// �{�[���̈ʒu����J�v�Z���̊J�n�_�ւ̃x�N�g��
		XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
		// �J�v�Z�����x�N�g���Ɛi�s�����x�N�g���̓��ς��v�Z
		float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
		// �J�v�Z�����x�N�g���̒���
		float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
		// �ŒZ�_�̈ʒu�i�J�v�Z���̎���̓_�j
		float t = std::clamp(dot / (capsuleLength * capsuleLength),0.0f,1.0f);
		// �ŒZ�_�̍��W���v�Z
		XMVECTOR closestPointOnCapsule = 
			XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));
		// �ڐG�_����J�v�Z���̒��S�֌������@���x�N�g�����v�Z
		XMVECTOR normal = XMVectorSubtract(closestPointOnCapsule, capsuleStartPos);
		normal = XMVector3Normalize(normal); // �@���𐳋K��
		// ���˃x�N�g�����v�Z
		float reflectVec = XMVector3Dot(ballVelocity_, normal).m128_f32[0];
		XMVECTOR reflectedVelocity = 
			XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2 * dot));
		// �����W����K�p�i��: 0.9�j
		float e = 0.9f;
		reflectedVelocity = DirectX::XMVectorScale(reflectedVelocity, e);
		
		// �V�����{�[���̑��x�𔽉f
		ballVelocity_ = reflectedVelocity;
	}
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}

void Ball::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Stage") {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
	}
	else if (pTarget->GetObjectName() == "Capsule") {
		/*SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_CLEAR);*/
		//Capsule capsule = pTarget;
		//float capsuleRad = capsule.GetRadius();
	}
}
