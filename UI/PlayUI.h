#pragma once
#include "../Engine/GameObject.h"

class PlayUI :
    public GameObject
{
    int hDrawPict_;
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

	enum PlayUIType {
		DRAW_LINE=1,
		DELETE_LINE,
		FINISH_LINE
	};
	PlayUIType playUIType_;

	void SetDrawUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
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

	void SetPlayUITransform(PlayUIType playUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (playUIType) {
		case DRAW_LINE:
			SetDrawUITransform(position, rotate, scale);
			break;
		case DELETE_LINE:
			SetDeleteUITransform(position, rotate, scale);
			break;
		case FINISH_LINE:
			SetFinishUITransform(position, rotate, scale);
			break;
		default:
			break;
		}
	}

private:
	bool isDisplay_;
public:
	void SetDisplay(bool display) { isDisplay_ = display; }
};

