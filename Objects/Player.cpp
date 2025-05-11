#include "Player.h"
#include"../Engine/Model.h"
#include"../Engine/Input.h"
#include"../Engine/Camera.h"
#include"../Engine/SphereCollider.h"
#include<algorithm>
#include "../Engine/SceneManager.h"
#include"Stage.h"
#include"../UI/LineGauge.h"
#include"../Engine/Audio.h"
#include"Ball.h"

namespace {
	XMVECTOR CAMERA_TARGET_VEC = XMVectorSet(0, 0, 25, 0); //カメラのターゲットベクトル
	XMVECTOR CAMERA_EYE_VEC = XMVectorSet(0, 20, -30, 0); //カメラの視点ベクトル
	float PLAYER_MOVE_SPEED = 10.0f; //プレイヤーの移動速度
	float PLAYER_ROTATION_SPEED = 60.0f; //プレイヤーの回転速度
	float MIN_DISTANCE_TO_BALL = 7.5f; //ボールとプレイヤーの最小距離
	float COLLIDER_RADIUS = 1.5f; //プレイヤーのコライダー半径
	float CAPSULE_SPAWN_INTERVAL = 0.1f; //カプセル生成間隔
	float MAX_LINE_VALUE = 100.0f; //ラインゲージの最大値
	XMFLOAT3 MAX_POS = { 45.0f, 50.0f, 45.0f }; //プレイヤーの最大位置
	XMFLOAT3 MIN_POS = { -45.0f, 0.0f, -45.0f }; //プレイヤーの最小位置
	float ROTATE_MAX = 45.0f; //プレイヤーの回転制限
	float INIT_TIME = 0.1f; //初期時間
	float VIBRATION_TIME = 1.0f; //振動時間
	float MAX_FINISHMOVE_Y = MAX_POS.y + 50.0f; //移動終了時の最大X座標
	XMFLOAT3 CAM_ORBIT_POS = { 0.0f, 25.0f, 0.0f }; //カメラの初期位置
	float CAM_ORBIT_RADIUS = 100.0f; //カメラの初期半径
	float CAM_ORBIT_SPEED = 0.5f; //カメラの初期回転速度
	XMVECTOR FRONT_VEC = XMVectorSet(0, 0, 1, 0); //プレイヤーの前方ベクトル
	float VIBRATION_AMPLITUDE = 0.1f;      //振動の振幅
	float VIBRATION_FREQUENCY = 100.0f;    //振動の周波数
	float SMOOTH_MOVE_SCALE = 3.0f;       //滑らかな移動のスケール
}

//エフェクト関連の定数を定義
namespace EffectConstants {
	//Fireエフェクト用定数
	constexpr const char* FIRE_TEXTURE_FILE = "Effects/cloudA.png";
	constexpr XMFLOAT3 FIRE_POSITION = { -1.0f, -1.0f, -1.0f };
	constexpr XMFLOAT3 FIRE_POSITION_RND = { 0.1f, 0.0f, 0.1f };
	constexpr int FIRE_DELAY = 5;
	constexpr int FIRE_NUMBER = 1;
	constexpr int FIRE_LIFETIME = 60;
	constexpr float FIRE_GRAVITY = -0.002f;
	constexpr XMFLOAT3 FIRE_DIRECTION = { 0.0f, 1.0f, 0.0f };
	constexpr XMFLOAT3 FIRE_DIRECTION_RND = { 0.0f, 0.0f, 0.0f };
	constexpr float FIRE_SPEED = 0.01f;
	constexpr float FIRE_SPEED_RND = 0.0f;
	constexpr XMFLOAT2 FIRE_SIZE = { 1.5f, 1.5f };
	constexpr XMFLOAT2 FIRE_SIZE_RND = { 0.4f, 0.4f };
	constexpr XMFLOAT2 FIRE_SCALE = { 1.01f, 1.01f };
	constexpr XMFLOAT4 FIRE_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
	constexpr XMFLOAT4 FIRE_DELTA_COLOR = { 0.0f, -0.03f, 0.0f, -0.02f };

