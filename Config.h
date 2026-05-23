#pragma once

namespace Config
{
    constexpr int SCREEN_WIDTH = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    constexpr int SCREEN_FPS = 60;

    // ===== テトリス =====

    constexpr int FIELD_WIDTH = 10;
    constexpr int FIELD_HEIGHT = 22;

    constexpr int BLOCK_SIZE = 32;

    constexpr int FIELD_X = 400;
    constexpr int FIELD_Y = 40;

    constexpr int VISIBLE_HEIGHT = 20;

    constexpr int FIELD_PIXEL_W = FIELD_WIDTH * BLOCK_SIZE;
    constexpr int FIELD_PIXEL_H = VISIBLE_HEIGHT * BLOCK_SIZE;

	// ===== 背景 =====

	constexpr int STAR_COUNT = 100;
}
