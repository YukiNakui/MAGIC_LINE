#include "ClearScene.h"
#include"../Engine/Image.h"
#include"../Engine/Input.h"
#include"../Engine/SceneManager.h"
#include"../Engine/Audio.h"

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent,"ClearScene"),hPict_(-1), hBGM_(-1)
{
}

void ClearScene::Initialize()
{
	//‰æ‘œ‚ÆBGM‚Ì“Ç‚Ýž‚Ý
	hPict_ = Image::Load("Scenes/CLEAR.png");
	assert(hPict_ >= 0);
	hBGM_ = Audio::Load("Sounds/BGM/ClearBGM.wav", true);
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);//BGM‚ðÄ¶
}

void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void ClearScene::Release()
{
}
