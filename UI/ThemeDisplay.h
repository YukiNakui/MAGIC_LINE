#pragma once
#include "../Engine/GameObject.h"
#include"../CDTimer.h"
#include"CountStart.h"
#include "../CameraOrbit.h"

class ThemeDisplay :
    public GameObject
{
    int hPict_;
public:
    ThemeDisplay(GameObject* parent);
    ~ThemeDisplay();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    bool IsMoveFinished() const { return hasMoved_; }
private:
    CDTimer* cdTimer_;
    float deltaTime_;
    float elapsedTime_;
    float displayDuration_;
    float transitionDuration_;
    bool isMoving_;
    bool hasMoved_; //àÍìxÇæÇØà⁄ìÆÇ∑ÇÈÇΩÇﬂÇÃÉtÉâÉO

	Transform targetTrans_; //à⁄ìÆêÊÇÃTransform

    CountStart* pCountStart_;
	CameraOrbit* pCameraOrbit_;
public:
	enum ThemeDisplayType {
		THEME_DEFAULT_TRANS = 1,
		THEME_TARGET_TRANS
	};

	void SetDefaultTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		transform_.position_ = position;
		transform_.rotate_ = rotate;
		transform_.scale_ = scale;
	}
	void SetTargetTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		targetTrans_.position_ = position;
		targetTrans_.rotate_ = rotate;
		targetTrans_.scale_ = scale;
	}
	
	void SetThemeDisplayTransform(ThemeDisplayType themeDisplayType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
		switch (themeDisplayType)
		{
		case THEME_DEFAULT_TRANS:
			SetDefaultTransform(position, rotate, scale);
			break;
		case THEME_TARGET_TRANS:
			SetTargetTransform(position, rotate, scale);
			break;
		default:
			break;
		}
	}

private:
    bool isDisplay_;
    bool isStartVisible_;
public:
	void SetDisplay(bool display) { isDisplay_ = display; }
	bool IsStartVisible() const { return isStartVisible_; }
};

