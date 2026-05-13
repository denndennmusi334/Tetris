#pragma once

#include "Job.h"
#include "SceneController.h"
#include "DrawManager.h"

class BaseScene: public Job
{
protected:
    SceneController* sceneController;
    
    DrawManager drawMgr;
    Camera camera;
public:
    BaseScene(SceneController* changer);
    ~BaseScene()        override {};
    void Initialize()   override {}; //‰Šú‰».
    void Finalize()     override {}; //I—¹ˆ—.
    void Update()       override {}; //XV.
    void Draw()         override {}; //•`‰æ.
    virtual void Kill() = 0;

    void AddDrawable(Drawable* obj)
    {
        drawMgr.Add(obj);
    }
}; 