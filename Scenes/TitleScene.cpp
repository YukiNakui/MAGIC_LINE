#include "TitleScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Audio.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene"),hPict_(-1), hSelectSound_(-1),hBGM_(-1),cdTimer_(nullptr)
{
}

void TitleScene::Initialize()
{
	//画像と音声の読み込み
	hPict_ = Image::Load("Scenes/TITLE.png");
	assert(hPict_ >= 0);
	hSelectSound_ = Audio::Load("Sounds/SoundEffect/SelectSound.wav");
	assert(hSelectSound_ >= 0);
	hBGM_ = Audio::Load("Sounds/BGM/TitleBGM.wav",true);
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);//BGM再生
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		Audio::Stop(hBGM_);//BGM停止
		Audio::Play(hSelectSound_);//SE再生
		cdTimer_ = Instantiate<CDTimer>(this);
		cdTimer_->SetInitTime(1.0f);
	}
	if (cdTimer_ != nullptr) {
		if (cdTimer_->IsTimeOver()) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_STAGESELECT);//シーン変更
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
