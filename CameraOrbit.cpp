#include "CameraOrbit.h"
#include "Engine/Camera.h"

CameraOrbit::CameraOrbit(GameObject* parent)
	: GameObject(nullptr, "CameraOrbit"), isOrbiting_(false), focusPoint_{ 0.0f, 0.0f, 0.0f }, orbitRadius_(30.0f), orbitSpeed_(0.5f), orbitAngle_(0.0f)
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
    focusPoint_ = focusPoint;
    orbitRadius_ = radius;
    orbitSpeed_ = speed;
    isOrbiting_ = true;
    orbitAngle_ = 0.0f;
}

void CameraOrbit::Update(float deltaTime)
{
    if (!isOrbiting_) return;

    // �J�����̊p�x���X�V
    orbitAngle_ += orbitSpeed_ * deltaTime;
    if (orbitAngle_ >= XM_2PI) {
        orbitAngle_ -= XM_2PI;
    }

    // �V�����J�����ʒu���v�Z
    float offsetX = orbitRadius_ * cosf(orbitAngle_);
    float offsetZ = orbitRadius_ * sinf(orbitAngle_);
    XMFLOAT3 newCameraPos = {
        focusPoint_.x + offsetX,
        focusPoint_.y + 20.0f, // �����𒲐�
        focusPoint_.z + offsetZ
    };

    // �J�����̈ʒu�ƒ����_��ݒ�
    Camera::SetPosition(newCameraPos);
    Camera::SetTarget(focusPoint_);
}
