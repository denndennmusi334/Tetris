#pragma once
#include "BaseCollider.h"

class BoxCollider : public BaseCollider
{
    Vector2D<float> halfSize;   //箱の縦横幅の半分
public:
    BoxCollider(const Vector2D<float> local, const Vector2D<float> hs) : halfSize(hs) {
        localPos = local;
    }
    Vector2D<float> GetHalfSize() const { return halfSize; }
    ColliderType GetColliderType() const override { return ColliderType::BOX; }

#if COLLIDER_DEBUG
    void DebugDraw() override;
#endif
};


class CircleCollider : public BaseCollider
{
    float radius;       // 円の半径
public:
    CircleCollider(const Vector2D<float> local, float r) : radius(r) {
        localPos = local;
    }
    float GetRadius() const { return radius; }
    ColliderType GetColliderType() const override { return ColliderType::CIRCLE; }

#if COLLIDER_DEBUG
    void DebugDraw() override;
#endif
};

class CapsuleCollider : public BaseCollider
{
private:
    float radius;      // 円の半径
    float halfHeight;  // 中央から上下への距離
public:
    CapsuleCollider(const Vector2D<float> local, float r, float hHe) :radius(r), halfHeight(hHe){
        localPos = local;
    }
    float GetRadius() const { return radius; }
    float GetHalfHeight() const { return halfHeight; }
    ColliderType GetColliderType() const override { return ColliderType::CAPSULE; }

    Vector2D<float> CapsuleTop() const
    {
        return GetWorldPosition() + Vector2D<float>(0, halfHeight);
    }

    Vector2D<float> CapsuleBottom() const
    {
        return GetWorldPosition() - Vector2D<float>(0, halfHeight);
    }
#if COLLIDER_DEBUG
    void DebugDraw() override;
#endif
};
