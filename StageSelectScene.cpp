#include "StageSelectScene.h"
#include"Engine/Image.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"Engine/CsvReader.h"

namespace {
	int MAX_STAGE_NUM = 3; // �ő�X�e�[�W��
	int STAGE_IMAGE_WIDTH = 200; // �X�e�[�W�摜�̕�
	int STAGE_IMAGE_SPACING = 20; // �X�e�[�W�摜�̊Ԋu
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
		stageInfos_[i].fileName = csvStage->GetString(i, 0);
		stageInfos_[i].stageNum = csvStage->GetInt(i, 1);
		stageInfos_[i].hThemePict = Image::Load(csvStage->GetString(i, 2));
		assert(stageInfos_[i].hThemePict >= 0);
		stageInfos_[i].hStagePict = Image::Load(csvStage->GetString(i, 3));
		assert(stageInfos_[i].hStagePict >= 0);
		stageInfos_[i].x = csvStage->GetInt(i, 4);
		stageInfos_[i].y = csvStage->GetInt(i, 5);
		stageInfos_[i].isSelected = false; // �����͑I������Ă��Ȃ�
	}
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
	Image::SetTransform(hStageSelectPict_, transform_);
	Image::Draw(hStageSelectPict_);
	Image::SetTransform(hStageFrameGrayPict_, transform_);
	Image::Draw(hStageFrameGrayPict_);
	Image::SetTransform(hStageFrameYellowPict_, transform_);
	Image::Draw(hStageFrameYellowPict_);
	Image::SetTransform(hExplUIGrayPict_, transform_);
	Image::Draw(hExplUIGrayPict_);
	Image::SetTransform(hExplUIYellowPict_, transform_);
	Image::Draw(hExplUIYellowPict_);
}

void StageSelectScene::Release()
{
}
