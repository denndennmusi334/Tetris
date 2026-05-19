#include "stdafx.h"
#include "ColliderManager.h"
#include "GameObject.h"

void ColliderManager::CheckAllCollisions()
{
    std::vector<BaseCollider*> snap = colliders;
    const size_t count = snap.size();
    for (size_t i = 0; i < count; ++i)
    {
        for (size_t j = i + 1; j < count; ++j)
        {
            BaseCollider* a = snap[i];
            BaseCollider* b = snap[j];

            if (!a || !b) continue;

            if (CheckPair(a, b))
            {
                if (a->GetOwner())
                    a->GetOwner()->OnCollision(b);
                if (b->GetOwner())
                    b->GetOwner()->OnCollision(a);
            }
        }
    }
}

bool ColliderManager::CheckPair(BaseCollider* a, BaseCollider* b)
{
    switch (a->GetColliderType())
    {
    case ColliderType::CIRCLE:
        switch (b->GetColliderType())
        {
        case ColliderType::CIRCLE:
            return CircleVsCircle(static_cast<CircleCollider*>(a),
                static_cast<CircleCollider*>(b));
        case ColliderType::BOX:
            return CircleVsBox(static_cast<CircleCollider*>(a),
                static_cast<BoxCollider*>(b));
        case ColliderType::CAPSULE:
            return CircleVsCapsule(static_cast<CircleCollider*>(a),
                static_cast<CapsuleCollider*>(b));
        default:
            break;
        }
        break;

    case ColliderType::BOX:
        switch (b->GetColliderType())
        {
        case ColliderType::CIRCLE:
            return CircleVsBox(static_cast<CircleCollider*>(b),
                static_cast<BoxCollider*>(a));
        case ColliderType::BOX:
            return BoxVsBox(static_cast<BoxCollider*>(a),
                static_cast<BoxCollider*>(b));
        case ColliderType::CAPSULE:
            return CapsuleVsBox(static_cast<CapsuleCollider*>(b),
                static_cast<BoxCollider*>(a));
        default:
            break;
        }
        break;

    case ColliderType::CAPSULE:
        switch (b->GetColliderType())
        {
        case ColliderType::CIRCLE:
            return CircleVsCapsule(static_cast<CircleCollider*>(a),
                static_cast<CapsuleCollider*>(b));
        case ColliderType::BOX:
            return CapsuleVsBox(static_cast<CapsuleCollider*>(a),
                static_cast<BoxCollider*>(b));
        case ColliderType::CAPSULE:
            return CapsuleVsCapsule(static_cast<CapsuleCollider*>(a),
                static_cast<CapsuleCollider*>(b));
        default:
            break;
        }
        break;

    default:
        break;
    }

    return false;
}

bool ColliderManager::CircleVsCircle(
    const CircleCollider* a,
    const CircleCollider* b)
{
    Vector2D<float> diff =
        a->GetWorldPosition() - b->GetWorldPosition();

    float r = a->GetRadius() + b->GetRadius();
    return diff.LengthSq() <= r * r;
}

bool ColliderManager::BoxVsBox(
    const BoxCollider* a,
    const BoxCollider* b)
{
    Vector2D<float> pa = a->GetWorldPosition();
    Vector2D<float> pb = b->GetWorldPosition();
    Vector2D<float> ha = a->GetHalfSize();
    Vector2D<float> hb = b->GetHalfSize();

    return
        std::abs(pa.x - pb.x) <= (ha.x + hb.x) &&
        std::abs(pa.y - pb.y) <= (ha.y + hb.y);
}

bool ColliderManager::CircleVsBox(
    const CircleCollider* c,
    const BoxCollider* b)
{
    Vector2D<float> cp = c->GetWorldPosition();
    Vector2D<float> bp = b->GetWorldPosition();
    Vector2D<float> h = b->GetHalfSize();

    float x = MyStd::Clamp(cp.x, bp.x - h.x, bp.x + h.x);
    float y = MyStd::Clamp(cp.y, bp.y - h.y, bp.y + h.y);

    Vector2D<float> closest{ x, y };
    return (cp - closest).LengthSq() <= c->GetRadius() * c->GetRadius();
}

