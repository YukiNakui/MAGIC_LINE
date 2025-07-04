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
	if (currentTime >= moveTime)
		return;
	currentTime += cdTimer_->GetDeltaTime();
	if (currentTime > moveTime)
		currentTime = moveTime;
	float rate = currentTime / moveTime;//補完 動いた時間を割合にする
	//	rate = pow(rate, 1.0f); rateにいろいろすると加速や減速ができるpos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.x = (targetPos.x - startPos.x) * rate + startPos.x;
	pos.y = (targetPos.y - startPos.y) * rate + startPos.y;
}

void ImageDrawer::Draw()
{
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

void ImageDrawer::SetPosition(int x, int y)
{
	pos.x = x;
	pos.y = y;
	pos.z = 0;
}

void ImageDrawer::Move(int x, int y, float time)
{
	targetPos.x = x;
	targetPos.y = y;
	targetPos.z = 0;
	moveTime = time;
	startPos = pos;
	currentTime = 0;
}
