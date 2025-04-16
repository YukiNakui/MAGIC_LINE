#pragma once
#include "Engine/GameObject.h"
#include"CDTimer.h"
#include"Capsule.h"
#include"CountStart.h"
#include<vector>

class Player :
    public GameObject
{
	int hModel_;
	int hLineSound_;
	int hLineDeleteSound_;
	int hBGM_;
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;

private:
	enum State {
		sBeforeStart,
		sMove,
		sMoveFinish,
		sResult,
	};
	State state_;
	void StartUpdate();
	void MoveUpdate();
	void MoveFinishUpdate();
	void ResultUpdate();

	CDTimer* cdTimer_;
	CDTimer* cupsuleTimer_;
	float capsuleSpawnInterval_;
	XMFLOAT3 lookTarget_;
	XMVECTOR front_;

	XMMATRIX rotX;
	XMMATRIX rotY;
	XMVECTOR move;
	XMVECTOR rotVec;

	XMVECTOR cameraTargetVec;
	XMFLOAT3 targetPos;
	XMVECTOR cameraEyeVec;
	XMFLOAT3 camPos;

	float maxLineValue_;
	float currentLineValue_;//現在引いた線の量
	Capsule* pCapsule_;
	std::vector<Capsule*> capsuleList_; // 画面上に存在するカプセルリスト
	std::vector<Capsule*> capsulePool_; // 再利用用のカプセルプール

	CountStart* pCountStart_;
	bool isMoveStarted_; //プレイヤーが動き始めたか
	bool canControl_; //プレイヤーが操作できるかどうか

	XMFLOAT3 maxPos_;
	XMFLOAT3 minPos_;

	bool isPlayerHitting_;
private:
	Capsule* GetCapsuleFromPool();
	void ClearCapsules();
	bool CheckPlayerOutOfRange(XMFLOAT3 playerPos, XMFLOAT3 maxPos, XMFLOAT3 minPos);
};

