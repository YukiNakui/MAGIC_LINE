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
	XMVECTOR CAMERA_TARGET_VEC = XMVectorSet(0, 0, 25, 0); //�J�����̃^�[�Q�b�g�x�N�g��
	XMVECTOR CAMERA_EYE_VEC = XMVectorSet(0, 20, -30, 0); //�J�����̎��_�x�N�g��
	float PLAYER_MOVE_SPEED = 10.0f; //�v���C���[�̈ړ����x
	float PLAYER_ROTATION_SPEED = 60.0f; //�v���C���[�̉�]���x
	float MIN_DISTANCE_TO_BALL = 7.5f; //�{�[���ƃv���C���[�̍ŏ�����
	float COLLIDER_RADIUS = 1.5f; //�v���C���[�̃R���C�_�[���a
	float CAPSULE_SPAWN_INTERVAL = 0.1f; //�J�v�Z�������Ԋu
	float MAX_LINE_VALUE = 100.0f; //���C���Q�[�W�̍ő�l
	XMFLOAT3 MAX_POS = { 45.0f, 50.0f, 45.0f }; //�v���C���[�̍ő�ʒu
	XMFLOAT3 MIN_POS = { -45.0f, 0.0f, -45.0f }; //�v���C���[�̍ŏ��ʒu
	float ROTATE_MAX = 45.0f; //�v���C���[�̉�]����
	float INIT_TIME = 0.1f; //��������
	float VIBRATION_TIME = 1.0f; //�U������
	float MAX_FINISHMOVE_Y = MAX_POS.y + 50.0f; //�ړ��I�����̍ő�X���W
	XMFLOAT3 CAM_ORBIT_POS = { 0.0f, 25.0f, 0.0f }; //�J�����̏����ʒu
	float CAM_ORBIT_RADIUS = 100.0f; //�J�����̏������a
	float CAM_ORBIT_SPEED = 0.5f; //�J�����̏�����]���x
	XMVECTOR FRONT_VEC = XMVectorSet(0, 0, 1, 0); //�v���C���[�̑O���x�N�g��
	float VIBRATION_AMPLITUDE = 0.1f;      //�U���̐U��
	float VIBRATION_FREQUENCY = 100.0f;    //�U���̎��g��
	float SMOOTH_MOVE_SCALE = 3.0f;       //���炩�Ȉړ��̃X�P�[��
}

//�G�t�F�N�g�֘A�̒萔���`
namespace EffectConstants {
	//Fire�G�t�F�N�g�p�萔
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

	//Spark�G�t�F�N�g�p�萔
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
	//�v���C���[���f�������[�h
	hModel_ = Model::Load("Models/Player/Player.fbx");
	assert(hModel_ >= 0);
	//�T�E���h�G�t�F�N�g�����[�h
	hLineSound_ = Audio::Load("Sounds/SoundEffect/LineSound.wav");
	assert(hLineSound_ >= 0);
	hLineDeleteSound_ = Audio::Load("Sounds/SoundEffect/LineDeleteSound.wav");
	assert(hLineDeleteSound_ >= 0);
	//BGM�����[�h�i���[�v�Đ��ɐݒ�j
	hBGM_ = Audio::Load("Sounds/BGM/PlayBGM.wav", true);
	assert(hBGM_ >= 0);

	//�J�E���g�_�E���^�C�}�[��������
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(INIT_TIME);  //�������Ԃ�ݒ�

	//�J�����̃^�[�Q�b�g�ʒu��ݒ�
	SetCameraTarget(CAMERA_TARGET_VEC);
	//�J�����̎��_�ʒu��ݒ�
	SetCameraPos(CAMERA_EYE_VEC);

	//�J�v�Z���^�C�}�[��������
	cupsuleTimer_ = Instantiate<CDTimer>(this);
	cupsuleTimer_->SetInitTime(capsuleSpawnInterval_);  //�J�v�Z�������Ԋu��ݒ�

