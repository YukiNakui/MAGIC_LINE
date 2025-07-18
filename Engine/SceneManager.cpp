#include "sceneManager.h"

#include "../Scenes/TitleScene.h"
#include "../Scenes/ExplanationScene.h"
#include"../Scenes/StageSelectScene.h"
#include "../Scenes/PlayScene.h"
#include"../Scenes/ClearScene.h"
#include"../Scenes/GameOverScene.h"
#include "Model.h"
#include "Image.h"
#include "Audio.h"

//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

//初期化
void SceneManager::Initialize()
{
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();
		
		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_EXPLANATION: Instantiate<ExplanationScene>(this); break;
		case SCENE_ID_STAGESELECT: Instantiate<StageSelectScene>(this); break;
		case SCENE_ID_PLAY: {
			PlayScene* pPlay = InstantiateEx<PlayScene>(this, nextStageFileName_);
			break;
		}
		case SCENE_ID_CLEAR: Instantiate<ClearScene>(this); break;
		case SCENE_ID_GAMEOVER: Instantiate<GameOverScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//描画
void SceneManager::Draw()
{
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}

void SceneManager::ChangeToPlaySceneWithFile(const std::string& fileName)
{
	nextStageFileName_ = fileName;
	ChangeScene(SCENE_ID_PLAY);
}
