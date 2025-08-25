#include "Ball.h"
#include"../Engine/Model.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Input.h"
#include"Line.h"
#include"../Torus.h"
#include<list>
#include<algorithm>

//�{�[���֘A�̒萔
namespace BallConstants {
    constexpr float GRAVITY = 9.8f;             //�d��
    constexpr float COLLISION_RADIUS = 5.0f;   //�{�[���ƃJ�v�Z���̏Փ˔��a
    constexpr float MIN_SPEED = 1.5f;          //�{�[���̍Œᑬ�x
    constexpr float BOUNCE_FACTOR = 0.8f;      //�����W��
    constexpr float LOW_SPEED_THRESHOLD = 3.0f; //�ᑬ�Ɣ��肷�鑬�x
    constexpr float PENETRATION_CORRECTION = 0.2f; //�Փˎ��̕␳�l
    constexpr float MIN_HEIGHT = -10.0f;
}

Ball::Ball(GameObject* parent)
    : GameObject(parent, "Ball"),hModel_(-1),cdTimer_(nullptr),
    gravity_(BallConstants::GRAVITY),moveVec_{ 0, 0, 0, 0 },canMove_(false)
{
}

void Ball::Initialize()
{
    //�{�[�����f���̃��[�h
    hModel_ = Model::Load("Models/Objects/Ball.fbx");
    assert(hModel_ >= 0);

    //�^�C�}�[�̏�����
    cdTimer_ = Instantiate<CDTimer>(this);

    //�Փ˔���p�̃R���C�_�[��ݒ�
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), BallConstants::COLLISION_RADIUS);
    AddCollider(collision);
}

void Ball::Update()
{
    if (canMove_) {
        //�{�[���̈ʒu���X�V
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; //�O��̈ʒu��ۑ�

        //�d�͂𑬓x�ɓK�p
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));

        //�i�s�����x�N�g�����v�Z
        if (XMVectorGetX(XMVector3Length(ballVelocity_)) > 0.0f)
            moveVec_ = XMVector3Normalize(ballVelocity_);
        else
            moveVec_ = XMVectorZero(); //���x���[���̏ꍇ�͈ړ��x�N�g�����[���ɂ���

        //�{�[���̈ʒu���X�V
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        //�V�����ʒu�𔽉f
        XMStoreFloat3(&transform_.position_, ballPos_);

        //�J�v�Z���Ƃ̏Փ˔���
        HandleCapsuleCollisions();

		//�g�[���X�Ƃ̏Փ˔���
        Torus* pTorus = (Torus*)FindObject("Torus");
        if (pTorus != nullptr) {
            if (pTorus->CheckHitTorusToSphere(transform_, BallConstants::COLLISION_RADIUS)) {
                // 1. ���̒��S����g�[���X���S�ւ̃x�N�g��
                XMFLOAT3 centerToSphere = {
                    transform_.position_.x - pTorus->GetPosition().x,
                    transform_.position_.y - pTorus->GetPosition().y,
                    transform_.position_.z - pTorus->GetPosition().z
                };
                XMVECTOR v = XMLoadFloat3(&centerToSphere);
                XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&pTorus->GetAxis()));
                float dot = XMVectorGetX(XMVector3Dot(v, axis));
                XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot));
                // ��]�ʏ�ł̍ŋߐړ_
                XMVECTOR nearestOnCircle = XMVectorScale(XMVector3Normalize(v_proj), pTorus->GetMainRadius());
                XMFLOAT3 torusPos = pTorus->GetPosition();
                XMVECTOR tubeCenter = XMVectorAdd(XMLoadFloat3(&torusPos), nearestOnCircle);

                // �����Ǖ\�ʂ܂ŉ����߂�
                XMVECTOR sphereToTube = XMVectorSubtract(ballPos_, tubeCenter);
                XMVECTOR normal = XMVector3Normalize(sphereToTube);
                XMVECTOR newPos = XMVectorAdd(tubeCenter, XMVectorScale(normal, pTorus->GetTubeRadius() + BallConstants::COLLISION_RADIUS));
                XMStoreFloat3(&transform_.position_, newPos);
                ballPos_ = newPos;

                // ���x�x�N�g���𔽎ˁi�Ǖ\�ʂ̖@���Ŕ��ˁ{�����j
                float speed = XMVectorGetX(XMVector3Length(ballVelocity_));
                XMVECTOR reflected = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2.0f * XMVector3Dot(ballVelocity_, normal).m128_f32[0]));
                ballVelocity_ = XMVectorScale(reflected, BallConstants::BOUNCE_FACTOR);

                // �ᑬ����
                CheckLowSpeedState();
            }
        }



        //�Œፂ�x�ȉ��ɂȂ�����A�Q�[���I�[�o�[
        if (transform_.position_.y <= BallConstants::MIN_HEIGHT) {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
        }
    }
}

