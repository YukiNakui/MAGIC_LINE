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

    lowSpeedThreshold_ = 50.0f;
    timeBeforeSceneChange_ = 1.5f;

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
        //�{�[���̈ʒu���X�V����
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; //�O��̃{�[���̈ʒu��ۑ�

        //�d�͂𑬓x�ɓK�p
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));
        //�i�s�����x�N�g���X�V
        moveVec_ = XMVector3Normalize(ballVelocity_);
        //�{�[���̈ʒu�X�V
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        //�V�����ʒu�𔽉f
        XMStoreFloat3(&transform_.position_, ballPos_);

        //�J�v�Z���̈ʒu�擾
        std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");

        for (auto& pCapsule : pCapsules) {
            if (pCapsule != nullptr) {
                XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
                XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
                XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
                XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);
                XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

                //�{�[���̈ʒu����J�v�Z���̊J�n�_�ւ̃x�N�g��
                XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
                //�J�v�Z�����x�N�g���Ɛi�s�����x�N�g���̓���
                float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
                //�J�v�Z�����x�N�g���̒���
                float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
                //�ŒZ�_�̈ʒu
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                //�ŒZ�_�̍��W�v�Z
                XMVECTOR closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));

                //�{�[���ƃJ�v�Z���̍ŒZ����
                XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
                float distLength = XMVectorGetX(XMVector3Length(distance));

                //���̋����ȉ��ŏՓ˔���
                if (distLength <= 5.0f) {
                    //�J�v�Z���̒��S�����擾
                    XMVECTOR capsuleAxis = XMVectorSubtract(capsuleEndPos, capsuleStartPos);
                    capsuleAxis = XMVector3Normalize(capsuleAxis);

                    //�{�[���ƃJ�v�Z���̍ŒZ�_�����ԃx�N�g�����v�Z
                    XMVECTOR ballToContact = XMVectorSubtract(ballPos_, closestPointOnCapsule);

                    // �{�[���̐ڐG�_�̖@�����v�Z
                    XMVECTOR normal = XMVectorSubtract(ballToContact, XMVectorScale(capsuleAxis, XMVector3Dot(ballToContact, capsuleAxis).m128_f32[0]));
                    normal = XMVector3Normalize(normal);

                    //�{�[�����J�v�Z���̕\�ʂɉ����߂�
                    float penetrationDepth = 5.0f - distLength;
                    ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(normal, penetrationDepth + 0.1f));

                    //���˃x�N�g�����v�Z
                    float e = 0.4f; //�����W��
                    XMVECTOR reflectedVelocity = XMVector3Reflect(ballVelocity_, normal);
                    reflectedVelocity = XMVectorScale(reflectedVelocity, e);

                    //�Œᑬ�x�ݒ�
                    float minSpeed = 1.0f;
                    if (XMVectorGetX(XMVector3Length(reflectedVelocity)) < minSpeed) {
                        reflectedVelocity = XMVectorScale(normal, minSpeed);
                    }

                    //�V�����{�[���̑��x��K�p
                    ballVelocity_ = reflectedVelocity;

                    float ballSpeed = XMVectorGetX(XMVector3Length(ballVelocity_));

                    // �ᑬ��Ԃ��J�E���g
                    if (ballSpeed < lowSpeedThreshold_) {
                        lowSpeedTime_ += deltaTime;
                        if (lowSpeedTime_ >= timeBeforeSceneChange_) {
                            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
                            pSceneManager->ChangeScene(SCENE_ID_CLEAR);
                        }
                    }
                    else {
                        lowSpeedTime_ = 0.0f; // ���x��臒l�𒴂����烊�Z�b�g
                    }

                    //�ʒu�̍X�V
                    XMStoreFloat3(&transform_.position_, ballPos_);
                    prevBallPos_ = ballPos_; //�V�����ʒu��ۑ�
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
