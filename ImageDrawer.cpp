#include "ImageDrawer.h"
#include"Engine/Image.h"

ImageDrawer::ImageDrawer(GameObject* parent)
	: GameObject(parent, "ImageDrawer"),
	hImage(-1),cdTimer_(nullptr)
{
}

ImageDrawer::~ImageDrawer()
{
}

void ImageDrawer::Initialize()
{
	currentTime = 1.0f;
	moveTime = currentTime;
	cdTimer_ = Instantiate<CDTimer>(this);
}

void ImageDrawer::Update()
{
	if (currentTime >= moveTime) {
		MoveFinish(); //�ړ��������Ă���Ȃ�΋����I�ɍŏI�n�_�Ɉړ�������
		return;
	}
	currentTime += cdTimer_->GetDeltaTime();
	if (currentTime > moveTime)
		currentTime = moveTime;
	float rate = currentTime / moveTime;//�⊮ ���������Ԃ������ɂ���
	//	rate = pow(rate, 1.0f); rate�ɂ��낢�낷��Ɖ����⌸�����ł���pos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.y = (targetPos.y - startPos.y) * rate + startPos.y;

	transform_.position_ = pos; //Transform�Ɉʒu���Z�b�g
	transform_.scale_ = imageScale; //Transform�ɃX�P�[�����Z�b�g
}

void ImageDrawer::Draw()
{
	Image::SetTransform(hImage, transform_);
	Image::Draw(hImage);
}

void ImageDrawer::LoadFile(std::string filename, int line)
{
	hImage = Image::Load(filename);//c_str��char�^�ɂ���
	if (hImage < 0) {
		std::string str = "";
		str += std::to_string(line + 1) + "�s��C���";
		str += filename + "���ǂ߂܂���";
		MessageBox(NULL, str.c_str(), "IMAGE�t�@�C�����ǂ߂܂���", MB_OK);
		assert(false);//�����A�T�[�g
	}
	assert(hImage > 0);
}

void ImageDrawer::SetPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
	pos.z = 0;
	targetPos.x = x;
	targetPos.y = y;
	targetPos.z = 0;
	transform_.position_ = pos; //Transform�Ɉʒu���Z�b�g
}

void ImageDrawer::SetScale(float x, float y)
{
	imageScale.x = x;
	imageScale.y = y;
	imageScale.z = 0;
	transform_.scale_ = imageScale; //Transform�ɃX�P�[�����Z�b�g
}

void ImageDrawer::SetAlpha(int alpha)
{
	Image::SetAlpha(hImage, alpha);
}

void ImageDrawer::Transform(float posX, float posY, float scaleX, float scaleY, float time)
{
	targetPos.x = posX;
	targetPos.y = posY;
	targetPos.z = 0;
	moveTime = time;
	startPos = pos;

	SetScale(scaleX, scaleY); //�X�P�[�����Z�b�g
	currentTime = 0;
}

void ImageDrawer::MoveFinish()
{
	pos.x = targetPos.x;
	pos.y = targetPos.y;
	pos.z = 0;
	currentTime = moveTime; //�����I�Ɉړ�����
	transform_.position_ = pos;
}
