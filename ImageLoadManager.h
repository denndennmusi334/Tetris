#pragma once
#include "Job.h"

enum E_TEXTURE
{
    BLOCK_LIGHT_BLUE,
    BLOCK_BLUE,
    BLOCK_ORANGE,
    BLOCK_YELLOW,
    BLOCK_GREEN,
    BLOCK_PURPLE,
    BLOCK_RED,

	TEX_COUNT
};

class ImageLoadManager : public Job
{
private:
	int grHandle[TEX_COUNT];
    ImageLoadManager()
    {
        memset(grHandle, 0, sizeof(grHandle));
    }
    ~ImageLoadManager() = default;
public:
    ImageLoadManager(const ImageLoadManager&) = delete;
    ImageLoadManager& operator=(const ImageLoadManager&) = delete;

    void Initialize() override ;
    void Finalize() override ;
    void Update() override {};
	void Draw() override {};

    int getGraphHandle(E_TEXTURE tex) { return grHandle[tex]; }

    static ImageLoadManager& GetInstance() {
        static ImageLoadManager instance;
        return instance;
    }
};
