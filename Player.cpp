#include "Player.h"
#include"Engine/Model.h"
#include"Engine/Input.h"
#include"Engine/Camera.h"
#include"Engine/SphereCollider.h"
#include<algorithm>
#include "Engine/SceneManager.h"
#include"Stage.h"
#include"Meter.h"
#include"Engine/Audio.h"
#include"Ball.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"),hModel_(-1), hLineSound_(-1), hLineDeleteSound_(-1), hBGM_(-1), cdTimer_(nullptr),cupsuleTimer_(nullptr),capsuleSpawnInterval_(0.1f),
	 lookTarget_{ 0,0,0 },front_{0,0,1,0},maxLineValue_(100.0f),currentLineValue_(0.0f),pCapsule_(nullptr),pCountStart_(nullptr),
	maxPos_(45.0f,30.0f,45.0f), minPos_(-45.0f, -60.0f, -45.0f)
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Model/Player.fbx");
	assert(hModel_ >= 0);

	hLineSound_ = Audio::Load("Sound/LineSound.wav");
	assert(hLineSound_ >= 0);
	hLineDeleteSound_ = Audio::Load("Sound/LineDeleteSound.wav");
	assert(hLineDeleteSound_ >= 0);
	hBGM_ = Audio::Load("Sound/PlayBGM.wav",true);
	assert(hBGM_ >= 0);

	transform_.position_ = { 0.0f,10.0f,-30.0f };
	cdTimer_ = Instantiate<CDTimer>(this);
	cdTimer_->SetInitTime(0.1f);

	cupsuleTimer_ = Instantiate<CDTimer>(this);
	cupsuleTimer_->SetInitTime(capsuleSpawnInterval_);

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 1.5f);
	AddCollider(collision);

	canMove_ = false;
}

