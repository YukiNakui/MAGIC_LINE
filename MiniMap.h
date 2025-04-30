#pragma once
#include "Engine/GameObject.h"
#include<vector>

class MiniMap :
    public GameObject
{
	int hMiniMapFramePict_;
	int hMiniMapBallPict_;
	int hMiniMapPlayerPict_;
	int hMiniMapLinePict_;
	Transform miniMapFrameTrs_;
	Transform miniMapBallTrs_;
	Transform miniMapPlayerTrs_;
	Transform miniMapLineTrs_;

	std::vector<Transform> miniMapCapsuleTrs_;

	XMFLOAT3 defaultPlayerPosition_;
	XMFLOAT3 defaultBallPosition_;
	XMFLOAT3 defaultLinePosition_;
public:
	MiniMap(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	

	enum MiniMapUIType
	{
		MiniMapFrame=1,
		MiniMapBall,
		MiniMapPlayer,
		MiniMapLine,
	};

	void SetMiniMapFrameTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapFrameTrs_.position_ = position;
		miniMapFrameTrs_.rotate_ = rotate;
		miniMapFrameTrs_.scale_ = scale;
	}

	void SetMiniMapBallTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapBallTrs_.position_ = position;
		miniMapBallTrs_.rotate_ = rotate;
		miniMapBallTrs_.scale_ = scale;
		defaultBallPosition_ = position;
	}

	void SetMiniMapPlayerTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapPlayerTrs_.position_ = position;
		miniMapPlayerTrs_.rotate_ = rotate;
		miniMapPlayerTrs_.scale_ = scale;
		defaultPlayerPosition_ = position;
	}

	void SetMiniMapLineTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		miniMapLineTrs_.position_ = position;
		miniMapLineTrs_.rotate_ = rotate;
		miniMapLineTrs_.scale_ = scale;
		defaultLinePosition_ = position;
	}

	void SetMiniMapUITransform(MiniMapUIType miniMapUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (miniMapUIType) {
		case MiniMapFrame:
			SetMiniMapFrameTransform(position, rotate, scale);
			break;
		case MiniMapBall:
			SetMiniMapBallTransform(position, rotate, scale);
			break;
		case MiniMapPlayer:
			SetMiniMapPlayerTransform(position, rotate, scale);
			break;
		case MiniMapLine:
			SetMiniMapLineTransform(position, rotate, scale);
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

