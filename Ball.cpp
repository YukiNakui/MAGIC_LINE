#include "Ball.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Capsule.h"
#include<list>
#include<algorithm>

Ball::Ball(GameObject* parent)
	:GameObject(parent, "Ball"), hModel_(-1), cdTimer_(nullptr), gravity_(9.8f), moveVec_{0,0,0,0} ,canMove_(false)
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Model/Ball.fbx");
	assert(hModel_ >= 0);
	
	transform_.position_ = { 0.0,10.0,5.0 };
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
		canMove_ = true;
	}

    if (canMove_) {
        // �{�[���̈ʒu���X�V���鏈��
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; // �O��̃{�[���̈ʒu��ۑ�

        // �d�͂𑬓x�ɓK�p�iY�������j
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));
        // �i�s�����x�N�g�����X�V�i���x����v�Z�j
        moveVec_ = XMVector3Normalize(ballVelocity_);
        // �{�[���̈ʒu���X�V�i���x�Ɋ�Â��ړ��j
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        // �V�����ʒu��transform�ɔ��f
        XMStoreFloat3(&transform_.position_, ballPos_);

        // �J�v�Z���̈ʒu���擾
        std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");

        for (auto& pCapsule : pCapsules) {
            if (pCapsule != nullptr) {
                XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
                XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
                XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
                XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);
                XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

                // �{�[���̈ʒu����J�v�Z���̊J�n�_�ւ̃x�N�g��
                XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
                // �J�v�Z�����x�N�g���Ɛi�s�����x�N�g���̓��ς��v�Z
                float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
                // �J�v�Z�����x�N�g���̒���
                float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
                // �ŒZ�_�̈ʒu�i�J�v�Z���̎���̓_�j
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                // �ŒZ�_�̍��W���v�Z
                XMVECTOR closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));

                // �{�[���ƃJ�v�Z���̍ŒZ�������v�Z�i�Փ˔���j
                XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
                float distLength = XMVectorGetX(XMVector3Length(distance));

                // ���̋����ȉ��ŏՓ˔�����s���i�����ł̏Փ˔��苗����5.0f�ɐݒ�j
                if (distLength <= 5.0f) {
                    // �ڐG�_����J�v�Z���̒��S�֌������@���x�N�g�����v�Z
                    XMVECTOR normal = XMVectorSubtract(closestPointOnCapsule, ballPos_);
                    normal = XMVector3Normalize(normal); // �@���𐳋K��

                    // ���˃x�N�g�����v�Z
                    float reflectVec = XMVector3Dot(ballVelocity_, normal).m128_f32[0];
                    XMVECTOR reflectedVelocity = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2 * reflectVec));
                    // �����W����K�p�i��: 0.9�j
                    float e = 0.9f;
                    reflectedVelocity = DirectX::XMVectorScale(reflectedVelocity, e);

                    // �V�����{�[���̑��x�𔽉f�i�i�s�����x�N�g�����X�V�j
                    ballVelocity_ = reflectedVelocity;
                }
            }
        }
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
