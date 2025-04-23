#include "PlayUI.h"
#include"../Engine/Image.h"

PlayUI::PlayUI(GameObject* parent)
	:GameObject(parent,"PlayUI"),hLinePict_(-1), hDeletePict_(-1), hFinishPict_(-1)
{
}

void PlayUI::Initialize()
{
	hLinePict_ = Image::Load("UI/PlayerUI/Line_UI.png");
	assert(hLinePict_ >= 0);
	hDeletePict_ = Image::Load("UI/PlayerUI/Delete_UI.png");
	assert(hDeletePict_ >= 0);
	hFinishPict_ = Image::Load("UI/PlayerUI/Finish_UI.png");
	assert(hFinishPict_ >= 0);

	lineUITrs_.position_ = {-0.9f,-0.8f,0.0f};
	lineUITrs_.scale_ = { 0.2f,0.2f,0.2f };
	deleteUITrs_.position_ = { -0.7f,-0.81f,0.0f };
	deleteUITrs_.scale_ = { 0.2f,0.2f,0.2f };
	finishUITrs_.position_ = { -0.5f,-0.8f,0.0f };
	finishUITrs_.scale_ = { 0.2f,0.2f,0.2f };
}

void PlayUI::Update()
{
}

void PlayUI::Draw()
{
	Image::SetTransform(hLinePict_, lineUITrs_);
	Image::Draw(hLinePict_);
	Image::SetTransform(hDeletePict_, deleteUITrs_);
	Image::Draw(hDeletePict_);
	Image::SetTransform(hFinishPict_, finishUITrs_);
	Image::Draw(hFinishPict_);
}

void PlayUI::Release()
{
}
