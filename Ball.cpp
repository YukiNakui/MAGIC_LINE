#include "Ball.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Capsule.h"
#include<list>

Ball::Ball(GameObject* parent)
	:GameObject(parent, "Ball"), hModel_(-1), cdTimer_(nullptr), speed_(0,0,0),canMove_(false)
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("Model/Ball.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = { 0.0,50.0,5.0 };
	transform_.scale_ = { 5.0f,5.0f,5.0f };
	cdTimer_ = Instantiate<CDTimer>(this);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 5.0f);
	AddCollider(collision);
}

void Ball::Update()
{
	if (Input::IsKeyDown(DIK_RETURN))
		canMove_ = true;
	if (canMove_) {
		float deltaTime = cdTimer_->GetDeltaTime();
		const float gravity = -9.8f;
		speed_.y += gravity * deltaTime;
		transform_.position_.y += speed_.y * deltaTime;
	}

	RayCastData data;
	data.start = transform_.position_;   //レイの発射位置
	data.dir = XMFLOAT3(0, -1, 0);       //レイの方向

	std::list <Capsule*> pCapsules = GetParent()->FindGameObjects<Capsule>("Capsule");//これで複数オブジェクトを取得できてるっぽい
	for (auto& pCapsule : pCapsules) {
		int hCapusleModel = 0;
		if (pCapsule != nullptr) {
			hCapusleModel = pCapsule->GetModelHandle();    //モデル番号を取得
			Model::RayCast(hCapusleModel, &data); //レイを発射
			if (data.dist < 100.0f) {
					speed_.y = 0.0f;
					canMove_ = false;
				}
		}
	}
	//Capsule* pCapsule = (Capsule*)FindObject("Capsule");    //ステージオブジェクトを探す
	//int hCapusleModel = 0;
	//if (pCapsule != nullptr) {
	//	hCapusleModel = pCapsule->GetModelHandle();    //モデル番号を取得
	//}
	//RayCastData data;
	//data.start = transform_.position_;   //レイの発射位置
	//data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
	//Model::RayCast(hCapusleModel, &data); //レイを発射

	//if (data.dist < 100.0f) {
	//	speed_.y = 0.0f;
	//	canMove_ = false;
	//}
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
	}
}
