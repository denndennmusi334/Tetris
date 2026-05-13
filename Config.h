#pragma once

namespace Config
{
    constexpr int ScreenWidth = 1280;
    constexpr int ScreenHeight = 720;

    constexpr int ScreenFPS = 60;
}

namespace Math
{
    template<typename T>
    constexpr T Clamp(T v, T min, T max)
    {
        return (v < min) ? min : (v > max) ? max : v;
    }
}