	//Sparkエフェクト用定数
	constexpr const char* SPARK_TEXTURE_FILE = "Effects/cloudA.png";
	constexpr int SPARK_NUMBER = 3;
	constexpr XMFLOAT3 SPARK_POSITION_RND = { 0.8f, 0.0f, 0.8f };
	constexpr XMFLOAT3 SPARK_DIRECTION = { 0.0f, 1.0f, 0.0f };
	constexpr XMFLOAT3 SPARK_DIRECTION_RND = { 10.0f, 10.0f, 10.0f };
	constexpr XMFLOAT2 SPARK_SIZE = { 0.2f, 0.2f };
	constexpr XMFLOAT2 SPARK_SCALE = { 0.95f, 0.95f };
	constexpr int SPARK_LIFETIME = 120;
	constexpr float SPARK_SPEED = 0.1f;
	constexpr float SPARK_GRAVITY = 0.0f;
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),state_(sBeforeStart),hModel_(-1), hLineSound_(-1), hLineDeleteSound_(-1), hBGM_(-1),
	cdTimer_(nullptr),cupsuleTimer_(nullptr),capsuleSpawnInterval_(CAPSULE_SPAWN_INTERVAL),
	lookTarget_{ 0,0,0 },rotX(XMMatrixIdentity()),rotY(XMMatrixIdentity()),move{XMVectorZero()}, rotVec{ XMVectorZero() },
	maxLineValue_(MAX_LINE_VALUE), currentLineValue_(0.0f), pCapsule_(nullptr),
	isPlayerHitting_(false),isInvisible_(false), isMoveStarted_(false), canControl_(false), pCameraOrbit_(nullptr),
	hFireEffect_(-1), hSparkEffect_(-1),vibrationTime_(0.0f)
{
}
void Player::Initialize()
{
	//プレイヤーモデルをロード
	hModel_ = Model::Load("Models/Player/Player.fbx");
	assert(hModel_ >= 0);
	//サウンドエフェクトをロード
	hLineSound_ = Audio::Load("Sounds/SoundEffect/LineSound.wav");
	assert(hLineSound_ >= 0);
	hLineDeleteSound_ = Audio::Load("Sounds/SoundEffect/LineDeleteSound.wav");
	assert(hLineDeleteSound_ >= 0);
	//BGMをロード（ループ再生に設定）
	hBGM_ = Audio::Load("Sounds/BGM/PlayBGM.wav", true);
	assert(hBGM_ >= 0);

	//カウントダウンタイマーを初期化
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(INIT_TIME);  //初期時間を設定

	//カメラのターゲット位置を設定
	SetCameraTarget(CAMERA_TARGET_VEC);
	//カメラの視点位置を設定
	SetCameraPos(CAMERA_EYE_VEC);

	//カプセルタイマーを初期化
	cupsuleTimer_ = Instantiate<CDTimer>(this);
	cupsuleTimer_->SetInitTime(capsuleSpawnInterval_);  //カプセル生成間隔を設定

	//プレイヤーのコライダーを作成
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), COLLIDER_RADIUS);
	AddCollider(collision);

	//エフェクトの設定
	SetFireEffect();
	SetSparkEffect();
}

void Player::Update()
{
	//プレイヤーの状態に応じて更新処理を実行
	switch (state_) {
	case sBeforeStart:	StartUpdate();		break;
	case sMove:			MoveUpdate();		break;
	case sMoveFinish:	MoveFinishUpdate(); break;
	case sResult:		ResultUpdate();		break;
	}

	//ライン残量の更新
	LineGaugeUpdate();
}

void Player::LineGaugeUpdate()
{
	LineGauge* pLineGauge_ = (LineGauge*)FindObject("LineGauge");
	if (pLineGauge_ == nullptr) return;
	pLineGauge_->SetMeterVal(maxLineValue_, currentLineValue_);
}

