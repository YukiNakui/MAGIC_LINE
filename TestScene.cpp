#include "TestScene.h"
//#include"Engine/Model.h"
#include "Player.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"),hModel_(-1)
{
}

//������
void TestScene::Initialize()
{
	Instantiate<Player>(this);
}

//�X�V
void TestScene::Update()
{
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