	//�v���C���[�̃R���C�_�[���쐬
	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), COLLIDER_RADIUS);
	AddCollider(collision);

	//�G�t�F�N�g�̐ݒ�
	SetFireEffect();
	SetSparkEffect();
}

void Player::Update()
{
	//�v���C���[�̏�Ԃɉ����čX�V���������s
	switch (state_) {
	case sBeforeStart:	StartUpdate();		break;
	case sMove:			MoveUpdate();		break;
	case sMoveFinish:	MoveFinishUpdate(); break;
	case sResult:		ResultUpdate();		break;
	}

	//���C���c�ʂ̍X�V
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
	//�J�����̃^�[�Q�b�g�ʒu��ݒ�
	SetCameraTarget(CAMERA_TARGET_VEC);
	//�J�����̎��_�ʒu��ݒ�
	SetCameraPos(CAMERA_EYE_VEC);

	//�X�^�[�g�J�E���g�_�E���}�l�[�W���[�̏�����
	if (pStartCountdownManager_ == nullptr)
		pStartCountdownManager_ = (StartCountdownManager*)FindObject("StartCountdownManager");

	//�v���C���[�̈ړ��J�n�t���O�m�F
	if (!isMoveStarted_)
	{
		//�J�E���g�_�E�����I�����Ă���ꍇ
		if (pStartCountdownManager_ != nullptr && pStartCountdownManager_->IsFinished())
		{
			isMoveStarted_ = true;  //�ړ����J�n
			canControl_ = true;     //�v���C���[�̑�����\�ɂ���
			Audio::Play(hBGM_);     //BGM���Đ�
			state_ = sMove;         //�v���C���[�̏�Ԃ��u�ړ����v�ɕύX
		}
	}
}

void Player::MoveUpdate()
{
	isPlayerHitting_ = false;//���̕��̂ɏՓ˂��Ă��邩�̃t���O���Z�b�g

	//�v���C���[�̌��݈ʒu���擾
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	float dir = 1.0f;  //�ړ������̃X�P�[��
	float deltaTime = cdTimer_->GetDeltaTime();  //�o�ߎ��Ԃ��擾
	XMVECTOR addMove = dir * move * deltaTime;   //�ړ��ʂ��v�Z

	//�v���C���[�̑��쏈��
	if (canControl_) {
		ProcessPlayerRotation(deltaTime);  //��]����
		rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
		rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
		rotVec = XMVector3TransformCoord(FRONT_VEC, rotX * rotY);
		move = PLAYER_MOVE_SPEED * rotVec;  //�ړ����x��K�p
		pos += addMove;  //���݈ʒu�Ɉړ��ʂ����Z
	}

	//�{�[���Ƃ̋����`�F�b�N
	CheckCollisionWithBall(pos);

	//�ړ��͈͂𐧌�����
	XMFLOAT3 newPosition;
	XMStoreFloat3(&newPosition, pos);
	ConstrainPlayerMovement(newPosition);

	//�v���C���[�̈ړ��ʒu��K�p
	transform_.position_ = newPosition;

	//�G�t�F�N�g�̍X�V����
	UpdateEffects(rotX, rotY);

	//�n�ʂƂ̏Փˏ���
	AdjustPlayerOnGround();

	//�J�����ړ�����
	if (canControl_) {
		//�J�����̃^�[�Q�b�g�ʒu��ݒ�
		SetCameraTarget(CAMERA_TARGET_VEC);
		//�J�����̎��_�ʒu��ݒ�
		SetCameraPos(CAMERA_EYE_VEC);
	}

	//�J�v�Z�����Z�b�g�����ƃA�N�V��������
	if (canControl_) {
		HandlePlayerActions(deltaTime, addMove);
	}
}