void Player::StartUpdate()
{
	//カメラのターゲット位置を設定
	SetCameraTarget(CAMERA_TARGET_VEC);
	//カメラの視点位置を設定
	SetCameraPos(CAMERA_EYE_VEC);

	//スタートカウントダウンマネージャーの初期化
	if (pStartCountdownManager_ == nullptr)
		pStartCountdownManager_ = (StartCountdownManager*)FindObject("StartCountdownManager");

	//プレイヤーの移動開始フラグ確認
	if (!isMoveStarted_)
	{
		//カウントダウンが終了している場合
		if (pStartCountdownManager_ != nullptr && pStartCountdownManager_->IsFinished())
		{
			isMoveStarted_ = true;  //移動を開始
			canControl_ = true;     //プレイヤーの操作を可能にする
			Audio::Play(hBGM_);     //BGMを再生
			state_ = sMove;         //プレイヤーの状態を「移動中」に変更
		}
	}
}

void Player::MoveUpdate()
{
	isPlayerHitting_ = false;//他の物体に衝突しているかのフラグリセット

	//プレイヤーの現在位置を取得
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	float dir = 1.0f;  //移動方向のスケール
	float deltaTime = cdTimer_->GetDeltaTime();  //経過時間を取得
	XMVECTOR addMove = dir * move * deltaTime;   //移動量を計算

	//プレイヤーの操作処理
	if (canControl_) {
		ProcessPlayerRotation(deltaTime);  //回転処理
		rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
		rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
		rotVec = XMVector3TransformCoord(FRONT_VEC, rotX * rotY);
		move = PLAYER_MOVE_SPEED * rotVec;  //移動速度を適用
		pos += addMove;  //現在位置に移動量を加算
	}

	//ボールとの距離チェック
	CheckCollisionWithBall(pos);

	//移動範囲を制限する
	XMFLOAT3 newPosition;
	XMStoreFloat3(&newPosition, pos);
	ConstrainPlayerMovement(newPosition);

	//プレイヤーの移動位置を適用
	transform_.position_ = newPosition;

	//エフェクトの更新処理
	UpdateEffects(rotX, rotY);

	//地面との衝突処理
	AdjustPlayerOnGround();

	//カメラ移動処理
	if (canControl_) {
		//カメラのターゲット位置を設定
		SetCameraTarget(CAMERA_TARGET_VEC);
		//カメラの視点位置を設定
		SetCameraPos(CAMERA_EYE_VEC);
	}

	//カプセルリセット処理とアクション処理
	if (canControl_) {
		HandlePlayerActions(deltaTime, addMove);
	}
}

void Player::MoveFinishUpdate()
{
	float deltaTime = cdTimer_->GetDeltaTime();

	if (transform_.rotate_.x > -ROTATE_MAX) {
		transform_.rotate_.x -= PLAYER_ROTATION_SPEED * deltaTime;  //回転速度を適用
	}
	else {
		//振動時間を更新
		vibrationTime_ += deltaTime;

		if (vibrationTime_ <= VIBRATION_TIME) {
			//プレイヤーが横方向に小刻みに振動
			ApplyVibration();
		}
		else {
			//プレイヤーを画面外に移動
			MovePlayerOutOfScreen(deltaTime);
		}
	}

	//プレイヤーが一定の高さ以上に移動した場合、状態を「結果表示」に遷移
	if (transform_.position_.y >= MAX_FINISHMOVE_Y) {
		HandleEndOfMovement();
	}
	else {
		//エフェクトの更新処理
		UpdateEffects(rotX, rotY);
	}
}

void Player::ResultUpdate()
{
	//カメラは指定された位置を中心にして、ステージ全体が見えるくらい離れてY軸回転
	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall == nullptr) return;
	pBall->BallMoveStart();

	//CameraOrbitの初期化と周回開始
	if (pCameraOrbit_ == nullptr) {
		pCameraOrbit_ = Instantiate<CameraOrbit>(this);
		pCameraOrbit_->SetOrbit(CAM_ORBIT_POS, CAM_ORBIT_RADIUS, CAM_ORBIT_SPEED);
	}

	//CameraOrbitの更新
	if (pCameraOrbit_ != nullptr) {
		float deltaTime = cdTimer_->GetDeltaTime();
		pCameraOrbit_->Update(deltaTime);
	}
}

