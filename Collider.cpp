#include "stdafx.h"
#include "Collider.h"

#if DEBUG
void BoxCollider::DebugDraw()
{
    DrawBoxAA(GetWorldPosition().x - halfSize.x, GetWorldPosition().y - halfSize.y, GetWorldPosition().x + halfSize.x, GetWorldPosition().y + halfSize.y, GetColor(255, 0, 0), FALSE);
}

void CircleCollider::DebugDraw()
{
    DrawCircleAA(GetWorldPosition().x, GetWorldPosition().y, radius, 32, GetColor(255, 0, 0), FALSE);
}

void CapsuleCollider::DebugDraw()
{
    auto center = GetWorldPosition();
    auto top = CapsuleTop();
    auto bottom = CapsuleBottom();

    // è„â∫ÇÃâ~
    DrawCircleAA(top.x, top.y, radius, 32, GetColor(255, 0, 0), FALSE);
    DrawCircleAA(bottom.x, bottom.y, radius, 32, GetColor(255, 0, 0), FALSE);

    // íÜâõÇÃãÈå`
    DrawBoxAA(
        center.x - radius, center.y - halfHeight,
        center.x + radius, center.y + halfHeight,
        GetColor(255, 0, 0), FALSE
    );
}
#endif
