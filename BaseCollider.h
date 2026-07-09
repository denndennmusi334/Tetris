#pragma once
#include "Job.h"

// 衝突判定用のタグを表す列挙型
enum class ColliderTag
{
    TAG_NULL,
    TAG_PLAYER,
    TAG_ENEMY,
    TAG_BULLET,
};

// 衝突判定用のタグを表す列挙型.
// カプセルは故障中.
enum class ColliderType
{
    BOX,
    CIRCLE,
    CAPSULE,

    TYPE_NULL
};

class GameObject;

class BaseCollider
{
protected:
    Vector2D<float> localPos{ 0,0 };
    GameObject* owner = nullptr;
    ColliderTag tag = ColliderTag::TAG_NULL;

	bool isDestroyed = false;

public:
    virtual ~BaseCollider();

    void SetOwner(GameObject* o);
    GameObject* GetOwner() const { return owner; }

	void Destroy() { isDestroyed = true; }
	bool IsDestroyed() const { return isDestroyed; }

    void SetLocalPosition(const Vector2D<float>& p) { localPos = p; }
    Vector2D<float> GetWorldPosition() const;

    void SetTag(ColliderTag t) { tag = t; }
    ColliderTag GetTag() const { return tag; }

    virtual ColliderType GetColliderType() const = 0;

#if COLLIDER_DEBUG
    virtual void DebugDraw() {};
#endif
};
