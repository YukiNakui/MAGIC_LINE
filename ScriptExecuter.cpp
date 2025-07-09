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
	if (waitTimer_ > 0) {//�ҋ@���Ȃ��
		if (cdTimer_ != nullptr)
			waitTimer_ -= cdTimer_->GetDeltaTime();//�o�ߎ��Ԃ�����
		return;
	}

	if (Input::IsKeyDown(DIK_RETURN)) {
		if (command == "MOVE") {
			ImageDrawer* pImage = images[script_->GetInt(executeLine_, 1)];
			pImage->MoveFinish(); //�����I�ɍŏI�n�_�Ɉړ�������
		}
		executeLine_ += 1; //Enter�L�[�Ŏ��̍s��
	}

	if (executeLine_ >= script_->GetLines())
		return;//�X�N���v�g�̏I�[�ɒB�����牽�����Ȃ�
	command = script_->GetString(executeLine_, 0);
	if (command == "END")
		return;


	

	if (command == "IMAGE") {
		int id = script_->GetInt(executeLine_, 2);
		if (images.count(id) > 0) {
			//���łɊG��ǂ�ł���
			images[id]->KillMe();
		}
		ImageDrawer* pImage = Instantiate<ImageDrawer>(GetParent());//ScriptExecute�Ɠ����K�w�ɂ���
		images[id] = pImage;
		pImage->LoadFile("UI/Tutorial/"+script_->GetString(executeLine_, 1)+".png", executeLine_);
		pImage->SetPosition(script_->GetFloat(executeLine_, 3), script_->GetFloat(executeLine_, 4));
	}
	else if (command == "MOVE") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			std::string str = std::to_string(executeLine_ + 1) + "�s�ڂ�";
			str += script_->GetString(executeLine_, 1) + "�͂���܂���";
			MessageBox(NULL, str.c_str(), "MOVE�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->Move(script_->GetFloat(executeLine_, 3), script_->GetFloat(executeLine_, 4), script_->GetFloat(executeLine_, 7));
	}
	else if (command == "DELETE") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			std::string str = std::to_string(executeLine_ + 1) + "�s�ڂ�";
			str += script_->GetString(executeLine_, 1) + "�͂���܂���";
			MessageBox(NULL, str.c_str(), "DELETE�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->KillMe(); //�G���폜
	}
	else {
		//�܂�����ĂȂ��R�}���h
		std::string str = std::to_string(executeLine_ + 1) + "�s�ڂ�";
		str += command + "�͂���܂���";
		MessageBox(NULL, str.c_str(), "���m�̃R�}���h", MB_OK);
	}
}

void ScriptExecuter::LoadFile(std::string fileName)
{
	script_ = new CsvReader("CSV/TutorialScript/" + fileName + ".csv");
	assert(script_ != nullptr && script_->GetLines() > 0);//null����Ȃ��čŒ�1�s�ȏ㏑����Ă���
	executeLine_ = 1;//1�s�ڂ̓w�b�_�Ȃ̂Ŏ��̍s������s����
}
