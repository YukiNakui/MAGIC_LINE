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
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	void SetAlpha(int alpha);
	void Transform(float posX, float posY, float scaleX, float scaleY, float time);
	void MoveFinish();//強制的に最終地点に移動させる
	bool IsMoveFinished() const { return currentTime >= moveTime; }
private:
	XMFLOAT3 pos;
	XMFLOAT3 targetPos;
	float moveTime;
	XMFLOAT3 startPos;
	XMFLOAT3 imageScale;
	float currentTime;
	CDTimer* cdTimer_;
};

