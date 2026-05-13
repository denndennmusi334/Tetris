#include "stdafx.h"
#include "ImageLoadManager.h"

void ImageLoadManager::Initialize()
{
	const TCHAR* tex[TEX_COUNT] = {
		_T("texture/background_forest.png"),
		_T("texture/background_cave.png"),
	};


	for (int i = 0; i < TEX_COUNT; i++)
	{
		grHandle[i] = LoadGraph(tex[i], FALSE);
		assert(grHandle[i] >= 0);
	}
}

void ImageLoadManager::Finalize()
{
	for(int i = 0;i < TEX_COUNT;i++)
	{
		DeleteGraph(grHandle[i]);
	}
}