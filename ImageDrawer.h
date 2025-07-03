#pragma once
#include"Engine/GameObject.h"

class ImageDrawer :public GameObject {
public:
	ImageDrawer(GameObject* parent);
	~ImageDrawer();
	void Update() override;
	void Draw() override;
	void LoadFile(std::string filename, int line);
	void SetPosition(int x, int y);
	void Move(int x, int y, float time);
private:
	int hImage;
	Transform position;
	Transform target;
	float moveTime;
	Transform start;
	float currentTime;
};