void Player::SetCameraTarget(XMVECTOR targetVec)
{
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //プレイヤーのY軸回転を計算
	cameraTargetVec = targetVec;  //プレイヤーからのオフセット
	cameraTargetVec = XMVector3TransformCoord(cameraTargetVec, rotY);  //Y軸回転を適用
	XMStoreFloat3(&targetPos, XMVectorAdd(pos, cameraTargetVec));
	Camera::SetTarget(targetPos);  //カメラのターゲットを設定
}

void Player::SetCameraPos(XMVECTOR camVec)
{
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //プレイヤーのY軸回転を計算
	cameraEyeVec = camVec;  //プレイヤーからのオフセット
	cameraEyeVec = XMVector3TransformCoord(cameraEyeVec, rotY);  //Y軸回転を適用
	XMStoreFloat3(&camPos, XMVectorAdd(pos, cameraEyeVec));
	Camera::SetPosition(camPos);  //カメラの視点を設定
}

void Player::ProcessPlayerRotation(float deltaTime)
{
	//プレイヤーの回転処理
	if (Input::IsKey(DIK_UP)) {
		transform_.rotate_.x += PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (Input::IsKey(DIK_DOWN)) {
		transform_.rotate_.x -= PLAYER_ROTATION_SPEED * deltaTime;
	}
	transform_.rotate_.x = std::clamp(transform_.rotate_.x, -ROTATE_MAX, ROTATE_MAX);  //上下回転の制限

	if (Input::IsKey(DIK_LEFT)) {
		transform_.rotate_.y -= PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y += PLAYER_ROTATION_SPEED * deltaTime;
	}
}

void Player::CheckCollisionWithBall(XMVECTOR& pos)
{
	//ボールとの距離チェック
	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall != nullptr) {
		XMFLOAT3 ballFloatPos = pBall->GetPosition();
		XMVECTOR ballPos = XMLoadFloat3(&ballFloatPos);
		XMVECTOR diff = pos - ballPos;
		float distance = XMVectorGetX(XMVector3Length(diff));
		if (distance < MIN_DISTANCE_TO_BALL) {
			//ボールの表面に押し戻す
			XMVECTOR pushBack = XMVector3Normalize(diff) * (MIN_DISTANCE_TO_BALL - distance);
			pos += pushBack;
			isPlayerHitting_ = true;
		}
	}
}

void Player::ConstrainPlayerMovement(XMFLOAT3& position)
{
	//プレイヤーの移動範囲を制限
	if (CheckPlayerOutOfRange(position, MAX_POS, MIN_POS)) {
		position.x = std::clamp(position.x, MIN_POS.x, MAX_POS.x);
		position.y = std::clamp(position.y, MIN_POS.y, MAX_POS.y);
		position.z = std::clamp(position.z, MIN_POS.z, MAX_POS.z);
		isPlayerHitting_ = true;  //範囲外に出た場合、衝突フラグを立てる
	}
}

void Player::ApplyVibration()
{
	//振動エフェクトを適用
	XMVECTOR moveVec = XMVectorSet(VIBRATION_AMPLITUDE * sinf(XM_PI / 1 - vibrationTime_ * VIBRATION_FREQUENCY), 0.0f, 0.0f, 0.0f);
	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	moveVec = XMVector3TransformCoord(moveVec, rotX * rotY);

	//現在の位置に振動エフェクトを加算
	XMVECTOR playerPos = XMLoadFloat3(&(transform_.position_));
	playerPos += moveVec;
	XMStoreFloat3(&(transform_.position_), playerPos);
}

void Player::MovePlayerOutOfScreen(float deltaTime)
{
	//プレイヤーを画面外に移動
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	rotVec = XMVector3TransformCoord(FRONT_VEC, rotX * rotY);

	//滑らかな移動スケールを計算
	float t = std::clamp(vibrationTime_ - 1.0f, 0.0f, 1.0f);
	float smoothT = (1.0f - cos(t * XM_PI));  //滑らかな移動曲線
	move = PLAYER_MOVE_SPEED * rotVec * smoothT * SMOOTH_MOVE_SCALE;  //移動量をスケール
	XMVECTOR addMove = move * deltaTime;

	//現在位置に移動量を加算
	pos += addMove;
	XMStoreFloat3(&(transform_.position_), pos);
}

