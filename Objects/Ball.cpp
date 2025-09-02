#include "Ball.h"
#include"../Engine/Model.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Input.h"
#include"Line.h"
#include"../Torus.h"
#include<list>
#include<algorithm>

//ボール関連の定数
namespace BallConstants {
    constexpr float GRAVITY = 9.8f;             //重力
    constexpr float MIN_SPEED = 1.5f;          //ボールの最低速度
    constexpr float BOUNCE_FACTOR = 0.8f;      //反発係数
    constexpr float LOW_SPEED_THRESHOLD = 3.0f; //低速と判定する速度
    constexpr float PENETRATION_CORRECTION = 0.5f; //衝突時の補正値
    constexpr float MIN_HEIGHT = -10.0f;
}

Ball::Ball(GameObject* parent)
	: GameObject(parent, "Ball"), hModel_(-1), cdTimer_(nullptr), radius_(1.0f),
    gravity_(BallConstants::GRAVITY),moveVec_{ 0, 0, 0, 0 },canMove_(false)
{
}

void Ball::Initialize()
{
    //ボールモデルのロード
    hModel_ = Model::Load("Models/Objects/Ball.fbx");
    assert(hModel_ >= 0);

    //タイマーの初期化
    cdTimer_ = Instantiate<CDTimer>(this);
}

void Ball::Update()
{
    if (canMove_) {
        //ボールの位置を更新
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; //前回の位置を保存

        //重力を速度に適用
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));

        //進行方向ベクトルを計算
        if (XMVectorGetX(XMVector3Length(ballVelocity_)) > 0.0f)
            moveVec_ = XMVector3Normalize(ballVelocity_);
        else
            moveVec_ = XMVectorZero(); //速度がゼロの場合は移動ベクトルをゼロにする

        //ボールの位置を更新
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        //新しい位置を反映
        XMStoreFloat3(&transform_.position_, ballPos_);

        //カプセルとの衝突判定
        HandleCapsuleCollisions();

        pTorus_ = (Torus*)FindObject("Torus");
        if (pTorus_ != nullptr) {
            //トーラスとの衝突判定
            HandleCollisionWithTorus(pTorus_);
        }


        //最低高度以下になったら、ゲームオーバー
        if (transform_.position_.y <= BallConstants::MIN_HEIGHT) {
            SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
            pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
        }
    }
}

void Ball::HandleCapsuleCollisions()
{
    //カプセルのリストを取得
    std::list<Line*> pCapsules = GetParent()->FindGameObjects<Line>("Line");

    for (auto& pCapsule : pCapsules) {
        if (pCapsule != nullptr) {
            //カプセルの開始点と終了点を取得
            XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
            XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
            XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
            XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);

            //カプセルの軸ベクトル
            XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

            //ボールとカプセルの最近接点を計算
            XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
            float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
            float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
			XMVECTOR closestPointOnCapsule;
            if (capsuleLength > 0.0f) {
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));
            }
            else
                closestPointOnCapsule = capsuleStartPos; //カプセルが点の場合、開始点を最近接点とする

            //ボールとカプセルの最短距離を計算
            XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
            float distLength = XMVectorGetX(XMVector3Length(distance));

            //衝突判定
            if (distLength <= radius_)
                HandleCollisionWithCapsule(distance, closestPointOnCapsule, capsuleDir, distLength);
        }
    }
}

void Ball::HandleCollisionWithTorus(Torus* torus)
{
    if (torus != nullptr) {
        if (torus->CheckHitTorusToSphere(transform_, radius_)) {
            // 管表面との衝突処理
            // 位置補正 & 速度反射
            XMFLOAT3 centerToSphere = {
                transform_.position_.x - torus->GetPosition().x,
                transform_.position_.y - torus->GetPosition().y,
                transform_.position_.z - torus->GetPosition().z
            };
            XMVECTOR v = XMLoadFloat3(&centerToSphere);
            XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torus->GetAxis()));
            float dot = XMVectorGetX(XMVector3Dot(v, axis));
            XMVECTOR v_proj = XMVectorSubtract(v, XMVectorScale(axis, dot));
            XMVECTOR nearestOnCircle = XMVectorScale(XMVector3Normalize(v_proj), torus->GetMainRadius());
            XMFLOAT3 torusPos = torus->GetPosition();
            XMVECTOR tubeCenter = XMVectorAdd(XMLoadFloat3(&torusPos), nearestOnCircle);

            XMVECTOR sphereToTube = XMVectorSubtract(ballPos_, tubeCenter);
            XMVECTOR normal = XMVector3Normalize(sphereToTube);
            XMVECTOR newPos = XMVectorAdd(tubeCenter, XMVectorScale(normal, torus->GetTubeRadius() + radius_));
            XMStoreFloat3(&transform_.position_, newPos);
            ballPos_ = newPos;

            float speed = XMVectorGetX(XMVector3Length(ballVelocity_));
            XMVECTOR reflected = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2.0f * XMVector3Dot(ballVelocity_, normal).m128_f32[0]));
            ballVelocity_ = XMVectorScale(reflected, BallConstants::BOUNCE_FACTOR);
        }
    }
}

