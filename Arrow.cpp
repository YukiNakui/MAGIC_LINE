#include "Arrow.h"
#include"Engine/Image.h"

Arrow::Arrow(GameObject* parent)
	:GameObject(parent, "Arrow"), hArrowPict_(-1)
{
}

void Arrow::Initialize()
{
	hArrowPict_ = Image::Load("ArrowUI.png");
	assert(hArrowPict_ >= 0);
}

void Arrow::Update()
{
}

void Arrow::Draw()
{
	Image::SetTransform(hArrowPict_, transform_);
	Image::Draw(hArrowPict_);
}

void Arrow::Release()
{
}
