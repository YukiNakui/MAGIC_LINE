#pragma once
#include "../Engine/GameObject.h"
class HeightMeter :
    public GameObject
{
	int hMeterPict_;
	int hTrianglePict_;

	Transform meterUITrs_;
	Transform triangleUITrs_;
public:
	HeightMeter(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
private:
    float maxHeight_;       // �\������ő�̍���
    float minHeight_;       // �\������ŏ��̍���
    float currentHeight_;   // ���݂̍���

public:
    //�������[�^�[UI�̎��
    enum HeightMeterUIType {
        HEIGHT_METER = 1,    //���[�^�[
        HEIGHT_TRIANGLE      //�O�p�`�J�[�\��
    };

    //���[�^�[��Transform��ݒ�
    void SetHeightMeterUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        meterUITrs_.position_ = position;
        meterUITrs_.rotate_ = rotate;
        meterUITrs_.scale_ = scale;
    }

    //�O�p�`�J�[�\����Transform��ݒ�
    void SetHeightTriangleUITransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        triangleUITrs_.position_ = position;
        triangleUITrs_.rotate_ = rotate;
        triangleUITrs_.scale_ = scale;
    }

    //���[�^�[�܂��͎O�p�`�J�[�\����Transform���ꊇ�Őݒ�
    void SetHeightMeterUITransform(HeightMeterUIType heightMeterUIType, XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        switch (heightMeterUIType) {
        case HEIGHT_METER:
            SetHeightMeterUITransform(position, rotate, scale);
            break;
        case HEIGHT_TRIANGLE:
            SetHeightTriangleUITransform(position, rotate, scale);
            break;
        default:
            break;
        }
    }

    //���[�^�[�̍ő�l�ƍŏ��l��ݒ�
    void SetMaxAndMinHeight(float maxHeight, float minHeight) {
        maxHeight_ = maxHeight;
        minHeight_ = minHeight;
    }

private:
    bool isDisplay_; //�������[�^�[���\������Ă��邩�ǂ����̃t���O

public:
    //�\����Ԃ�ݒ�
    void SetDisplay(bool display) { isDisplay_ = display; }
};