void Player::MoveFinishUpdate()
{
	float deltaTime = cdTimer_->GetDeltaTime();

	if (transform_.rotate_.x > -ROTATE_MAX) {
		transform_.rotate_.x -= PLAYER_ROTATION_SPEED * deltaTime;  //��]���x��K�p
	}
	else {
		//�U�����Ԃ��X�V
		vibrationTime_ += deltaTime;

		if (vibrationTime_ <= VIBRATION_TIME) {
			//�v���C���[���������ɏ����݂ɐU��
			ApplyVibration();
		}
		else {
			//�v���C���[����ʊO�Ɉړ�
			MovePlayerOutOfScreen(deltaTime);
		}
	}

	//�v���C���[�����̍����ȏ�Ɉړ������ꍇ�A��Ԃ��u���ʕ\���v�ɑJ��
	if (transform_.position_.y >= MAX_FINISHMOVE_Y) {
		HandleEndOfMovement();
	}
	else {
		//�G�t�F�N�g�̍X�V����
		UpdateEffects(rotX, rotY);
	}
}

void Player::ResultUpdate()
{
	//�J�����͎w�肳�ꂽ�ʒu�𒆐S�ɂ��āA�X�e�[�W�S�̂������邭�炢�����Y����]
	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall == nullptr) return;
	pBall->BallMoveStart();

	//CameraOrbit�̏������Ǝ���J�n
	if (pCameraOrbit_ == nullptr) {
		pCameraOrbit_ = Instantiate<CameraOrbit>(this);
		pCameraOrbit_->SetOrbit(CAM_ORBIT_POS, CAM_ORBIT_RADIUS, CAM_ORBIT_SPEED);
	}

	//CameraOrbit�̍X�V
	if (pCameraOrbit_ != nullptr) {
		float deltaTime = cdTimer_->GetDeltaTime();
		pCameraOrbit_->Update(deltaTime);
	}
}

void Player::SetCameraTarget(XMVECTOR targetVec)
{
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //�v���C���[��Y����]���v�Z
	cameraTargetVec = targetVec;  //�v���C���[����̃I�t�Z�b�g
	cameraTargetVec = XMVector3TransformCoord(cameraTargetVec, rotY);  //Y����]��K�p
	XMStoreFloat3(&targetPos, XMVectorAdd(pos, cameraTargetVec));
	Camera::SetTarget(targetPos);  //�J�����̃^�[�Q�b�g��ݒ�
}

void Player::SetCameraPos(XMVECTOR camVec)
{
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));  //�v���C���[��Y����]���v�Z
	cameraEyeVec = camVec;  //�v���C���[����̃I�t�Z�b�g
	cameraEyeVec = XMVector3TransformCoord(cameraEyeVec, rotY);  //Y����]��K�p
	XMStoreFloat3(&camPos, XMVectorAdd(pos, cameraEyeVec));
	Camera::SetPosition(camPos);  //�J�����̎��_��ݒ�
}

