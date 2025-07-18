#include "RootObject.h"
#include "SceneManager.h"


RootObject::RootObject():
	GameObject(nullptr, "RootObject")
{
}


RootObject::~RootObject()
{
}

void RootObject::Initialize()
{
	Instantiate<SceneManager>(this);
}

void RootObject::Update()
{
}

void RootObject::Draw()
{
}

void RootObject::Release()
{
}


//void RootObject::SetLightViewProj(const DirectX::XMMATRIX& mat) {
//    for (auto child : *GetChildList()) {
//        child->SetLightViewProj(mat);
//    }
//}