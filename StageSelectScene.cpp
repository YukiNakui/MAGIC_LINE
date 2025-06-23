#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // �ő�X�e�[�W��
	int STAGE_IMAGE_WIDTH = 3; // �X�e�[�W�摜�̕�
	int STAGE_IMAGE_HEIGHT = 3; // �X�e�[�W�摜�̍���
	int STAGE_IMAGE_SPACING = 2; // �X�e�[�W�摜�̊Ԋu
	int STAGE_IMAGE_X_OFFSET = 5; // �X�e�[�W�摜��X�����I�t�Z�b�g�ʒu
	int STAGE_IMAGE_Y_OFFSET = 5; // �X�e�[�W�摜��Y�����I�t�Z�b�g�ʒu
	int THEME_IMAGE_Y_OFFSET = 3; // ����摜��Y�����I�t�Z�b�g�ʒu
}

StageSelectScene::StageSelectScene(GameObject* parent)
	: GameObject(parent, "StageSelectScene"),
	hStageSelectPict_(-1),
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

	//�X�e�[�W���ꗗ��ǂݍ���
	CsvReader* csvStage = new CsvReader("CSV/StageData/StageInfoList.csv");
	int lines = csvStage->GetLines();
	for (int i = 1;i < lines;i++) {
		stageInfos_.push_back(StageInfo());
		//�X�e�[�W�t�@�C�������擾
		stageInfos_[i].fileName = "CSV/StageData/" + csvStage->GetString(i, 0) + ".csv";
		//�X�e�[�W�̃e�[�}�摜�ƃX�e�[�W�摜�����[�h
		stageInfos_[i].hThemePict = Image::Load("UI/ThemeImage/" + csvStage->GetString(i, 1) + ".png");
		assert(stageInfos_[i].hThemePict >= 0);
		stageInfos_[i].hStagePict = Image::Load("UI/StageImage/" + csvStage->GetString(i, 2) + ".png");
		assert(stageInfos_[i].hStagePict >= 0);
		//�X�e�[�W�摜�̕\���ʒu���v�Z
		stageInfos_[i].stageImgTrs.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * i + STAGE_IMAGE_SPACING * i;
		stageInfos_[i].stageImgTrs.position_.y = 0;
		//����摜�̕\���ʒu���v�Z
		stageInfos_[i].themeImgTrs.position_.x = -STAGE_IMAGE_X_OFFSET + STAGE_IMAGE_WIDTH * i + STAGE_IMAGE_SPACING * i;
		stageInfos_[i].themeImgTrs.position_.y = THEME_IMAGE_Y_OFFSET;
		stageInfos_[i].isSelected = false; // �����͑I������Ă��Ȃ�
	}
	stageInfos_[0].isSelected = true; // �ŏ��̃X�e�[�W��I����Ԃɂ���
}

void StageSelectScene::Update()
{
	// �X�e�[�W�I�����莞
	std::string selectedStageFile = stageInfos_[0].fileName; // �I�񂾃t�@�C����
	if (Input::IsKeyDown(DIK_RETURN)) {
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeToPlaySceneWithFile(selectedStageFile);
	}
}

void StageSelectScene::Draw()
{
	Image::SetTransform(hStageSelectPict_, stageSelectPictTrs_);
	Image::Draw(hStageSelectPict_);
	Image::SetTransform(hExplUIGrayPict_, explUITrs_);
	Image::Draw(hExplUIGrayPict_);
	Image::SetTransform(hExplUIYellowPict_, explUITrs_);
	Image::Draw(hExplUIYellowPict_);
	// �X�e�[�W����`��
	for (int i = 0; i < stageInfos_.size(); i++) {
		// �X�e�[�W�̃e�[�}�摜��`��
		Image::SetTransform(stageInfos_[i].hThemePict, stageInfos_[i].themeImgTrs);
		Image::Draw(stageInfos_[i].hThemePict);
		// �I������Ă���X�e�[�W�̃t���[����`��
		if (stageInfos_[i].isSelected) {
			Image::SetTransform(hStageFrameYellowPict_, stageFrameTrs_);
			Image::Draw(hStageFrameYellowPict_);
		}
		else {
			Image::SetTransform(hStageFrameGrayPict_, stageFrameTrs_);
			Image::Draw(hStageFrameGrayPict_);
		}
		// �X�e�[�W�̉摜��`��
		Image::SetTransform(stageInfos_[i].hStagePict, stageInfos_[i].stageImgTrs);
		Image::Draw(stageInfos_[i].hStagePict);
	}
}

void StageSelectScene::Release()
{
}
