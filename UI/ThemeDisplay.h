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

	//�ړ��������������𔻒�
    bool IsMoveFinished() const { return hasMoved_; }
private:
    //�^�C�}�[�֘A
    CDTimer* cdTimer_;         //�^�C�}�[�I�u�W�F�N�g
    float deltaTime_;          //�t���[���Ԃ̌o�ߎ���
    float elapsedTime_;        //�ݐς̌o�ߎ���

    //�\���ƈړ��̐ݒ�
    float displayDuration_;    //�\������
    float transitionDuration_; //�ړ��A�j���[�V�����̎���
    bool isMoving_;            //���݈ړ������ǂ���
    bool hasMoved_;            //��x�����ړ����邽�߂̃t���O

    //�ړ����Transform���
    Transform targetTrans_;    //�ړ����Transform

    //�֘A�I�u�W�F�N�g
    StartCountdownManager* pStartCountdownNumber_; //�J�E���g�_�E���Ǘ��I�u�W�F�N�g
    CameraOrbit* pCameraOrbit_;                    //�J�����̋O������I�u�W�F�N�g

public:
    //�e�[�}�\����Transform�̎��
    enum ThemeDisplayType {
        THEME_DEFAULT_TRANS = 1, //�f�t�H���g��Transform
        THEME_TARGET_TRANS       //�ړ����Transform
    };

    //�f�t�H���g��Transform��ݒ�
    void SetDefaultTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        transform_.position_ = position;
        transform_.rotate_ = rotate;
        transform_.scale_ = scale;
    }

    //�ړ����Transform��ݒ�
    void SetTargetTransform(XMFLOAT3 position, XMFLOAT3 rotate, XMFLOAT3 scale) {
        targetTrans_.position_ = position;
        targetTrans_.rotate_ = rotate;
        targetTrans_.scale_ = scale;
    }

    // �e�[�}�\����Transform���ꊇ�Őݒ�
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
    //�\����ԊǗ�
    bool isDisplay_;       //�e�[�}���\������Ă��邩�ǂ���
    bool isStartVisible_;  //"START"���\�������ǂ���

public:
    //�\����Ԃ�ݒ�
    void SetDisplay(bool display) { isDisplay_ = display; }

    //"START"���\�������ǂ����𔻒�
    bool IsStartVisible() const { return isStartVisible_; }
};