void Player::HandleEndOfMovement()
{
	isInvisible_ = true;//プレイヤーを透明にする

	//エフェクトを停止
	if (fireEffect_ != nullptr) {
		fireEffect_->StopEffect();
	}
	if (sparkEffect_ != nullptr) {
		sparkEffect_->StopEffect();
	}

	//状態を結果表示に変更
	state_ = sResult;
}

void Player::UpdateEffects(const XMMATRIX& rotX, const XMMATRIX& rotY)
{
	//エフェクトの生成と更新
	XMVECTOR backVec = XMVectorSet(0, 0, -1, 0);
	backVec = XMVector3TransformCoord(backVec, rotX * rotY);  //プレイヤーの回転を適用
	XMFLOAT3 backDir;
	XMStoreFloat3(&backDir, backVec);

	if (fireEffect_ == nullptr && sparkEffect_ == nullptr) {
		//エフェクトを生成
		fireEffect_ = Instantiate<Effect>(this);
		fireEffect_->SetEmitterData(fireEffectData_);
		sparkEffect_ = Instantiate<Effect>(this);
		sparkEffect_->SetEmitterData(sparkEffectData_);
	}
	else {
		//エフェクトの位置と向きを更新
		fireEffect_->SetEffectPosition(transform_.position_);
		fireEffect_->SetEffectDirection(backDir);
		sparkEffect_->SetEffectPosition(transform_.position_);
		sparkEffect_->SetEffectDirection(backDir);
	}
}

void Player::AdjustPlayerOnGround()
{
	//地面との衝突を防ぐ処理
	Stage* pStage = (Stage*)FindObject("Stage");
	if (pStage == nullptr) return;

	int hGroundModel = pStage->GetModelHandle();
	if (hGroundModel < 0) return;

	RayCastData data;
	data.start = transform_.position_;	  //レイの発射位置
	data.dir = XMFLOAT3(0, -1, 0);        //レイの方向
	Model::RayCast(hGroundModel, &data);  //レイキャストを実行

	//地面をすり抜けないように位置を調整
	if (data.dist < 1.0f) {
		transform_.position_.y += 1.0f - data.dist;
		isPlayerHitting_ = true;
	}
}

void Player::HandlePlayerActions(float deltaTime, XMVECTOR& addMove)
{
	//カプセルリセット処理
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)) {
		ClearCapsules();
		Audio::Play(hLineDeleteSound_);
	}

	//スペースキーでカプセルを配置
	if (Input::IsKey(DIK_SPACE) && !isPlayerHitting_) {
		if (currentLineValue_ <= maxLineValue_) {
			currentLineValue_ += XMVectorGetX(XMVector3Length(addMove));  //ラインゲージを増加
			if (cupsuleTimer_->IsTimeOver()) {
				pCapsule_ = GetCapsuleFromPool();
				pCapsule_->SetPosition(transform_.position_);
				pCapsule_->SetRotate(transform_.rotate_);
				cupsuleTimer_->ResetTimer();
				Audio::Play(hLineSound_);//ライン生成音を再生
			}
		}
	}

	//Enterキーで移動終了
	if (Input::IsKey(DIK_RETURN)) {
		canControl_ = false;//操作を無効化
		defaultPlayerPos_ = transform_.position_;
		state_ = sMoveFinish;//移動終了状態に遷移
	}
}


Line* Player::GetCapsuleFromPool()
{
	for (auto& capsule : capsulePool_) {
		if (!capsule->IsActive()) {  //非アクティブなカプセルを探す
			capsule->SetActive(true); //再利用時にアクティブ化
			capsule->SetPosition(transform_.position_);
			capsule->SetRotate(transform_.rotate_);

			capsuleList_.push_back(capsule); //カプセルリストに追加
			return capsule;
		}
	}

	//使えるカプセルがなければ新規作成
	Line* newCapsule = Instantiate<Line>(this->GetParent());
	newCapsule->SetActive(true);  //新規作成時にアクティブ化
	newCapsule->SetPosition(transform_.position_);
	newCapsule->SetRotate(transform_.rotate_);

	capsulePool_.push_back(newCapsule);
	capsuleList_.push_back(newCapsule);  //カプセルリストに追加
	return newCapsule;
}

