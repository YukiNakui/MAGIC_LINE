#include "TitleScene.h"
#include"Engine/Image.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hPict_(-1), hSelectSound_(-1),hBGM_(-1),cdTimer_(nullptr)
{
}

void TitleScene::Initialize()
{
	hPict_ = Image::Load("TITLE.png");
	assert(hPict_ >= 0);
	hSelectSound_ = Audio::Load("Sound/SelectSound.wav");
	assert(hSelectSound_ >= 0);
	hBGM_ = Audio::Load("Sound/TitleBGM.wav",true);
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		Audio::Stop(hBGM_);
		Audio::Play(hSelectSound_);
		cdTimer_ = Instantiate<CDTimer>(this);
		cdTimer_->SetInitTime(1.0f);
	}
	if (cdTimer_ != nullptr) {
		if (cdTimer_->IsTimeOver()) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}
	}
}

void TitleScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void TitleScene::Release()
{
}
