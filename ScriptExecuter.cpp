#include "ScriptExecuter.h"
#include"Engine/Input.h"
#include"UI/ThemeDisplay.h"

ScriptExecuter::ScriptExecuter(GameObject* parent)
	:GameObject(parent,"ScriptExecuter"),
	script_(nullptr)
{
}

ScriptExecuter::~ScriptExecuter()
{
	if (script_ != nullptr) {
		delete script_;
		script_ = nullptr;
	}
}

void ScriptExecuter::Initialize()
{
	cdTimer_ = Instantiate<CDTimer>(this);
	waitTimer_ = 0.0f;
	isEnd_ = false;
	isWait_ = false;
}

void ScriptExecuter::Update()
{
	ThemeDisplay* pThemeDisplay = (ThemeDisplay*)FindObject("ThemeDisplay");
	if (pThemeDisplay != nullptr && !pThemeDisplay->IsMoveFinished())//お題表示が完了していないなら
		return;

	if (waitTimer_ > 0) {//待機中ならば
		if (cdTimer_ != nullptr)
			waitTimer_ -= cdTimer_->GetDeltaTime();//経過時間を引く
		return;
	}

	if (Input::IsKeyDown(DIK_RETURN)) {
		if (command == "MOVE") {
			ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
			pImage->MoveFinish(); //強制的に最終地点に移動させる
		}
		executeLine_ += 1; //Enterキーで次の行へ
	}

	if (executeLine_ >= script_->GetLines())
		return;//スクリプトの終端に達したら何もしない
	command = script_->GetString(executeLine_, 0);
	if (command == "END") {
		isEnd_ = true;
		return;
	}


	if (command == "IMAGE") {
		int id = script_->GetInt(executeLine_, 2);
		if (images.count(id) > 0) {
			//すでに絵を読んでいる
			images[id]->KillMe();
		}
		ImageDrawer* pImage = Instantiate<ImageDrawer>(GetParent());//ScriptExecuteと同じ階層にする
		images[id] = pImage;
		pImage->LoadFile("UI/Tutorial/"+script_->GetString(executeLine_, 1)+".png", executeLine_);
		pImage->SetPosition(script_->GetFloat(executeLine_, 3), script_->GetFloat(executeLine_, 4));
		pImage->SetScale(script_->GetFloat(executeLine_, 5), script_->GetFloat(executeLine_, 6));
		pImage->SetAlpha(script_->GetInt(executeLine_, 7));
		executeLine_ += 1; //次の行へ
	}
	else if (command == "MOVE") {
		//絵をロードしていなかったら?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "コマンド実行不可", "MOVEできません", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->Move(script_->GetFloat(executeLine_, 3), script_->GetFloat(executeLine_, 4), script_->GetFloat(executeLine_, 8));
		if(pImage->IsMoveFinished())
			executeLine_ += 1; //次の行へ
	}
	else if (command == "DELETE") {
		//絵をロードしていなかったら?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "コマンド実行不可", "DELETEできません", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->KillMe(); //絵を削除
		executeLine_ += 1; //次の行へ
	}
	else if (command == "SET ORDER") {
		//絵をロードしていなかったら?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "コマンド実行不可", "SET ORDERできません", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->SetRenderOrder(script_->GetInt(executeLine_, 9));
		executeLine_ += 1; //次の行へ
	}
	else if (command == "STOP") {
		isWait_ = true; //待機状態にする
	}
	else {
		//まだ作ってないコマンド
		std::string str = std::to_string(executeLine_ + 1) + "行目の";
		str += command + "はありません";
		MessageBox(NULL, str.c_str(), "未知のコマンド", MB_OK);
	}
}

void ScriptExecuter::LoadFile(std::string fileName)
{
	script_ = new CsvReader("CSV/TutorialScript/" + fileName + ".csv");
	assert(script_ != nullptr && script_->GetLines() > 0);//nullじゃなくて最低1行以上書かれている
	executeLine_ = 1;//1行目はヘッダなので次の行から実行する
}
