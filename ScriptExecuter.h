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
	int executeLine_; //実行中の行
	std::unordered_map<int, ImageDrawer*> images;
	CDTimer* cdTimer_;
	float waitTimer_; //待機時間
	std::string command; //現在のコマンド
	bool isWait_; //待機中かどうか
	bool isEnd_; //スクリプトの終端に達したかどうか
};

