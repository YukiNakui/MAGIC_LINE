#include "CameraOrbit.h"
#include "../Engine/Camera.h"

//�J�����O���֘A�̒萔���`
namespace CameraOrbitConstants {
    constexpr float DEFAULT_ORBIT_RADIUS = 30.0f;   //�f�t�H���g�̋O�����a
    constexpr float DEFAULT_ORBIT_SPEED = 0.5f;     //�f�t�H���g�̋O�����x
    constexpr float CAMERA_HEIGHT_OFFSET = 20.0f;   //�J�����̍����I�t�Z�b�g
    constexpr float FULL_CIRCLE = XM_2PI;           //1�����̊p�x
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
    //�O���̃p�����[�^��ݒ�
    focusPoint_ = focusPoint;
    orbitRadius_ = radius;
    orbitSpeed_ = speed;
    isOrbiting_ = true;       //�O���̓�����J�n
    orbitAngle_ = 0.0f;       //�����p�x�����Z�b�g
}

void CameraOrbit::Update(float deltaTime)
{
    if (!isOrbiting_) return; //�O�����삪�����̏ꍇ�͉������Ȃ�

    //�J�����̊p�x���X�V
    orbitAngle_ += orbitSpeed_ * deltaTime;
    if (orbitAngle_ >= CameraOrbitConstants::FULL_CIRCLE) {
        orbitAngle_ -= CameraOrbitConstants::FULL_CIRCLE; //����𒴂����烊�Z�b�g
    }

    //�V�����J�����ʒu���v�Z
    float offsetX = orbitRadius_ * cosf(orbitAngle_);
    float offsetZ = orbitRadius_ * sinf(orbitAngle_);
    XMFLOAT3 newCameraPos = {
        focusPoint_.x + offsetX,
        focusPoint_.y + CameraOrbitConstants::CAMERA_HEIGHT_OFFSET, //�����𒲐�
        focusPoint_.z + offsetZ
    };

    //�J�����̈ʒu�ƒ����_��ݒ�
    Camera::SetPosition(newCameraPos);
    Camera::SetTarget(focusPoint_);
}