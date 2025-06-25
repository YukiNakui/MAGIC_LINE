#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // �ő�X�e�[�W��
	float STAGE_IMAGE_WIDTH = 0.5f; // �X�e�[�W�摜�̕�
	float STAGE_IMAGE_HEIGHT = 0.5f; // �X�e�[�W�摜�̍���
	float STAGE_IMAGE_SPACING = 0.1f; // �X�e�[�W�摜�̊Ԋu
	float STAGE_IMAGE_X_OFFSET = 0.7f; // �X�e�[�W�摜��X�����I�t�Z�b�g�ʒu
	float STAGE_IMAGE_Y_OFFSET = 0.0f; // �X�e�[�W�摜��Y�����I�t�Z�b�g�ʒu
	float THEME_IMAGE_Y_OFFSET = 0.5f; // ����摜��Y�����I�t�Z�b�g�ʒu
}

StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"),
	hStageSelectPict_(-1),
	currentStageIndex_(0), isExplUISelected_(false),
	hStageFrameGrayPict_(-1), hStageFrameYellowPict_(-1),
	hExplUIGrayPict_(-1), hExplUIYellowPict_(-1)
{
}

void StageSelectScene::Initialize()
{
	//�摜�����[�h
	hStageSelectPict_ = Image::Load("UI/StageSelect/StageSelect.png");
	assert(hStageSelectPict_ >= 0);
	hStageFrameGrayPict_ = Image::Load("UI/StageSelect/StageFrame_Gray.png");
	assert(hStageFrameGrayPict_ >= 0);
	hStageFrameYellowPict_ = Image::Load("UI/StageSelect/StageFrame_Yellow.png");
	assert(hStageFrameYellowPict_ >= 0);
	hExplUIGrayPict_ = Image::Load("UI/StageSelect/Explanation_Gray.png");
	assert(hExplUIGrayPict_ >= 0);
	hExplUIYellowPict_ = Image::Load("UI/StageSelect/Explanation_Yellow.png");
	assert(hExplUIYellowPict_ >= 0);

	stageSelectPictTrs_.position_ = { 0.0f, 0.7f, 0.0f }; // �X�e�[�W�I����ʂ̉摜�̈ʒu
	stageSelectPictTrs_.scale_ = { 0.5f, 0.5f, 0.0f }; // �X�e�[�W�I����ʂ̉摜�̃X�P�[��
	stageFrameTrs_.scale_ = { 0.53f, 0.53f, 0.0f }; // �X�e�[�W�t���[���̃X�P�[��
	explUITrs_.position_ = { 0.0f, -0.7f, 0.0f }; // ����E���[������UI�̈ʒu
	explUITrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // ����E���[������UI�̃X�P�[��

	//�X�e�[�W���ꗗ��ǂݍ���
	CsvReader* csvStage = new CsvReader("CSV/StageData/StageInfoList.csv");
	int lines = csvStage->GetLines();
	for (int i = 1;i < lines;i++) {//1�s�ڂ̓w�b�_�[�Ȃ̂ŃX�L�b�v
		stageInfos_.push_back(StageInfo());
		//0-index����n�܂�̂ŁAi-1�Œ���
		int stageIndex = i - 1;
		//�X�e�[�W�t�@�C�������擾
		stageInfos_[stageIndex].fileName_ = "CSV/StageData/" + csvStage->GetString(i, 0) + ".csv";
		//�X�e�[�W�̃e�[�}�摜�ƃX�e�[�W�摜�����[�h
		stageInfos_[stageIndex].hThemePict_ = Image::Load("UI/Theme/" + csvStage->GetString(i, 1) + ".png");
		assert(stageInfos_[stageIndex].hThemePict_ >= 0);
		stageInfos_[stageIndex].hStagePict_ = Image::Load("UI/StageImage/" + csvStage->GetString(i, 2) + ".png");
		assert(stageInfos_[stageIndex].hStagePict_ >= 0);
		//�X�e�[�W�摜�̕\���ʒu���v�Z
		stageInfos_[stageIndex].stageImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].stageImgTrs_.position_.y = 0;
		stageInfos_[stageIndex].stageImgTrs_.scale_ = { 0.25f, 0.25f, 0.25f }; // �X�e�[�W�摜�̃X�P�[��
		//����摜�̕\���ʒu���v�Z
		stageInfos_[stageIndex].themeImgTrs_.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * stageIndex + STAGE_IMAGE_SPACING * stageIndex;
		stageInfos_[stageIndex].themeImgTrs_.position_.y = -THEME_IMAGE_Y_OFFSET;
		stageInfos_[stageIndex].themeImgTrs_.scale_ = { 0.3f, 0.3f, 0.0f }; // ����摜�̃X�P�[��
		stageInfos_[stageIndex].isSelected_ = false; // �����͑I������Ă��Ȃ�
	}
	stageInfos_[0].isSelected_ = true; // �ŏ��̃X�e�[�W��I����Ԃɂ���
	isExplUISelected_ = false; // ����E���[������UI�͏����͑I������Ă��Ȃ�
}

