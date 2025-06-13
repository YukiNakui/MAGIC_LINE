#pragma once
#include "../Engine/GameObject.h"
#include "../Functions/CDTimer.h"
#include "Line.h"
#include "../UI/StartCountdownManager.h"
#include "../Functions/Effect.h"
#include "../Functions/CameraOrbit.h"
#include <vector>
#include "../Engine/VFX.h"

//�v���C���[�N���X
class Player : public GameObject
{
    //���f����T�E���h�Ȃǂ̃��\�[�X�n���h��
    int hModel_;
    int hLineSound_;         //���������ۂ̃T�E���h
    int hLineDeleteSound_;   //�����폜����ۂ̃T�E���h
    int hBGM_;               //�v���C���[��p��BGM

    //�v���C���[�̏��
    enum State {
        sBeforeStart,  //�J�n�O
        sMove,         //�ړ���
        sMoveFinish,   //�ړ��I��
        sResult        //���ʕ\��
    };
    State state_;  //���݂̃v���C���[�̏��
public:
    Player(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;


    // �V���h�E�}�b�v�`��
    void DrawShadowMap(const XMMATRIX& lightViewProj) override;


    //�v���C���[�̏�Ԃ��擾
    State GetPlayerState() { return state_; }

    //�v���C���[���ړ��������������ǂ������擾
    bool IsPlayerMoveFinished() { return state_ >= sMoveFinish; }

    //���݉�ʏ�ɑ��݂���J�v�Z�����X�g���擾
    std::vector<Line*> GetCapsuleList() { return capsuleList_; }

private:
    //�e��Ԃ̍X�V���\�b�h
    void StartUpdate();       //�J�n�O�̍X�V����
    void MoveUpdate();        //�ړ����̍X�V����
    void MoveFinishUpdate();  //�ړ��I�����̍X�V����
    void ResultUpdate();      //���ʕ\�����̍X�V����

    //�^�C�}�[�֘A
    CDTimer* cdTimer_;           //���ʃ^�C�}�[
    CDTimer* cupsuleTimer_;      //�J�v�Z�������p�^�C�}�[
    float capsuleSpawnInterval_; //�J�v�Z�������Ԋu

	//�v���C���[�̈ʒu�E��]�֘A
    XMMATRIX rotX;              //X����]�s��
    XMMATRIX rotY;              //Y����]�s��
    XMVECTOR move;              //�ړ��x�N�g��
    XMVECTOR rotVec;            //��]�x�N�g��
    XMFLOAT3 defaultPlayerPos_; //�v���C���[�̏����ʒu
	void ProcessPlayerRotation(float deltaTime); //�v���C���[�̉�]����

	//�v���C���[�̈ړ��֘A
	void ConstrainPlayerMovement(XMFLOAT3& position); //�v���C���[�̈ړ��𐧌�
	void CheckCollisionWithBall(XMVECTOR& pos); //�{�[���Ƃ̏Փ˃`�F�b�N
	void AdjustPlayerOnGround(); //�n�ʂɍ��킹�ăv���C���[�𒲐�
	void MovePlayerOutOfScreen(float deltaTime); //��ʊO�Ɉړ����鏈��

	void HandlePlayerActions(float deltaTime, XMVECTOR& addMove); //�v���C���[�̃A�N�V��������
	void HandleEndOfMovement(); //�ړ��I�����̏���
	void ApplyVibration(); //�U����K�p

    //�v���C���[�̎��_�E�J�����֘A
    XMFLOAT3 lookTarget_;       //�J�����̎��_�^�[�Q�b�g
    XMVECTOR cameraTargetVec;   //�J�����̃^�[�Q�b�g�x�N�g��
    XMFLOAT3 targetPos;         //�^�[�Q�b�g�ʒu
    XMVECTOR cameraEyeVec;      //�J�����̎��_�x�N�g��
    XMFLOAT3 camPos;            //�J�����ʒu
    CameraOrbit* pCameraOrbit_; //�J�����O��
	void SetCameraTarget(XMVECTOR targetVec); //�J�����̃^�[�Q�b�g��ݒ�
	void SetCameraPos(XMVECTOR camVec);    //�J�����̈ʒu��ݒ�

    //���i�J�v�Z���j�֘A
    float maxLineValue_;             //��������̍ő�l
    float currentLineValue_;         //���݈��������̗�
    Line* pCapsule_;                 //�J�v�Z���i���j�I�u�W�F�N�g
    std::vector<Line*> capsuleList_; //���݉�ʏ�ɑ��݂���J�v�Z�����X�g
    std::vector<Line*> capsulePool_; //�ė��p�p�̃J�v�Z���v�[��
    Line* GetCapsuleFromPool();      //�J�v�Z�����v�[������擾
    void ClearCapsules();            //�J�v�Z�����N���A
	void LineGaugeUpdate();          //���̍X�V����

    //�J�E���g�_�E���֘A
    StartCountdownManager* pStartCountdownManager_; //�J�E���g�_�E���}�l�[�W���[

    //�v���C���[�̑�����
    bool isMoveStarted_; //�v���C���[�������n�߂���
    bool canControl_;    //�v���C���[������ł��邩�ǂ���

    //�v���C���[�̈ړ��͈͐���
    bool CheckPlayerOutOfRange(XMFLOAT3 playerPos, XMFLOAT3 maxPos, XMFLOAT3 minPos); //�͈͊O�`�F�b�N

    //���̑��̏��
    float vibrationTime_;  //�U������
    bool isInvisible_;     //�v���C���[���������ǂ���
    bool isPlayerHitting_; //�v���C���[���Փ˂��Ă��邩�ǂ���

    //�G�t�F�N�g�֘A
    XMFLOAT3 rocketNozzlePos_;    //���P�b�g�m�Y���̈ʒu
    XMVECTOR fireEffectVec_;      //���̃G�t�F�N�g�x�N�g��
    EmitterData fireEffectData_;  //���G�t�F�N�g�̃f�[�^
    int hFireEffect_;             //���G�t�F�N�g�̃n���h��
    EmitterData sparkEffectData_; //�X�p�[�N�G�t�F�N�g�̃f�[�^
    int hSparkEffect_;            //�X�p�[�N�G�t�F�N�g�̃n���h��
    Effect* fireEffect_;          //���G�t�F�N�g
    Effect* sparkEffect_;         //�X�p�[�N�G�t�F�N�g
	void SetFireEffect(); //���G�t�F�N�g�̐ݒ�
	void SetSparkEffect(); //�X�p�[�N�G�t�F�N�g�̐ݒ�
	void UpdateEffects(const XMMATRIX& rotX, const XMMATRIX& rotY); //�G�t�F�N�g�̍X�V
};
