#include "ExplanationScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Audio.h"

ExplanationScene::ExplanationScene(GameObject* parent)
	:GameObject(parent, "ExplanationScene"), hSelectSound_(-1), cdTimer_(nullptr)
{
}

void ExplanationScene::Initialize()
{
	//âÊëúÇ∆å¯â âπÇÃì«Ç›çûÇ›
	hPict_ = Image::Load("Scenes/Explanation.png");
	assert(hPict_ >= 0);
	hSelectSound_ = Audio::Load("Sounds/SoundEffect/SelectSound.wav");
	assert(hSelectSound_ >= 0);

	transform_.position_ = IMAGE_TRANS_POS;
	transform_.scale_ = IMAGE_TRANS_SCALE;
}

void ExplanationScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		Audio::Play(hSelectSound_);//SEçƒê∂
		cdTimer_ = Instantiate<CDTimer>(this);
		cdTimer_->SetInitTime(1.0f);
	}
	if (cdTimer_ != nullptr) {
		if (cdTimer_->IsTimeOver()) {
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);//ÉVÅ[ÉìïœçX
		}
	}
}

void ExplanationScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void ExplanationScene::Release()
{
}