void Player::ProcessPlayerRotation(float deltaTime)
{
	//�v���C���[�̉�]����
	if (Input::IsKey(DIK_UP)) {
		transform_.rotate_.x += PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (Input::IsKey(DIK_DOWN)) {
		transform_.rotate_.x -= PLAYER_ROTATION_SPEED * deltaTime;
	}
	transform_.rotate_.x = std::clamp(transform_.rotate_.x, -ROTATE_MAX, ROTATE_MAX);  //�㉺��]�̐���

	if (Input::IsKey(DIK_LEFT)) {
		transform_.rotate_.y -= PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (Input::IsKey(DIK_RIGHT)) {
		transform_.rotate_.y += PLAYER_ROTATION_SPEED * deltaTime;
	}
}

void Player::CheckCollisionWithBall(XMVECTOR& pos)
{
	//�{�[���Ƃ̋����`�F�b�N
	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall != nullptr) {
		XMFLOAT3 ballFloatPos = pBall->GetPosition();
		XMVECTOR ballPos = XMLoadFloat3(&ballFloatPos);
		XMVECTOR diff = pos - ballPos;
		float distance = XMVectorGetX(XMVector3Length(diff));
		if (distance < MIN_DISTANCE_TO_BALL) {
			//�{�[���̕\�ʂɉ����߂�
			XMVECTOR pushBack = XMVector3Normalize(diff) * (MIN_DISTANCE_TO_BALL - distance);
			pos += pushBack;
			isPlayerHitting_ = true;
		}
	}
}

void Player::ConstrainPlayerMovement(XMFLOAT3& position)
{
	//�v���C���[�̈ړ��͈͂𐧌�
	if (CheckPlayerOutOfRange(position, MAX_POS, MIN_POS)) {
		position.x = std::clamp(position.x, MIN_POS.x, MAX_POS.x);
		position.y = std::clamp(position.y, MIN_POS.y, MAX_POS.y);
		position.z = std::clamp(position.z, MIN_POS.z, MAX_POS.z);
		isPlayerHitting_ = true;  //�͈͊O�ɏo���ꍇ�A�Փ˃t���O�𗧂Ă�
	}
}

void Player::ApplyVibration()
{
	//�U���G�t�F�N�g��K�p
	XMVECTOR moveVec = XMVectorSet(VIBRATION_AMPLITUDE * sinf(XM_PI / 1 - vibrationTime_ * VIBRATION_FREQUENCY), 0.0f, 0.0f, 0.0f);
	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	moveVec = XMVector3TransformCoord(moveVec, rotX * rotY);

	//���݂̈ʒu�ɐU���G�t�F�N�g�����Z
	XMVECTOR playerPos = XMLoadFloat3(&(transform_.position_));
	playerPos += moveVec;
	XMStoreFloat3(&(transform_.position_), playerPos);
}

void Player::MovePlayerOutOfScreen(float deltaTime)
{
	//�v���C���[����ʊO�Ɉړ�
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	rotVec = XMVector3TransformCoord(FRONT_VEC, rotX * rotY);

	//���炩�Ȉړ��X�P�[�����v�Z
	float t = std::clamp(vibrationTime_ - 1.0f, 0.0f, 1.0f);
	float smoothT = (1.0f - cos(t * XM_PI));  //���炩�Ȉړ��Ȑ�
	move = PLAYER_MOVE_SPEED * rotVec * smoothT * SMOOTH_MOVE_SCALE;  //�ړ��ʂ��X�P�[��
	XMVECTOR addMove = move * deltaTime;

	//���݈ʒu�Ɉړ��ʂ����Z
	pos += addMove;
	XMStoreFloat3(&(transform_.position_), pos);
}

void Player::HandleEndOfMovement()
{
	isInvisible_ = true;//�v���C���[�𓧖��ɂ���

	//�G�t�F�N�g���~
	if (fireEffect_ != nullptr) {
		fireEffect_->StopEffect();
	}
	if (sparkEffect_ != nullptr) {
		sparkEffect_->StopEffect();
	}

	//��Ԃ����ʕ\���ɕύX
	state_ = sResult;
}

void Player::UpdateEffects(const XMMATRIX& rotX, const XMMATRIX& rotY)
{
	//�G�t�F�N�g�̐����ƍX�V
	XMVECTOR backVec = XMVectorSet(0, 0, -1, 0);
	backVec = XMVector3TransformCoord(backVec, rotX * rotY);  //�v���C���[�̉�]��K�p
	XMFLOAT3 backDir;
	XMStoreFloat3(&backDir, backVec);

	if (fireEffect_ == nullptr && sparkEffect_ == nullptr) {
		//�G�t�F�N�g�𐶐�
		fireEffect_ = Instantiate<Effect>(this);
		fireEffect_->SetEmitterData(fireEffectData_);
		sparkEffect_ = Instantiate<Effect>(this);
		sparkEffect_->SetEmitterData(sparkEffectData_);
	}
	else {
		//�G�t�F�N�g�̈ʒu�ƌ������X�V
		fireEffect_->SetEffectPosition(transform_.position_);
		fireEffect_->SetEffectDirection(backDir);
		sparkEffect_->SetEffectPosition(transform_.position_);
		sparkEffect_->SetEffectDirection(backDir);
	}
}

void Player::AdjustPlayerOnGround()
{
	//�n�ʂƂ̏Փ˂�h������
	Stage* pStage = (Stage*)FindObject("Stage");
	if (pStage == nullptr) return;

	int hGroundModel = pStage->GetModelHandle();
	if (hGroundModel < 0) return;

	RayCastData data;
	data.start = transform_.position_;	  //���C�̔��ˈʒu
	data.dir = XMFLOAT3(0, -1, 0);        //���C�̕���
	Model::RayCast(hGroundModel, &data);  //���C�L���X�g�����s

	//�n�ʂ����蔲���Ȃ��悤�Ɉʒu�𒲐�
	if (data.dist < 1.0f) {
		transform_.position_.y += 1.0f - data.dist;
		isPlayerHitting_ = true;
	}
}

void Player::HandlePlayerActions(float deltaTime, XMVECTOR& addMove)
{
	//�J�v�Z�����Z�b�g����
	if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)) {
		ClearCapsules();
		Audio::Play(hLineDeleteSound_);
	}

	//�X�y�[�X�L�[�ŃJ�v�Z����z�u
	if (Input::IsKey(DIK_SPACE) && !isPlayerHitting_) {
		if (currentLineValue_ <= maxLineValue_) {
			currentLineValue_ += XMVectorGetX(XMVector3Length(addMove));  //���C���Q�[�W�𑝉�
			if (cupsuleTimer_->IsTimeOver()) {
				pCapsule_ = GetCapsuleFromPool();
				pCapsule_->SetPosition(transform_.position_);
				pCapsule_->SetRotate(transform_.rotate_);
				cupsuleTimer_->ResetTimer();
				Audio::Play(hLineSound_);//���C�����������Đ�
			}
		}
	}

	//Enter�L�[�ňړ��I��
	if (Input::IsKey(DIK_RETURN)) {
		canControl_ = false;//����𖳌���
		defaultPlayerPos_ = transform_.position_;
		state_ = sMoveFinish;//�ړ��I����ԂɑJ��
	}
}