void Player::ClearCapsules()
{
	for (auto& capsule : capsuleList_) {
		capsule->SetActive(false);  //非アクティブ化
		capsule->SetPosition({ 0, -1000, 0 });  //画面外へ移動
		capsulePool_.push_back(capsule);  //プールに戻す
	}
	capsuleList_.clear();  //画面上のカプセルリストをクリア

	currentLineValue_ = 0.0f;  //カウントリセット
}

bool Player::CheckPlayerOutOfRange(XMFLOAT3 playerPos, XMFLOAT3 maxPos, XMFLOAT3 minPos)
{
	if (playerPos.x > maxPos.x || playerPos.x < minPos.x ||
		playerPos.y > maxPos.y || playerPos.y < minPos.y ||
		playerPos.z > maxPos.z || playerPos.z < minPos.z) {
		return true;
	}
	return false;
}

void Player::SetFireEffect()
{
	fireEffectData_.textureFileName = EffectConstants::FIRE_TEXTURE_FILE;
	fireEffectData_.position = EffectConstants::FIRE_POSITION;
	fireEffectData_.positionRnd = EffectConstants::FIRE_POSITION_RND;
	fireEffectData_.delay = EffectConstants::FIRE_DELAY;
	fireEffectData_.number = EffectConstants::FIRE_NUMBER;
	fireEffectData_.lifeTime = EffectConstants::FIRE_LIFETIME;
	fireEffectData_.gravity = EffectConstants::FIRE_GRAVITY;
	fireEffectData_.direction = EffectConstants::FIRE_DIRECTION;
	fireEffectData_.directionRnd = EffectConstants::FIRE_DIRECTION_RND;
	fireEffectData_.speed = EffectConstants::FIRE_SPEED;
	fireEffectData_.speedRnd = EffectConstants::FIRE_SPEED_RND;
	fireEffectData_.size = EffectConstants::FIRE_SIZE;
	fireEffectData_.sizeRnd = EffectConstants::FIRE_SIZE_RND;
	fireEffectData_.scale = EffectConstants::FIRE_SCALE;
	fireEffectData_.color = EffectConstants::FIRE_COLOR;
	fireEffectData_.deltaColor = EffectConstants::FIRE_DELTA_COLOR;
}

void Player::SetSparkEffect()
{
	sparkEffectData_.textureFileName = EffectConstants::SPARK_TEXTURE_FILE;
	sparkEffectData_.number = EffectConstants::SPARK_NUMBER;
	sparkEffectData_.positionRnd = EffectConstants::SPARK_POSITION_RND;
	sparkEffectData_.direction = EffectConstants::SPARK_DIRECTION;
	sparkEffectData_.directionRnd = EffectConstants::SPARK_DIRECTION_RND;
	sparkEffectData_.size = EffectConstants::SPARK_SIZE;
	sparkEffectData_.scale = EffectConstants::SPARK_SCALE;
	sparkEffectData_.lifeTime = EffectConstants::SPARK_LIFETIME;
	sparkEffectData_.speed = EffectConstants::SPARK_SPEED;
	sparkEffectData_.gravity = EffectConstants::SPARK_GRAVITY;
}

void Player::Draw()
{
	if (!isInvisible_) {
		Model::SetTransform(hModel_, transform_);
		Model::Draw(hModel_);
	}
}

void Player::Release()
{
	//出現中のカプセルを削除
	for (auto& capsule : capsuleList_) {
		if (capsule && capsule->IsActive()) { //カプセルが存在していてアクティブな場合のみ削除
			capsule->KillMe();
		}
	}
	capsuleList_.clear(); //カプセルリストをクリア

	//プール内のカプセルも削除
	for (auto& capsule : capsulePool_) {
		if (capsule) { //ポインタが有効かチェック
			capsule->KillMe();
		}
	}
	capsulePool_.clear(); //プールのクリア
}