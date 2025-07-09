#pragma once
#include "Engine/GameObject.h"
#include"Engine/CsvReader.h"
#include"ImageDrawer.h"
#include<unordered_map>
#include "Functions/CDTimer.h"

class ScriptExecuter :
    public GameObject
{
public:
	ScriptExecuter(GameObject* parent);
	~ScriptExecuter();
	void Initialize() override;
	void Update() override;
	void Draw() override {}
	void Release() override {}
	void LoadFile(std::string fileName);
	bool IsEnd() const { return isEnd_; }
private:
	CsvReader* script_;
	int executeLine_; //���s���̍s
	std::unordered_map<int, ImageDrawer*> images;
	CDTimer* cdTimer_;
	float waitTimer_; //�ҋ@����
	std::string command; //���݂̃R�}���h
	bool isWait_; //�ҋ@�����ǂ���
	bool isEnd_; //�X�N���v�g�̏I�[�ɒB�������ǂ���
};