void Ball::HandleCapsuleCollisions()
{
    //�J�v�Z���̃��X�g���擾
    std::list<Line*> pCapsules = GetParent()->FindGameObjects<Line>("Line");

    for (auto& pCapsule : pCapsules) {
        if (pCapsule != nullptr) {
            //�J�v�Z���̊J�n�_�ƏI���_���擾
            XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
            XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
            XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
            XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);

            //�J�v�Z���̎��x�N�g��
            XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

            //�{�[���ƃJ�v�Z���̍ŋߐړ_���v�Z
            XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
            float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
            float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
			XMVECTOR closestPointOnCapsule;
            if (capsuleLength > 0.0f) {
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));
            }
            else
                closestPointOnCapsule = capsuleStartPos; //�J�v�Z�����_�̏ꍇ�A�J�n�_���ŋߐړ_�Ƃ���

            //�{�[���ƃJ�v�Z���̍ŒZ�������v�Z
            XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
            float distLength = XMVectorGetX(XMVector3Length(distance));

            //�Փ˔���
            if (distLength <= BallConstants::COLLISION_RADIUS)
                HandleCollisionWithCapsule(distance, closestPointOnCapsule, capsuleDir, distLength);
        }
    }
}

void Ball::HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength)
{
    //�J�v�Z���\�ʂɃ{�[���������߂�
    XMVECTOR capsuleAxis = XMVector3Normalize(capsuleDir);
    XMVECTOR ballToContact = XMVectorSubtract(ballPos_, closestPoint);
    XMVECTOR normal = XMVectorSubtract(ballToContact, XMVectorScale(capsuleAxis, XMVector3Dot(ballToContact, capsuleAxis).m128_f32[0]));

    if (XMVectorGetX(XMVector3Length(normal)) > 0.0f)
        normal = XMVector3Normalize(normal);
    else
        normal = XMVector3Normalize(capsuleAxis);

    //�Փː[�x��␳
    float penetrationDepth = BallConstants::COLLISION_RADIUS - distLength;
    ballPos_ = XMVectorAdd(closestPoint, XMVectorScale(normal, BallConstants::COLLISION_RADIUS + BallConstants::PENETRATION_CORRECTION));

    //���˃x�N�g�����v�Z
    XMVECTOR reflectedVelocity = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2.0f * XMVector3Dot(ballVelocity_, normal).m128_f32[0]));
    reflectedVelocity = XMVectorScale(reflectedVelocity, BallConstants::BOUNCE_FACTOR);

    //�Œᑬ�x��ۏ�
    if (XMVectorGetX(XMVector3Length(reflectedVelocity)) < BallConstants::MIN_SPEED)
        reflectedVelocity = XMVectorScale(normal, BallConstants::MIN_SPEED);

    //�V�������x��K�p
    ballVelocity_ = reflectedVelocity;

    //�ᑬ��Ԃ̃`�F�b�N
    CheckLowSpeedState();
}

void Ball::CheckLowSpeedState()
{
    //�{�[���̑��x���v�Z
    float ballSpeed = XMVectorGetX(XMVector3Length(ballVelocity_));

    if (ballSpeed <= BallConstants::LOW_SPEED_THRESHOLD) {
        //�J�E���g�_�E���I�u�W�F�N�g�𐶐�
        if (pCountDownNumber_ == nullptr)
            pCountDownNumber_ = Instantiate<CountdownNumber>(this);
        else {
            if (!pCountDownNumber_->IsCounting())
                pCountDownNumber_->StartCountdown();
        }

        //�J�E���g�_�E�����I��������V�[����؂�ւ�
        if (pCountDownNumber_ != nullptr && pCountDownNumber_->IsFinished()) {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_CLEAR);
        }
    }
    else {
        if (pCountDownNumber_ != nullptr) {
            pCountDownNumber_->ResetCountdown();
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
}

void Ball::DrawShadowMap(const XMMATRIX& lightViewProj)
{
    // ���f���Ƀ��[���h�ϊ����Z�b�g
    Model::SetTransform(hModel_, transform_);
    // �V���h�E�}�b�v�p�`��
    Model::DrawShadow(hModel_, lightViewProj);
}