void StageSelectScene::Update()
{
	if (Input::IsKeyDown(DIK_RIGHT)) {
		// �E�L�[�������ꂽ�玟�̃X�e�[�W��
		if (!isExplUISelected_) {
			if (currentStageIndex_ < stageInfos_.size() - 1) {
				stageInfos_[currentStageIndex_].isSelected_ = false; // ���݂̃X�e�[�W���I���ɂ���
				currentStageIndex_++;
				stageInfos_[currentStageIndex_].isSelected_ = true; // ���̃X�e�[�W��I����Ԃɂ���
			}
		}
	}
	else if (Input::IsKeyDown(DIK_LEFT)) {
		// ���L�[�������ꂽ��O�̃X�e�[�W��
		if (!isExplUISelected_) {
			if (currentStageIndex_ > 0) {
				stageInfos_[currentStageIndex_].isSelected_ = false; // ���݂̃X�e�[�W���I���ɂ���
				currentStageIndex_--;
				stageInfos_[currentStageIndex_].isSelected_ = true; // �O�̃X�e�[�W��I����Ԃɂ���
			}
		}
	}
	else if (Input::IsKeyDown(DIK_UP)) {
		// ��L�[�������ꂽ�瑀��E���[������UI���I��
		isExplUISelected_ = false;
		stageInfos_[currentStageIndex_].isSelected_ = true;
	}
	else if (Input::IsKeyDown(DIK_DOWN)) {
		// ���L�[�������ꂽ�瑀��E���[������UI��I��
		isExplUISelected_ = true;
		stageInfos_[currentStageIndex_].isSelected_ = false;
	}
	
	if (Input::IsKeyDown(DIK_RETURN)) {
		if (isExplUISelected_) {
			// ����E���[������UI���I������Ă���ꍇ
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_EXPLANATION); // �����V�[���ɑJ��
		}
		else {
			// �X�e�[�W�I�����莞
			std::string selectedStageFile = stageInfos_[currentStageIndex_].fileName_; // �I�񂾃t�@�C����
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeToPlaySceneWithFile(selectedStageFile);
		}
	}
}

void StageSelectScene::Draw()
{
	Image::SetTransform(hStageSelectPict_, stageSelectPictTrs_);
	Image::Draw(hStageSelectPict_);

	// ����E���[������UI�̑I����Ԃɉ����ĕ`��
	if (isExplUISelected_) {
		Image::SetTransform(hExplUIYellowPict_, explUITrs_);
		Image::Draw(hExplUIYellowPict_);
	}
	else {
		Image::SetTransform(hExplUIGrayPict_, explUITrs_);
		Image::Draw(hExplUIGrayPict_);
	}

	// �I������Ă���X�e�[�W�̃t���[����`��
	for (int i = 0; i < stageInfos_.size(); i++) {
		stageFrameTrs_.position_ = stageInfos_[i].stageImgTrs_.position_;
		if (stageInfos_[i].isSelected_) {
			Image::SetTransform(hStageFrameYellowPict_, stageFrameTrs_);
			Image::Draw(hStageFrameYellowPict_);
		}
		else {
			Image::SetTransform(hStageFrameGrayPict_, stageFrameTrs_);
			Image::Draw(hStageFrameGrayPict_);
		}
	}
	// �X�e�[�W����`��
	for (int i = 0; i < stageInfos_.size(); i++) {
		// �X�e�[�W�̃e�[�}�摜��`��
		Image::SetTransform(stageInfos_[i].hThemePict_, stageInfos_[i].themeImgTrs_);
		Image::Draw(stageInfos_[i].hThemePict_);
		
		// �X�e�[�W�̉摜��`��
		Image::SetTransform(stageInfos_[i].hStagePict_, stageInfos_[i].stageImgTrs_);
		Image::Draw(stageInfos_[i].hStagePict_);
	}
}

void StageSelectScene::Release()
{
}