bool ColliderManager::CapsuleVsBox(
    const CapsuleCollider* cap,
    const BoxCollider* box)
{
    // 仮の円コライダーを作って判定を呼ぶ
    CircleCollider topCircle(cap->CapsuleTop(), cap->GetRadius());
    CircleCollider bottomCircle(cap->CapsuleBottom(), cap->GetRadius());

    // CircleVsBox に渡す
    if (CircleVsBox(&topCircle, box)) return true;
    if (CircleVsBox(&bottomCircle, box)) return true;

    // 中央線分との判定は別途実装
    return SegmentVsBox(
        cap->CapsuleBottom(),
        cap->CapsuleTop(),
        box,
        cap->GetRadius());
}

bool ColliderManager::CircleVsCapsule(
    const CircleCollider* circle,
    const CapsuleCollider* cap)
{
    Vector2D<float> cp = circle->GetWorldPosition();
    float r = circle->GetRadius() + cap->GetRadius();

    const Vector2D<float> a = cap->CapsuleBottom() + Vector2D<float>{0, cap->GetRadius()};
    const Vector2D<float> b = cap->CapsuleTop() - Vector2D<float>{0, cap->GetRadius()};

    DrawCircleAA(a.x, a.y, 30.0f, 32, GetColor(255, 0, 0), FALSE);
    DrawCircleAA(b.x, b.y, 30.0f, 32, GetColor(255, 0, 0), FALSE);

    return DistanceSq_Point_Segment(cp, a, b) <= r * r;
}

bool ColliderManager::CapsuleVsCapsule(
    const CapsuleCollider* a,
    const CapsuleCollider* b)
{
    float r = a->GetRadius() + b->GetRadius();

    // a の線分と b の端点
    if (DistanceSq_Point_Segment(b->CapsuleTop(), a->CapsuleBottom(), a->CapsuleTop()) <= r * r)
        return true;
    if (DistanceSq_Point_Segment(b->CapsuleBottom(), a->CapsuleBottom(), a->CapsuleTop()) <= r * r)
        return true;

    // b の線分と a の端点
    if (DistanceSq_Point_Segment(a->CapsuleTop(), b->CapsuleBottom(), b->CapsuleTop()) <= r * r)
        return true;
    if (DistanceSq_Point_Segment(a->CapsuleBottom(), b->CapsuleBottom(), b->CapsuleTop()) <= r * r)
        return true;

    return false;
}

Vector2D<float> ClosestPointOnSegment(
    const Vector2D<float>& a,
    const Vector2D<float>& b,
    const Vector2D<float>& p)
{
    Vector2D<float> ab = b - a;
    float t = (p - a).Dot(ab) / ab.LengthSq();
    t = MyStd::Clamp(t, 0.0f, 1.0f);
    return a + ab * t;
}

bool ColliderManager::SegmentVsBox(
    const Vector2D<float>& a,
    const Vector2D<float>& b,
    const BoxCollider* box,
    float radius)
{
    Vector2D<float> boxCenter = box->GetWorldPosition();
    Vector2D<float> h = box->GetHalfSize();

    Vector2D<float> minPt = boxCenter - h - Vector2D<float>{radius, radius};
    Vector2D<float> maxPt = boxCenter + h + Vector2D<float>{radius, radius};

    Vector2D<float> d = b - a;
    float tmin = 0.0f;
    float tmax = 1.0f;

    for (int axis = 0; axis < 2; ++axis)
    {
        float a_coord = (axis == 0) ? a.x : a.y;
        float d_coord = (axis == 0) ? d.x : d.y;
        float min_coord = (axis == 0) ? minPt.x : minPt.y;
        float max_coord = (axis == 0) ? maxPt.x : maxPt.y;

        if (fabsf(d_coord) < 1e-8f)
        {
           
            if (a_coord < min_coord || a_coord > max_coord)
                return false;
        }
        else
        {
            float ood = 1.0f / d_coord;
            float t1 = (min_coord - a_coord) * ood;
            float t2 = (max_coord - a_coord) * ood;
            if (t1 > t2) std::swap(t1, t2);
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            if (tmin > tmax) return false;
        }
    }

    return (tmax >= 0.0f && tmin <= 1.0f);
}


float ColliderManager::DistanceSq_Point_Segment(
    const Vector2D<float>& p,
    const Vector2D<float>& a,
    const Vector2D<float>& b)
{
    Vector2D<float> ab = b - a;
    Vector2D<float> ap = p - a;

    float t = ap.Dot(ab) / ab.Dot(ab);
    t = MyStd::Clamp(t, 0.0f, 1.0f);

    Vector2D<float> closest = a + ab * t;
    return (p - closest).LengthSq();
}

#if DEBUG
void ColliderManager::DebugDraw() 
{
    for (auto& a : colliders) {
        a->DebugDraw();
    }
}
#endif