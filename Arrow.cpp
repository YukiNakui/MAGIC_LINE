#include "Arrow.h"
#include"Engine/Model.h"
#include"Ball.h"
#include"Player.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), hModel_(-1),
	targetPos_({ 0.0f,0.0f,0.0f }), playerPos_({ 0.0f,0.0f,0.0f }),
	directionVec_({ 0.0f,0.0f,0.0f }), playerPosVec_({ 0.0f,0.0f,0.0f }),targetPosVec_({ 0.0f,0.0f,0.0f }),
    cdTimer_(nullptr)
{
}

void Arrow::Initialize()
{
	hModel_ = Model::Load("Model/SquarePyramid.fbx");
	assert(hModel_ >= 0);
	
    transform_.scale_ = { 2.0f,2.0f,2.0f };

    cdTimer_ = Instantiate<CDTimer>(this);
}

void Arrow::Update()
{
    //�{�[���̈ʒu�擾
    Ball* ball = (Ball*)FindObject("Ball");
    if (!ball) return;
    targetPos_ = ball->GetPosition();

    //�v���C���[�̈ʒu�擾
    Player* player = (Player*)FindObject("Player");
    if (!player) return;
    playerPos_ = player->GetPosition();

    //�x�N�g���ϊ�
    targetPosVec_ = XMLoadFloat3(&targetPos_);
    playerPosVec_ = XMLoadFloat3(&playerPos_);

    //�v���C���[����{�[���ւ̃x�N�g���i���K���j
    directionVec_ = XMVector3Normalize(targetPosVec_ - playerPosVec_);

    //���̕\���ʒu
    /*static float deltaTime = 0.0f;
    deltaTime = fmodf(deltaTime + cdTimer_->GetDeltaTime(), XM_2PI);

    float oscillation = sinf(deltaTime * 3.0f) * 0.5f;
    XMVECTOR offset = directionVec_ * 5.0f + XMVectorSet(0.0f, 0.0f, oscillation, 0.0f);
    XMStoreFloat3(&transform_.position_, playerPosVec_ + offset);*/

    // ��]�p�x�̌v�Z�i�����x�N�g������j
    float dx = targetPos_.x - playerPos_.x;
    float dy = targetPos_.y - playerPos_.y;
    float dz = targetPos_.z - playerPos_.z;

    float yaw = atan2f(dx, dz);             // ���E�iY����]�j
    float pitch = -atan2f(dy, sqrtf(dx * dx + dz * dz));  // �㉺�iX����]�j

    // Transform �ɓn���̂́u�x�v
    transform_.rotate_.x = XMConvertToDegrees(pitch);
    transform_.rotate_.y = XMConvertToDegrees(yaw);



    static float deltaTime = 0.0f;
    deltaTime = fmodf(deltaTime + cdTimer_->GetDeltaTime(), XM_2PI);

    float oscillation = sinf(deltaTime * 2.0f) * 1.0f;
    XMVECTOR offset = directionVec_ * (5.0f + oscillation);
    XMStoreFloat3(&transform_.position_, playerPosVec_ + offset);

    float spin = sinf(deltaTime * 2.0f);
    transform_.rotate_.z = spin * 180.0f;
}

void Arrow::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Arrow::Release()
{
}
