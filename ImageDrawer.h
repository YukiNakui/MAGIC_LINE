#pragma once
#include"Engine/GameObject.h"
#include"Functions/CDTimer.h"

class ImageDrawer
	:public GameObject 
{
	int hImage;
public:
	ImageDrawer(GameObject* parent);
	~ImageDrawer();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override {}
	void LoadFile(std::string filename, int line);
	void SetPosition(int x, int y);
	void Move(int x, int y, float time);
private:
	XMFLOAT3 pos;
	XMFLOAT3 targetPos;
	float moveTime;
	XMFLOAT3 startPos;
	float currentTime;
	CDTimer* cdTimer_;
};

