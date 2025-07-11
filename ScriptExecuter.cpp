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
	if (pThemeDisplay != nullptr && !pThemeDisplay->IsMoveFinished())//����\�����������Ă��Ȃ��Ȃ�
		return;

	if (waitTimer_ > 0) {//�ҋ@���Ȃ��
		if (cdTimer_ != nullptr)
			waitTimer_ -= cdTimer_->GetDeltaTime();//�o�ߎ��Ԃ�����
		return;
	}

	if (Input::IsKeyDown(DIK_RETURN)) {
		if (command == "MOVE") {
			ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
			pImage->MoveFinish(); //�����I�ɍŏI�n�_�Ɉړ�������
		}
		executeLine_ += 1; //Enter�L�[�Ŏ��̍s��
	}

	if (executeLine_ >= script_->GetLines())
		return;//�X�N���v�g�̏I�[�ɒB�����牽�����Ȃ�
	command = script_->GetString(executeLine_, 0);
	if (command == "END") {
		isEnd_ = true;
		return;
	}


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
		pImage->SetScale(script_->GetFloat(executeLine_, 5), script_->GetFloat(executeLine_, 6));
		pImage->SetAlpha(script_->GetInt(executeLine_, 7));
		executeLine_ += 1; //���̍s��
	}
	else if (command == "MOVE") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "�R�}���h���s�s��", "MOVE�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->Move(script_->GetFloat(executeLine_, 3), script_->GetFloat(executeLine_, 4), script_->GetFloat(executeLine_, 8));
		if(pImage->IsMoveFinished())
			executeLine_ += 1; //���̍s��
	}
	else if (command == "DELETE") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "�R�}���h���s�s��", "DELETE�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->KillMe(); //�G���폜
		executeLine_ += 1; //���̍s��
	}
	else if (command == "SET ORDER") {
		//�G�����[�h���Ă��Ȃ�������?
		if (images.count(script_->GetInt(executeLine_, 2)) == 0) {
			MessageBox(NULL, "�R�}���h���s�s��", "SET ORDER�ł��܂���", MB_OK);
		}
		ImageDrawer* pImage = images[script_->GetInt(executeLine_, 2)];
		pImage->SetRenderOrder(script_->GetInt(executeLine_, 9));
		executeLine_ += 1; //���̍s��
	}
	else if (command == "STOP") {
		isWait_ = true; //�ҋ@��Ԃɂ���
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
