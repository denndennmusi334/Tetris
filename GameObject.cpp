#include "stdafx.h"
#include "GameObject.h"

void GameObject::AddCollider(std::unique_ptr<BaseCollider> col)
{
    col->SetOwner(this);
    colliders.push_back(std::move(col));
}
