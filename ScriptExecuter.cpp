#include "ScriptExecuter.h"
#include"Engine/Input.h"

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
}

void ScriptExecuter::Update()
{
	if (waitTimer_ > 0) {//待機中ならば
		if (cdTimer_ != nullptr)
			waitTimer_ -= cdTimer_->GetDeltaTime();//経過時間を引く
		return;
	}
	if (executeLine_ >= script_->GetLines())
		return;//スクリプトの終端に達したら何もしない
	std::string command = script_->GetString(executeLine_, 0);
	if (command == "END")
		return;

	if (Input::IsKeyDown(DIK_RETURN)) {
		executeLine_ += 1; //Enterキーで次の行へ
	}


	if (command == "IMAGE") {
		int id = script_->GetInt(executeLine_, 2);
		if (images.count(id) > 0) {
			//すでに絵を読んでいる
			images[id]->KillMe();
		}
		ImageDrawer* pImage = Instantiate<ImageDrawer>(GetParent());//ScriptExecuteと同じ階層にする
		images[id] = pImage;
		pImage->LoadFile(script_->GetString(executeLine_, 1), executeLine_);
		pImage->SetPosition(script_->GetInt(executeLine_, 3), script_->GetInt(executeLine_, 4));
	}
	else if (command == "MOVE") {
		//絵をロードしていなかったら?
		if (images.count(script_->GetInt(executeLine_, 1)) == 0) {
			std::string str = std::to_string(executeLine_ + 1) + "行目の";
			str += script_->GetString(executeLine_, 2) + "はありません";
			MessageBox(NULL, str.c_str(), "MOVEできません", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 1)];
		pImage->Move(script_->GetInt(executeLine_, 3), script_->GetInt(executeLine_, 4), script_->GetFloat(executeLine_, 5));
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
