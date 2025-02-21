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
        // ボールの位置を更新する処理
        float deltaTime = cdTimer_->GetDeltaTime();
        prevBallPos_ = ballPos_; // 前回のボールの位置を保存

        // 重力を速度に適用（Y軸方向）
        ballVelocity_ = XMVectorAdd(ballVelocity_, XMVectorSet(0.0f, -gravity_ * deltaTime, 0.0f, 0.0f));
        // 進行方向ベクトルを更新（速度から計算）
        moveVec_ = XMVector3Normalize(ballVelocity_);
        // ボールの位置を更新（速度に基づく移動）
        ballPos_ = XMVectorAdd(ballPos_, XMVectorScale(ballVelocity_, deltaTime));

        // 新しい位置をtransformに反映
        XMStoreFloat3(&transform_.position_, ballPos_);

        // カプセルの位置を取得
        std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");

        for (auto& pCapsule : pCapsules) {
            if (pCapsule != nullptr) {
                XMFLOAT3 capsuleStart = pCapsule->GetStartPos();
                XMVECTOR capsuleStartPos = XMLoadFloat3(&capsuleStart);
                XMFLOAT3 capsuleEnd = pCapsule->GetEndPos();
                XMVECTOR capsuleEndPos = XMLoadFloat3(&capsuleEnd);
                XMVECTOR capsuleDir = XMVectorSubtract(capsuleEndPos, capsuleStartPos);

                // ボールの位置からカプセルの開始点へのベクトル
                XMVECTOR ballToStart = XMVectorSubtract(ballPos_, capsuleStartPos);
                // カプセル軸ベクトルと進行方向ベクトルの内積を計算
                float dot = XMVector3Dot(ballToStart, capsuleDir).m128_f32[0];
                // カプセル軸ベクトルの長さ
                float capsuleLength = XMVector3Length(capsuleDir).m128_f32[0];
                // 最短点の位置（カプセルの軸上の点）
                float t = std::clamp(dot / (capsuleLength * capsuleLength), 0.0f, 1.0f);
                // 最短点の座標を計算
                XMVECTOR closestPointOnCapsule = XMVectorAdd(capsuleStartPos, XMVectorScale(capsuleDir, t));

                // ボールとカプセルの最短距離を計算（衝突判定）
                XMVECTOR distance = XMVectorSubtract(ballPos_, closestPointOnCapsule);
                float distLength = XMVectorGetX(XMVector3Length(distance));

                // 一定の距離以下で衝突判定を行う（ここでの衝突判定距離は5.0fに設定）
                if (distLength <= 5.0f) {
                    // 接触点からカプセルの中心へ向かう法線ベクトルを計算
                    XMVECTOR normal = XMVectorSubtract(closestPointOnCapsule, ballPos_);
                    normal = XMVector3Normalize(normal); // 法線を正規化

                    // 反射ベクトルを計算
                    float reflectVec = XMVector3Dot(ballVelocity_, normal).m128_f32[0];
                    XMVECTOR reflectedVelocity = XMVectorSubtract(ballVelocity_, XMVectorScale(normal, 2 * reflectVec));
                    // 反発係数を適用（例: 0.9）
                    float e = 0.9f;
                    reflectedVelocity = DirectX::XMVectorScale(reflectedVelocity, e);

                    // 新しいボールの速度を反映（進行方向ベクトルを更新）
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
