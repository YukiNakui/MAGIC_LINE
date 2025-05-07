#include "CountdownBase.h"

CountdownBase::CountdownBase(GameObject* parent, const std::string& name)
    : GameObject(parent, name), cdTimer_(nullptr), deltaTime_(0.0f), elapsedTime_(0.0f), isDisplay_(true), scale_(1.0f)
{
}

CountdownBase::~CountdownBase()
{
}

void CountdownBase::Release()
{
}