void Player::Update()
{
	if (pCountStart_ == nullptr) {
		pCountStart_ = (CountStart*)FindObject("CountStart");
	}

	if (!canMove_)
	{
		if (pCountStart_ != nullptr && !pCountStart_->IsStartVisible())
		{
			canMove_ = true;  //START����������ړ��J�n
			Audio::Play(hBGM_);
		}
	}

	
	XMMATRIX rotX = XMMatrixIdentity();
	XMMATRIX rotY = XMMatrixIdentity();
	XMVECTOR move{ 0, 0, 0, 0 };
	XMVECTOR rotVec{ 0, 0, 0, 0 };
	float dir = 1.0f;
	float deltaTime = cdTimer_->GetDeltaTime();
	if (canMove_) {
		if (Input::IsKey(DIK_UP)) {
			transform_.rotate_.x += 60.0f * deltaTime;
		}
		if (Input::IsKey(DIK_DOWN)) {
			transform_.rotate_.x -= 60.0f * deltaTime;
		}
		transform_.rotate_.x = std::clamp(transform_.rotate_.x, -45.0f, 45.0f);

		if (Input::IsKey(DIK_LEFT)) {
			transform_.rotate_.y -= 60.0f * deltaTime;
		}
		if (Input::IsKey(DIK_RIGHT)) {
			transform_.rotate_.y += 60.0f * deltaTime;
		}

		//�v���C���[�ړ�����
		rotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
		rotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
		rotVec = XMVector3TransformCoord(front_, rotX * rotY);
		move = 10.0f * rotVec;
	}
	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));
	XMVECTOR addMove = dir * move * deltaTime;
	pos += addMove;


	//�{�[���Ƃ̋����`�F�b�N
	Ball* pBall = (Ball*)FindObject("Ball");
	if (pBall != nullptr)
	{
		XMFLOAT3 ballFloatPos = pBall->GetPosition();
		XMVECTOR ballPos = XMLoadFloat3(&ballFloatPos);
		XMVECTOR diff = pos - ballPos;
		float distance = XMVectorGetX(XMVector3Length(diff));

		float minDistance = 7.5f; //�{�[���ƃv���C���[�̍ŏ������i�v���C���[�̃R���C�_�[���a + �{�[���̔��a)

		if (distance < minDistance)
		{
			//�{�[���̕\�ʂɉ����߂�
			XMVECTOR pushBack = XMVector3Normalize(diff) * (minDistance - distance);
			pos += pushBack;
		}
	}


	// **�ړ���������**
	XMFLOAT3 newPosition;
	XMStoreFloat3(&newPosition, pos);

	newPosition.x = std::clamp(newPosition.x, minPos_.x, maxPos_.x);
	newPosition.y = std::clamp(newPosition.y, minPos_.y, maxPos_.y);
	newPosition.z = std::clamp(newPosition.z, minPos_.z, maxPos_.z);
	// **�ړ���K�p**
	transform_.position_ = newPosition;

	//�J�����ړ�����
	XMVECTOR vTarget{ 0, 0, 15, 0 };
	vTarget = XMVector3TransformCoord(vTarget, rotY);
	XMFLOAT3 targetPos;
	XMStoreFloat3(&targetPos, XMVectorAdd(pos, vTarget));
	Camera::SetTarget(transform_.position_);
	XMVECTOR vEye{ 0, 10, -20, 0 };
	vEye = XMVector3TransformCoord(vEye, rotY);
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, XMVectorAdd(pos, vEye));
	Camera::SetPosition(camPos);

	if (canMove_) {
		//**�J�v�Z�����Z�b�g����**
		if (Input::IsKeyDown(DIK_LSHIFT) || Input::IsKeyDown(DIK_RSHIFT)) { // �V�t�g�L�[�ŃJ�v�Z�������Z�b�g
			ClearCapsules();
			Audio::Play(hLineDeleteSound_);
		}

		if (Input::IsKey(DIK_SPACE)) {
			if (currentLineValue_ <= maxLineValue_) {
				currentLineValue_ += XMVectorGetX(XMVector3Length(addMove));
				if (cupsuleTimer_->IsTimeOver()) {
					pCapsule_ = GetCapsuleFromPool();

					pCapsule_->SetPosition(transform_.position_);
					pCapsule_->SetRotate(transform_.rotate_);
					cupsuleTimer_->ResetTimer();

					Audio::Play(hLineSound_);
				}
			}
		}
	}

	Meter* pMeter_ = (Meter*)FindObject("Meter");
	pMeter_->SetMeterVal(maxLineValue_, currentLineValue_);

	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
	int hGroundModel = pStage->GetModelHandle();    //���f���ԍ����擾

	RayCastData data;
	data.start = transform_.position_;   //���C�̔��ˈʒu
	data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
	Model::RayCast(hGroundModel, &data); //���C�𔭎�
	
	//�n�ʂ����蔲���Ȃ��悤�ɂ���
	if (data.dist < 1.0f) {
		transform_.position_.y += 1.0f - data.dist;
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Player::Release()
{
	// **�o�����̃J�v�Z�����폜**
	for (auto& capsule : capsuleList_) {
		if (capsule && capsule->IsActive()) { // **�J�v�Z�������݂��Ă��ăA�N�e�B�u�ȏꍇ�̂ݍ폜**
			capsule->KillMe();
		}
	}
	capsuleList_.clear(); // **�J�v�Z�����X�g���N���A**

	// **�v�[�����̃J�v�Z�����폜**
	for (auto& capsule : capsulePool_) {
		if (capsule) { // **�|�C���^���L�����`�F�b�N**
			capsule->KillMe();
		}
	}
	capsulePool_.clear(); // **�v�[���̃N���A**
}

void Player::OnCollision(GameObject* pTarget)
{
}

Capsule* Player::GetCapsuleFromPool()
{
	for (auto& capsule : capsulePool_) {
		if (!capsule->IsActive()) {  // ��A�N�e�B�u�ȃJ�v�Z����T��
			capsule->SetActive(true); // �ė��p���ɃA�N�e�B�u��
			capsule->SetPosition(transform_.position_);
			capsule->SetRotate(transform_.rotate_);

			capsuleList_.push_back(capsule); // **�J�v�Z�����X�g�ɒǉ�**
			return capsule;
		}
	}

	// �g����J�v�Z�����Ȃ���ΐV�K�쐬
	Capsule* newCapsule = Instantiate<Capsule>(this->GetParent());
	newCapsule->SetActive(true);  // �V�K�쐬���ɃA�N�e�B�u��
	newCapsule->SetPosition(transform_.position_);
	newCapsule->SetRotate(transform_.rotate_);

	capsulePool_.push_back(newCapsule);
	capsuleList_.push_back(newCapsule);  // **�J�v�Z�����X�g�ɒǉ�**
	return newCapsule;
}

void Player::ClearCapsules()
{
	for (auto& capsule : capsuleList_) {
		capsule->SetActive(false);  // **��A�N�e�B�u��**
		capsule->SetPosition({ 0, -1000, 0 });  // ��ʊO�ֈړ�
		capsulePool_.push_back(capsule);  // �v�[���ɖ߂�
	}
	capsuleList_.clear();  // ��ʏ�̃J�v�Z�����X�g���N���A

	currentLineValue_ = 0.0f;  // �J�E���g���Z�b�g
}