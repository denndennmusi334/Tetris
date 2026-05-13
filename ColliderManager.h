#pragma once
#include <vector>
#include "BaseCollider.h"
#include "Collider.h"

class ColliderManager
{
private:
    std::vector<BaseCollider*> colliders;
    ColliderManager() = default;

    bool CheckPair(BaseCollider* a, BaseCollider* b);

    bool CircleVsCircle(const CircleCollider* a, const CircleCollider* b);
    bool BoxVsBox(const BoxCollider* a, const BoxCollider* b);
    bool CircleVsBox(const CircleCollider* c, const BoxCollider* b);
    bool CircleVsCapsule(const CircleCollider* circle, const CapsuleCollider* cap);
    bool CapsuleVsCapsule(const CapsuleCollider* a, const CapsuleCollider* b);
    bool CapsuleVsBox(const CapsuleCollider* cap, const BoxCollider* box);
    bool SegmentVsBox(const Vector2D<float>& a, const Vector2D<float>& b, const BoxCollider* box, float radius);
    float DistanceSq_Point_Segment(const Vector2D<float>& pc, const Vector2D<float>& a, const Vector2D<float>& b);
public:
    static ColliderManager& GetInstance()
    {
        static ColliderManager instance;
        return instance;
    }
    // ìoò^
    void Register(BaseCollider* col)
    {
        colliders.push_back(col);
    }


	// ìoò^âèúä÷êî SceneÇÃKIllÇ≈ïKÇ∏ñàÉtÉåÅ[ÉÄåƒÇ‘.
	void DestroyedColliderCheck()
	{
		colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
			[](BaseCollider* col) { return !col || col->IsDestroyed(); }),
			colliders.end());
	}

    // è’ìÀîªíË
    void CheckAllCollisions();
#ifdef DEBUG
    void DebugDraw();
#endif 

};
