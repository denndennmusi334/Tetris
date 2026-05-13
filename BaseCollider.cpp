#include "stdafx.h"
#include "BaseCollider.h"
#include "GameObject.h"
#include "ColliderManager.h"

Vector2D<float> BaseCollider::GetWorldPosition() const
{
    if (!owner) return localPos;
    return owner->GetPosition() + localPos;
}

void BaseCollider::SetOwner(GameObject* o)
{
    owner = o;
    ColliderManager::GetInstance().Register(this);
}

BaseCollider::~BaseCollider()
{
}