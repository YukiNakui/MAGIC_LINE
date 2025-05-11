#include "HeightMeter.h"
#include"../Engine/Image.h"
#include"../Objects/Player.h"

namespace {
	const float MAXHEIGHT = 50.0f;       //�ő卂��
	const float MINHEIGHT = 0.0f;       //�ŏ�����
	const float POSITION_SCALE_FACTOR = 0.02f;   //Y�����̈ʒu�X�P�[���W��
	const float POSITION_BASE_OFFSET = 0.47f;    //Y�����̈ʒu�I�t�Z�b�g
}

HeightMeter::HeightMeter(GameObject* parent)
	:GameObject(parent, "HeightMeter"), hMeterPict_(-1), hTrianglePict_(-1),
	maxHeight_(0.0f), minHeight_(0.0f), currentHeight_(0.0f), isDisplay_(false)
{
}

void HeightMeter::Initialize()
{
	hMeterPict_ = Image::Load("UI/PlayerUI/HeightMeter.png");
	assert(hMeterPict_ >= 0);
	hTrianglePict_ = Image::Load("UI/PlayerUI/Triangle.png");
	assert(hTrianglePict_ >= 0);

	SetMaxAndMinHeight(MAXHEIGHT,MINHEIGHT);
}

void HeightMeter::Update()
{
	Player* player = (Player*)FindObject("Player");
	if(!player) return;
	currentHeight_ = player->GetPosition().y;
	//�v���C���[�̍����Ɋ�Â��ĎO�p�`��Y���W���v�Z
	triangleUITrs_.position_.y =
		(currentHeight_ - (maxHeight_ - minHeight_)) * POSITION_SCALE_FACTOR + POSITION_BASE_OFFSET + meterUITrs_.position_.y;
}

void HeightMeter::Draw()
{
	if (!isDisplay_) return;
	Image::SetTransform(hMeterPict_, meterUITrs_);
	Image::Draw(hMeterPict_);
	Image::SetTransform(hTrianglePict_, triangleUITrs_);
	Image::Draw(hTrianglePict_);
}

void HeightMeter::Release()
{
}
