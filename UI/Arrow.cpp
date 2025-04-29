#include "Arrow.h"
#include"../Engine/Model.h"
#include"../Objects/Ball.h"
#include"../Objects/Player.h"

namespace {
    const float OSCILLATION_FREQUENCY = 2.0f; // �U���̎��g��
    const float OSCILLATION_AMPLITUDE = 1.0f; // �U���̐U��
    const float OFFSET_DISTANCE = 5.0f;      // �I�t�Z�b�g����
    const float SPIN_MULTIPLIER = 180.0f;    // ��]�p�x�̔{��
}

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), hModel_(-1),
	targetPos_({ 0.0f,0.0f,0.0f }), playerPos_({ 0.0f,0.0f,0.0f }),
	directionVec_({ 0.0f,0.0f,0.0f }), playerPosVec_({ 0.0f,0.0f,0.0f }),targetPosVec_({ 0.0f,0.0f,0.0f }),
	cdTimer_(nullptr), isDisplay_(false)
{
}

void Arrow::Initialize()
{
	hModel_ = Model::Load("Models/UI/SquarePyramid.fbx");
	assert(hModel_ >= 0);
	
    cdTimer_ = Instantiate<CDTimer>(this);
}

void Arrow::Update()
{
    // �{�[���̈ʒu�擾
    Ball* ball = (Ball*)FindObject("Ball");
    if (!ball) return;
    targetPos_ = ball->GetPosition();

    // �v���C���[�̈ʒu�擾
    Player* player = (Player*)FindObject("Player");
    if (!player) return;
    playerPos_ = player->GetPosition();

    // �x�N�g���ϊ�
    targetPosVec_ = XMLoadFloat3(&targetPos_);
    playerPosVec_ = XMLoadFloat3(&playerPos_);

    // �v���C���[����{�[���ւ̃x�N�g���i���K���j
    directionVec_ = XMVector3Normalize(targetPosVec_ - playerPosVec_);


    // ��]�p�x�̌v�Z�i�����x�N�g������j
    float dx = targetPos_.x - playerPos_.x;
    float dy = targetPos_.y - playerPos_.y;
    float dz = targetPos_.z - playerPos_.z;

    float yaw = atan2f(dx, dz);             // ���E�iY����]�j
    float pitch = -atan2f(dy, sqrtf(dx * dx + dz * dz));  // �㉺�iX����]�j

    transform_.rotate_.x = XMConvertToDegrees(pitch);
    transform_.rotate_.y = XMConvertToDegrees(yaw);


    static float deltaTime = 0.0f;
    deltaTime = fmodf(deltaTime + cdTimer_->GetDeltaTime(), XM_2PI);

    // �I�t�Z�b�g�ƐU���̌v�Z
    float oscillation = sinf(deltaTime * OSCILLATION_FREQUENCY) * OSCILLATION_AMPLITUDE;
    XMVECTOR offset = directionVec_ * (OFFSET_DISTANCE + oscillation);
    XMStoreFloat3(&transform_.position_, playerPosVec_ + offset);

    // ��]�̌v�Z
    float spin = sinf(deltaTime * OSCILLATION_FREQUENCY);
    transform_.rotate_.z = spin * SPIN_MULTIPLIER;
}

void Arrow::Draw()
{
	if (!isDisplay_) return;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
