#pragma once
#include "../Engine/GameObject.h"

class PlayUI :
    public GameObject
{
    int hLinePict_;
    int hDeletePict_;
    int hFinishPict_;
	Transform lineUITrs_;
	Transform deleteUITrs_;
	Transform finishUITrs_;
public:
	PlayUI(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetLineUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		lineUITrs_.position_ = position;
		lineUITrs_.rotate_ = rotate;
		lineUITrs_.scale_ = scale;
	}

	void SetDeleteUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		deleteUITrs_.position_ = position;
		deleteUITrs_.rotate_ = rotate;
		deleteUITrs_.scale_ = scale;
	}
	
	void SetFinishUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		finishUITrs_.position_ = position;
		finishUITrs_.rotate_ = rotate;
		finishUITrs_.scale_ = scale;
	}

	bool isDisplay_;
public:
	void SetDisplay(bool display) { isDisplay_ = display; }
};

