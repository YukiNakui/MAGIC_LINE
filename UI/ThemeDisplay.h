#pragma once
#include "../Engine/GameObject.h"
#include"../Functions/CDTimer.h"
#include"StartCountdownManager.h"
#include "../Functions/CameraOrbit.h"


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

	//移動が完了したかを判定
    bool IsMoveFinished() const { return hasMoved_; }
private:
    //タイマー関連
    CDTimer* cdTimer_;         //タイマーオブジェクト
    float deltaTime_;          //フレーム間の経過時間
    float elapsedTime_;        //累積の経過時間

    //表示と移動の設定
    float displayDuration_;    //表示時間
    float transitionDuration_; //移動アニメーションの時間
    bool isMoving_;            //現在移動中かどうか
    bool hasMoved_;            //一度だけ移動するためのフラグ

    //移動先のTransform情報
    Transform targetTrans_;    //移動先のTransform

    //関連オブジェクト
    StartCountdownManager* pStartCountdownNumber_; //カウントダウン管理オブジェクト
    CameraOrbit* pCameraOrbit_;                    //カメラの軌道制御オブジェクト

public:
    //テーマ表示のTransformの種類
    enum ThemeDisplayType {
        THEME_DEFAULT_TRANS = 1, //デフォルトのTransform
        THEME_TARGET_TRANS       //移動先のTransform
    };

    //デフォルトのTransformを設定
    void SetDefaultTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        transform_.position_ = position;
        transform_.rotate_ = rotate;
        transform_.scale_ = scale;
    }

    //移動先のTransformを設定
    void SetTargetTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        targetTrans_.position_ = position;
        targetTrans_.rotate_ = rotate;
        targetTrans_.scale_ = scale;
    }

    // テーマ表示のTransformを一括で設定
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
    //表示状態管理
    bool isDisplay_;       //テーマが表示されているかどうか
    bool isStartVisible_;  //"START"が表示中かどうか

public:
    //表示状態を設定
    void SetDisplay(bool display) { isDisplay_ = display; }

    //"START"が表示中かどうかを判定
    bool IsStartVisible() const { return isStartVisible_; }
};

