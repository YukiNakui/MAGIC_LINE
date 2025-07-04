#include "ScriptExecuter.h"

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
	if (waitTimer_ > 0) {
		if (cdTimer_ != nullptr)
			waitTimer_ -= cdTimer_->GetDeltaTime();
		return;
	}
	if (executeLine_ >= script_->GetLines())
		return;
	std::string command = script_->GetString(executeLine_, 0);
	if (command == "END")
		return;

	if (command == "IMAGE") {
		int id = script_->GetInt(executeLine_, 1);
		if (images.count(id) > 0) {
			//���łɊG��ǂ�ł���
			images[id]->KillMe();
		}
		ImageDrawer* pImage = Instantiate<ImageDrawer>(GetParent());//ScriptExecute�Ɠ����K�w�ɂ���
		images[id] = pImage;
		pImage->LoadFile(script_->GetString(executeLine_, 2), executeLine_);
		pImage->SetPosition(script_->GetInt(executeLine_, 3), script_->GetInt(executeLine_, 4));
	}
	else if (command == "WAIT") {
		waitTimer_ = script_->GetFloat(executeLine_, 5);
	}
	else if (command == "MOVE") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 1)) == 0) {
			std::string str = std::to_string(executeLine_ + 1) + "�s�ڂ�";
			str += script_->GetString(executeLine_, 2) + "�͂���܂���";
			MessageBox(NULL, str.c_str(), "MOVE�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 1)];
		pImage->Move(script_->GetInt(executeLine_, 3), script_->GetInt(executeLine_, 4), script_->GetFloat(executeLine_, 5));
	}
	else {
		//�܂�����ĂȂ��R�}���h
		std::string str = std::to_string(executeLine_ + 1) + "�s�ڂ�";
		str += command + "�͂���܂���";
		MessageBox(NULL, str.c_str(), "���m�̃R�}���h", MB_OK);
	}
	executeLine_ += 1;
}

void ScriptExecuter::LoadFile(std::string fileName)
{
	script_ = new CsvReader("CSV/TutorialScript/" + fileName + ".csv");
	assert(script_ != nullptr && script_->GetLines() > 0);//null����Ȃ��čŒ�1�s�ȏ㏑����Ă���
	executeLine_ = 1;//1�s�ڂ̓w�b�_�Ȃ̂Ŏ��̍s������s����
}
