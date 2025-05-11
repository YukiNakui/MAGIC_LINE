#pragma once
#include "../Engine/GameObject.h"
#include<vector>

class MiniMap :
    public GameObject
{
	//ミニマップのUI要素画像のハンドル
	int hMiniMapFramePict_;  //ミニマップフレーム画像ハンドル
	int hMiniMapBallPict_;   //ミニマップボール画像ハンドル
	int hMiniMapPlayerPict_; //ミニマッププレイヤー画像ハンドル
	int hMiniMapLinePict_;   //ミニマップライン画像ハンドル

	//各UI要素のTransform情報
	Transform miniMapFrameTrs_;  //ミニマップフレームのTransform
	Transform miniMapBallTrs_;   //ミニマップボールのTransform
	Transform miniMapPlayerTrs_; //ミニマッププレイヤーのTransform
	Transform miniMapLineTrs_;   //ミニマップラインのTransform

	//ミニマップに表示されるカプセルのTransformリスト
	std::vector<Transform> miniMapCapsuleTrs_;

	//デフォルトの位置情報
	XMFLOAT3 defaultPlayerPosition_; //プレイヤーのデフォルト位置
	XMFLOAT3 defaultBallPosition_;   //ボールのデフォルト位置
	XMFLOAT3 defaultLinePosition_;   //ラインのデフォルト位置
public:
	MiniMap(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//ミニマップUIの種類
	enum MiniMapUIType {
		MiniMapFrame = 1, //ミニマップフレーム
		MiniMapBall,      //ミニマップボール
		MiniMapPlayer,    //ミニマッププレイヤー
		MiniMapLine       //ミニマップライン
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

	//ミニマップUIのTransformを設定する関数
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
	bool isDisplay_; //ミニマップの表示フラグ
public:
	//ミニマップの表示状態を取得する関数
	void SetDisplay(bool display) { isDisplay_ = display; }
};