Line* Player::GetCapsuleFromPool()
{
	for (auto& capsule : capsulePool_) {
		if (!capsule->IsActive()) {  //��A�N�e�B�u�ȃJ�v�Z����T��
			capsule->SetActive(true); //�ė��p���ɃA�N�e�B�u��
			capsule->SetPosition(transform_.position_);
			capsule->SetRotate(transform_.rotate_);

			capsuleList_.push_back(capsule); //�J�v�Z�����X�g�ɒǉ�
			return capsule;
		}
	}

	//�g����J�v�Z�����Ȃ���ΐV�K�쐬
	Line* newCapsule = Instantiate<Line>(this->GetParent());
	newCapsule->SetActive(true);  //�V�K�쐬���ɃA�N�e�B�u��
	newCapsule->SetPosition(transform_.position_);
	newCapsule->SetRotate(transform_.rotate_);

	capsulePool_.push_back(newCapsule);
	capsuleList_.push_back(newCapsule);  //�J�v�Z�����X�g�ɒǉ�
	return newCapsule;
}

void Player::ClearCapsules()
{
	for (auto& capsule : capsuleList_) {
		capsule->SetActive(false);  //��A�N�e�B�u��
		capsule->SetPosition({ 0, -1000, 0 });  //��ʊO�ֈړ�
		capsulePool_.push_back(capsule);  //�v�[���ɖ߂�
	}
	capsuleList_.clear();  //��ʏ�̃J�v�Z�����X�g���N���A

	currentLineValue_ = 0.0f;  //�J�E���g���Z�b�g
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
	//�o�����̃J�v�Z�����폜
	for (auto& capsule : capsuleList_) {
		if (capsule && capsule->IsActive()) { //�J�v�Z�������݂��Ă��ăA�N�e�B�u�ȏꍇ�̂ݍ폜
			capsule->KillMe();
		}
	}
	capsuleList_.clear(); //�J�v�Z�����X�g���N���A

	//�v�[�����̃J�v�Z�����폜
	for (auto& capsule : capsulePool_) {
		if (capsule) { //�|�C���^���L�����`�F�b�N
			capsule->KillMe();
		}
	}
	capsulePool_.clear(); //�v�[���̃N���A
}