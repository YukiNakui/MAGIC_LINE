#pragma once
#include "Engine/GameObject.h"
#include "Engine/VFX.h"

class Effect : public GameObject {
private:
    int handle_;              // VFXのエミッタハンドル
    bool isActive_;           // エフェクトがアクティブかどうか

public:
    Effect(GameObject* parent)
        : GameObject(parent, "VFX"), handle_(-1), isActive_(false) {
    }

    ~Effect() {
        StopEffect();
    }

    void Initialize() override {}

    // エフェクトデータを設定
    void SetEmitterData(const EmitterData& emitterData) {
        if (handle_ >= 0) {
            StopEffect(); // 既存のエフェクトを停止
        }

        // 新しいエフェクトを開始
        handle_ = VFX::Start(emitterData);
        isActive_ = true;
    }

    // エフェクトを停止
    void StopEffect() {
        if (isActive_ && handle_ >= 0) {
            VFX::End(handle_);
            handle_ = -1;
            isActive_ = false;
        }
    }

    // エフェクトがアクティブかどうか
    bool IsActive() const {
        return isActive_;
    }

    // 位置を更新
    void SetEffectPosition(const XMFLOAT3& position) {
        if (isActive_ && handle_ >= 0) {
            VFX::SetEmitterPosition(handle_, position);
        }
    }

    // 方向を更新
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