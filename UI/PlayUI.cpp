#include "PlayUI.h"
#include"../Engine/Image.h"

PlayUI::PlayUI(GameObject* parent)
	:GameObject(parent, "PlayUI"), hDrawPict_(-1), hDeletePict_(-1), hFinishPict_(-1), isDisplay_(false)
{
}

void PlayUI::Initialize()
{
	hDrawPict_ = Image::Load("UI/PlayerUI/Line_UI.png");
	assert(hDrawPict_ >= 0);
	hDeletePict_ = Image::Load("UI/PlayerUI/Delete_UI.png");
	assert(hDeletePict_ >= 0);
	hFinishPict_ = Image::Load("UI/PlayerUI/Finish_UI.png");
	assert(hFinishPict_ >= 0);
}

void PlayUI::Update()
{
}

void PlayUI::Draw()
{
	if (!isDisplay_) return;
	Image::SetTransform(hDrawPict_, lineUITrs_);
	Image::Draw(hDrawPict_);
	Image::SetTransform(hDeletePict_, deleteUITrs_);
	Image::Draw(hDeletePict_);
	Image::SetTransform(hFinishPict_, finishUITrs_);
	Image::Draw(hFinishPict_);
}

void PlayUI::Release()
{
}
