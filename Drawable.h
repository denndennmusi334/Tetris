#pragma once
#include "Job.h"
#include "Camera.h"
#include "ImageLoadManager.h"
#include "Animator.h"

//•`‰æ‚ÌŠî’êƒNƒ‰ƒX.
class Drawable :
    public Job
{
protected:
	Animator animator;
	bool isDestroyed = false;
private:
    int drawLayer = 0;
public:
    virtual ~Drawable() {}

    virtual void Draw(const Camera& camera) = 0;
    virtual void CreateAnimation() {};

	void Destroy() { isDestroyed = true; }
	bool IsDestroyed() const { return isDestroyed; }

    std::function<void()> onLayerChanged;
    void SetDrawLayer(int _drawLayer) {
        drawLayer = _drawLayer;
		assert(onLayerChanged && "onLayerChanged ‚Ì’†g‚ª‚È‚¢‚æ.");
        if (onLayerChanged)
            onLayerChanged();
    }

    int GetDrawLayer() const { return drawLayer; }
};

