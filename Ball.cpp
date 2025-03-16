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
        //ボールの位置を更新する
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; //前回のボールの位置を保存

        //重力を速度に適用
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));
        //進行方向ベクトル更新
        moveVec_ = XMVector3Normalize(ballVelocity_);
        //ボールの位置更新
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        //新しい位置を反映
        XMStoreFloat3(&transform_.position_, ballPos_);

        //カプセルの位置取得
        std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");

        for (auto& pCapsule : pCapsules) {
            if (pCapsule != nullptr) {
                XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
                XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
                XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
                XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);
                XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

                //ボールの位置からカプセルの開始点へのベクトル
                XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
                //カプセル軸ベクトルと進行方向ベクトルの内積
                float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
                //カプセル軸ベクトルの長さ
                float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
                //最短点の位置
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                //最短点の座標計算
                XMVECTOR closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));

                //ボールとカプセルの最短距離
                XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
                float distLength = XMVectorGetX(XMVector3Length(distance));

                //一定の距離以下で衝突判定
                if (distLength <= 5.0f) {
                    //カプセルの中心軸を取得
                    XMVECTOR capsuleAxis = XMVectorSubtract(capsuleEndPos, capsuleStartPos);
                    capsuleAxis = XMVector3Normalize(capsuleAxis);

                    //ボールとカプセルの最短点を結ぶベクトルを計算
                    XMVECTOR ballToContact = XMVectorSubtract(ballPos_, closestPointOnCapsule);

                    // ボールの接触点の法線を計算
                    XMVECTOR normal = XMVectorSubtract(ballToContact, XMVectorScale(capsuleAxis, XMVector3Dot(ballToContact, capsuleAxis).m128_f32[0]));
                    normal = XMVector3Normalize(normal);

                    //ボールをカプセルの表面に押し戻す
                    float penetrationDepth = 5.0f - distLength;
                    ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(normal, penetrationDepth + 0.1f));

                    //反射ベクトルを計算
                    float e = 0.4f; //反発係数
                    XMVECTOR reflectedVelocity = XMVector3Reflect(ballVelocity_, normal);
                    reflectedVelocity = XMVectorScale(reflectedVelocity, e);

                    //最低速度設定
                    float minSpeed = 1.0f;
                    if (XMVectorGetX(XMVector3Length(reflectedVelocity)) < minSpeed) {
                        reflectedVelocity = XMVectorScale(normal, minSpeed);
                    }

                    //新しいボールの速度を適用
                    ballVelocity_ = reflectedVelocity;

                    float ballSpeed = XMVectorGetX(XMVector3Length(ballVelocity_));

                    // 低速状態をカウント
                    if (ballSpeed < lowSpeedThreshold_) {
                        lowSpeedTime_ += deltaTime;
                        if (lowSpeedTime_ >= timeBeforeSceneChange_) {
                            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
                            pSceneManager->ChangeScene(SCENE_ID_CLEAR);
                        }
                    }
                    else {
                        lowSpeedTime_ = 0.0f; // 速度が閾値を超えたらリセット
                    }

                    //位置の更新
                    XMStoreFloat3(&transform_.position_, ballPos_);
                    prevBallPos_ = ballPos_; //新しい位置を保存
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
