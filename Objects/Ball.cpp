#include "Ball.h"
#include"../Engine/Model.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Input.h"
#include"Line.h"
#include<list>
#include<algorithm>

//�{�[���֘A�̒萔
namespace BallConstants {
    constexpr float GRAVITY = 9.8f;             //�d��
    constexpr float COLLISION_RADIUS = 5.8f;   //�{�[���ƃJ�v�Z���̏Փ˔��a
    constexpr float MIN_SPEED = 1.0f;          //�{�[���̍Œᑬ�x
    constexpr float BOUNCE_FACTOR = 0.6f;      //�����W��
    constexpr float LOW_SPEED_THRESHOLD = 5.0f; //�ᑬ�Ɣ��肷�鑬�x
    constexpr float PENETRATION_CORRECTION = 0.5f; //�Փˎ��̕␳�l
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
        moveVec_ = XMVector3Normalize(ballVelocity_);
        //�{�[���̈ʒu���X�V
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        //�V�����ʒu�𔽉f
        XMStoreFloat3(&transform_.position_, ballPos_);

        //�J�v�Z���Ƃ̏Փ˔���
        HandleCapsuleCollisions();
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
            float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
            XMVECTOR closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));

            //�{�[���ƃJ�v�Z���̍ŒZ�������v�Z
            XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
            float distLength = XMVectorGetX(XMVector3Length(distance));

            //�Փ˔���
            if (distLength <= BallConstants::COLLISION_RADIUS) {
                HandleCollisionWithCapsule(distance, closestPointOnCapsule, capsuleDir, distLength);
            }
        }
    }
}

void Ball::HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength)
{
    //�J�v�Z���\�ʂɃ{�[���������߂�
    XMVECTOR capsuleAxis = XMVector3Normalize(capsuleDir);
    XMVECTOR ballToContact = XMVectorSubtract(ballPos_, closestPoint);
    XMVECTOR normal = XMVectorSubtract(ballToContact, XMVectorScale(capsuleAxis, XMVector3Dot(ballToContact, capsuleAxis).m128_f32[0]));
    normal = XMVector3Normalize(normal);

    //�Փː[�x��␳
    float penetrationDepth = BallConstants::COLLISION_RADIUS - distLength;
    ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(normal, penetrationDepth + BallConstants::PENETRATION_CORRECTION));

    //���˃x�N�g�����v�Z
    XMVECTOR reflectedVelocity = XMVector3Reflect(ballVelocity_, normal);
    reflectedVelocity = XMVectorScale(reflectedVelocity, BallConstants::BOUNCE_FACTOR);

    //�Œᑬ�x��ۏ�
    if (XMVectorGetX(XMVector3Length(reflectedVelocity)) < BallConstants::MIN_SPEED) {
        reflectedVelocity = XMVectorScale(normal, BallConstants::MIN_SPEED);
    }

    //�V�������x��K�p
    ballVelocity_ = reflectedVelocity;

    //�ᑬ��Ԃ̃`�F�b�N
    CheckLowSpeedState();
}

void Ball::CheckLowSpeedState()
{
    //�{�[���̑��x���v�Z
    float ballSpeed = XMVectorGetX(XMVector3Length(ballVelocity_));

    if (ballSpeed < BallConstants::LOW_SPEED_THRESHOLD) {
        //�J�E���g�_�E���I�u�W�F�N�g�𐶐�
        if (pCountDownNumber_ == nullptr) {
            pCountDownNumber_ = Instantiate<CountdownNumber>(this);
        }

        //�J�E���g�_�E�����I��������V�[����؂�ւ�
        if (pCountDownNumber_ != nullptr && pCountDownNumber_->IsFinished()) {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_CLEAR);
        }
    }
    else {
        //���x��臒l�𒴂����ꍇ�A���Z�b�g
        lowSpeedTime_ = 0.0f;
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