void Ball::HandleCollisionWithCapsule(XMVECTOR distance, XMVECTOR closestPoint, XMVECTOR capsuleDir, float distLength)
{
    //カプセル表面にボールを押し戻す
    XMVECTOR capsuleAxis = XMVector3Normalize(capsuleDir);
    XMVECTOR ballToContact = XMVectorSubtract(ballPos_, closestPoint);
    XMVECTOR normal = XMVectorSubtract(ballToContact, XMVectorScale(capsuleAxis, XMVector3Dot(ballToContact, capsuleAxis).m128_f32[0]));

    if (XMVectorGetX(XMVector3Length(normal)) > 0.0f)
        normal = XMVector3Normalize(normal);
    else
        normal = XMVector3Normalize(capsuleAxis);

    //衝突深度を補正
    float penetrationDepth = radius_ - distLength;
    ballPos_ = XMVectorAdd(closestPoint, XMVectorScale(normal, radius_ + BallConstants::PENETRATION_CORRECTION));

    //反射ベクトルを計算
    XMVECTOR reflectedVelocity = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2.0f * XMVector3Dot(ballVelocity_, normal).m128_f32[0]));
    reflectedVelocity = XMVectorScale(reflectedVelocity, BallConstants::BOUNCE_FACTOR);

    //最低速度を保証
    if (XMVectorGetX(XMVector3Length(reflectedVelocity)) < BallConstants::MIN_SPEED)
        reflectedVelocity = XMVectorScale(normal, BallConstants::MIN_SPEED);

    //新しい速度を適用
    ballVelocity_ = reflectedVelocity;

    //低速状態のチェック
    //CheckLowSpeedState();
}

void Ball::CheckClearConditions()
{
    switch(clearCondition_) {
    case LOW_SPEED:
		CheckLowSpeedState();
		break;
	case PASSED_THROUGH_TORUS:
		CheckPassedThroughTorus(pTorus_);
		break;
	default:
		break;
    }
}

void Ball::CheckLowSpeedState()
{
    //ボールの速度を計算
    float ballSpeed = XMVectorGetX(XMVector3Length(ballVelocity_));

    if (ballSpeed <= BallConstants::LOW_SPEED_THRESHOLD) {
        //カウントダウンオブジェクトを生成
        if (pCountDownNumber_ == nullptr)
            pCountDownNumber_ = Instantiate<CountdownNumber>(this);
        else {
            if (!pCountDownNumber_->IsCounting())
                pCountDownNumber_->StartCountdown();
        }

        //カウントダウンが終了したらシーンを切り替え
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

void Ball::CheckPassedThroughTorus(Torus* torus)
{
    // トーラス中心・軸
    XMFLOAT3 torusCenter = torus->GetPosition();
    XMVECTOR axis = XMVector3Normalize(XMLoadFloat3(&torus->GetAxis()));

    // 前回・今回の中心からトーラス中心へのベクトル
    XMVECTOR prevVec = XMVectorSubtract(prevPos, XMLoadFloat3(&torusCenter));
    XMVECTOR currVec = XMVectorSubtract(currPos, XMLoadFloat3(&torusCenter));

    // 軸方向への投影
    float prevDot = XMVectorGetX(XMVector3Dot(prevVec, axis));
    float currDot = XMVectorGetX(XMVector3Dot(currVec, axis));
    XMVECTOR prevProj = XMVectorSubtract(prevVec, XMVectorScale(axis, prevDot));
    XMVECTOR currProj = XMVectorSubtract(currVec, XMVectorScale(axis, currDot));

    // 回転面からの距離
    float prevD = XMVectorGetX(XMVector3Length(prevProj));
    float currD = XMVectorGetX(XMVector3Length(currProj));

    // 穴判定（軸上近傍かつ距離が穴の範囲）
    float holeRadius = torus.GetMainRadius() - torus.GetTubeRadius();

    bool prevInHole = (prevD < 1e-6f) && (fabs(XMVectorGetX(XMVector3Length(prevVec)) - torus.GetMainRadius()) <= (holeRadius - ballRadius));
    bool currInHole = (currD < 1e-6f) && (fabs(XMVectorGetX(XMVector3Length(currVec)) - torus.GetMainRadius()) <= (holeRadius - ballRadius));

    // 通り抜け判定（前回穴外・今回穴内、またはその逆）
    return prevInHole != currInHole;
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

void Ball::SetColider(XMFLOAT3 scale)
{
	radius_ = scale.x;

    //衝突判定用のコライダーを設定
    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), radius_);
    AddCollider(collision);
}

void Ball::DrawShadowMap(const XMMATRIX& lightViewProj)
{
    // モデルにワールド変換をセット
    Model::SetTransform(hModel_, transform_);
    // シャドウマップ用描画
    Model::DrawShadow(hModel_, lightViewProj);
}