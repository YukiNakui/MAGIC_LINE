#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Effect : public GameObject {
private:
    int handle_;              // VFX�̃G�~�b�^�n���h��
    bool isActive_;           // �G�t�F�N�g���A�N�e�B�u���ǂ���

public:
    Effect(GameObject* parent)
        : GameObject(parent, "VFX"), handle_(-1), isActive_(false) {
    }

    ~Effect() {
        StopEffect();
    }

    void Initialize() override {}

    // �G�t�F�N�g�f�[�^��ݒ�
    void SetEmitterData(const EmitterData& emitterData) {
        if (handle_ >= 0) {
            StopEffect(); // �����̃G�t�F�N�g���~
        }

        // �V�����G�t�F�N�g���J�n
        handle_ = VFX::Start(emitterData);
        isActive_ = true;
    }

    // �G�t�F�N�g���~
    void StopEffect() {
        if (isActive_ && handle_ >= 0) {
            VFX::End(handle_);
            handle_ = -1;
            isActive_ = false;
        }
    }

    // �G�t�F�N�g���A�N�e�B�u���ǂ���
    bool IsActive() const {
        return isActive_;
    }

    // �ʒu���X�V
    void SetEffectPosition(const XMFLOAT3& position) {
        if (isActive_ && handle_ >= 0) {
            VFX::SetEmitterPosition(handle_, position);
        }
    }

    // �������X�V
    void SetEffectDirection(const XMFLOAT3& direction) {
        if (isActive_ && handle_ >= 0) {
            VFX::SetEmitterDirection(handle_, direction);
        }
    }

    void Update() override {
        if (isActive_) {
            VFX::SetEmitterPosition(handle_, GetWorldPosition());
        }
    }

    void Draw() override {}

    void Release() override {
        StopEffect();
    }
};