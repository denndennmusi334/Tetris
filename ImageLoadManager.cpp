#include "stdafx.h"
#include "ImageLoadManager.h"

void ImageLoadManager::Initialize()
{
	/*
	const TCHAR* tex[TEX_COUNT] = {

	};


	for (int i = 0; i < TEX_COUNT; i++)
	{
		grHandle[i] = LoadGraph(tex[i], FALSE);
		assert(grHandle[i] >= 0);
	}
	*/

	LoadDivGraph(L"Resources/Images/テトリス素材.png", 8, 8, 1, 32, 32, &grHandle[0]);
}

void ImageLoadManager::Finalize()
{
	for(int i = 0;i < TEX_COUNT;i++)
	{
		DeleteGraph(grHandle[i]);
	}
}
