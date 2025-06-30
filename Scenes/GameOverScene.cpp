#include "GameOverScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Audio.h"

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene"), hSelectSound_(-1), hBGM_(-1), cdTimer_(nullptr)
{
}

void GameOverScene::Initialize()
{
	//�摜��BGM�̓ǂݍ���
	hPict_ = Image::Load("Scenes/GAMEOVER.png");
	assert(hPict_ >= 0);
	hSelectSound_ = Audio::Load("Sounds/SoundEffect/SelectSound.wav");
	assert(hSelectSound_ >= 0);
	hBGM_ = Audio::Load("Sounds/BGM/GameOverBGM.wav", true);
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);//BGM���Đ�
}

void GameOverScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		Audio::Stop(hBGM_);//BGM��~
		Audio::Play(hSelectSound_);//SE�Đ�
		cdTimer_ = Instantiate<CDTimer>(this);
		cdTimer_->SetInitTime(1.0f);
	}
	if (cdTimer_ != nullptr) {
		if (cdTimer_->IsTimeOver()) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_STAGESELECT);//�V�[���ύX
		}
	}
}

void GameOverScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void GameOverScene::Release()
{
}
