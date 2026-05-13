#pragma once
#include "BaseCollider.h"
#include "Drawable.h"

class GameObject : public Drawable
{
private:
    Vector2D<float> pos = { 0,0 };
    std::vector<std::unique_ptr<BaseCollider>> colliders;


public:
    
    Vector2D<float> GetPosition() const { return pos; }
    void SetPosition(const Vector2D<float>& p) { pos = p; }

    void AddCollider(std::unique_ptr<BaseCollider> col);

    const auto& GetColliders() const { return colliders; }

    virtual void OnCollision(BaseCollider* other) {};

	void CreateAnimation() override {};

    void Initialize()   override {}; //‰Šú‰».
    void Finalize()     override {}; //I—¹ˆ—.
    void Update()       override {}; //XV.
    void Draw(const Camera& camera) override {};

};