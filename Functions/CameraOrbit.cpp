#include "CameraOrbit.h"
#include "../Engine/Camera.h"

//カメラ軌道関連の定数を定義
namespace CameraOrbitConstants {
    constexpr float DEFAULT_ORBIT_RADIUS = 30.0f;   //デフォルトの軌道半径
    constexpr float DEFAULT_ORBIT_SPEED = 0.5f;     //デフォルトの軌道速度
    constexpr float CAMERA_HEIGHT_OFFSET = 20.0f;   //カメラの高さオフセット
    constexpr float FULL_CIRCLE = XM_2PI;           //1周分の角度
}

CameraOrbit::CameraOrbit(GameObject* parent)
    : GameObject(nullptr, "CameraOrbit"),
    isOrbiting_(false),
    focusPoint_{ 0.0f, 0.0f, 0.0f },
    orbitRadius_(CameraOrbitConstants::DEFAULT_ORBIT_RADIUS),
    orbitSpeed_(CameraOrbitConstants::DEFAULT_ORBIT_SPEED),
    orbitAngle_(0.0f)
{
}

CameraOrbit::~CameraOrbit()
{
}

void CameraOrbit::Initialize()
{
}

void CameraOrbit::Update()
{
}

void CameraOrbit::Draw()
{
}

void CameraOrbit::Release()
{
}

void CameraOrbit::SetOrbit(const XMFLOAT3& focusPoint, float radius, float speed)
{
    //軌道のパラメータを設定
    focusPoint_ = focusPoint;
    orbitRadius_ = radius;
    orbitSpeed_ = speed;
    isOrbiting_ = true;       //軌道の動作を開始
    orbitAngle_ = 0.0f;       //初期角度をリセット
}

void CameraOrbit::Update(float deltaTime)
{
    if (!isOrbiting_) return; //軌道動作が無効の場合は何もしない

    //カメラの角度を更新
    orbitAngle_ += orbitSpeed_ * deltaTime;
    if (orbitAngle_ >= CameraOrbitConstants::FULL_CIRCLE) {
        orbitAngle_ -= CameraOrbitConstants::FULL_CIRCLE; //一周を超えたらリセット
    }

    //新しいカメラ位置を計算
    float offsetX = orbitRadius_ * cosf(orbitAngle_);
    float offsetZ = orbitRadius_ * sinf(orbitAngle_);
    XMFLOAT3 newCameraPos = {
        focusPoint_.x + offsetX,
        focusPoint_.y + CameraOrbitConstants::CAMERA_HEIGHT_OFFSET, //高さを調整
        focusPoint_.z + offsetZ
    };

    //カメラの位置と注視点を設定
    Camera::SetPosition(newCameraPos);
    Camera::SetTarget(focusPoint_);
}