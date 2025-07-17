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
		MoveFinish(); //移動完了しているならば強制的に最終地点に移動させる
		return;
	}
	currentTime += cdTimer_->GetDeltaTime();
	if (currentTime > moveTime)
		currentTime = moveTime;
	float rate = currentTime / moveTime;//補完 動いた時間を割合にする
	//	rate = pow(rate, 1.0f); rateにいろいろすると加速や減速ができるpos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.y = (targetPos.y - startPos.y) * rate + startPos.y;

	transform_.position_ = pos; //Transformに位置をセット
	transform_.scale_ = imageScale; //Transformにスケールをセット
}

void ImageDrawer::Draw()
{
	Image::SetTransform(hImage, transform_);
	Image::Draw(hImage);
}

void ImageDrawer::LoadFile(std::string filename, int line)
{
	hImage = Image::Load(filename);//c_strでchar型にする
	if (hImage < 0) {
		std::string str = "";
		str += std::to_string(line + 1) + "行目C列の";
		str += filename + "が読めません";
		MessageBox(NULL, str.c_str(), "IMAGEファイルが読めません", MB_OK);
		assert(false);//強制アサート
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
	transform_.position_ = pos; //Transformに位置をセット
}

void ImageDrawer::SetScale(float x, float y)
{
	imageScale.x = x;
	imageScale.y = y;
	imageScale.z = 0;
	transform_.scale_ = imageScale; //Transformにスケールをセット
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

	SetScale(scaleX, scaleY); //スケールをセット
	currentTime = 0;
}

void ImageDrawer::MoveFinish()
{
	pos.x = targetPos.x;
	pos.y = targetPos.y;
	pos.z = 0;
	currentTime = moveTime; //強制的に移動完了
	transform_.position_ = pos;
}
