#pragma once
#include <random>
namespace MyStd
{
#pragma region Random
	/// <summary>
	/// シードの入手と乱数エンジンの生成を行う関数.
	/// </summary>
	/// <returns></returns>
	inline std::mt19937& GetEngine()
	{
		static std::random_device rd;
		static std::mt19937 rng(rd());

		return rng;
	}
	/// <summary>
	/// ランダムな整数を min ～ max の範囲で生成する関数.
	/// </summary>
	/// <param name="min">最小値(int)</param>
	/// <param name="max">最大値(int)</param>
	/// <returns>乱数結果</returns>
	inline int IRange(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(GetEngine());
	}
	/// <summary>
	/// ランダムな浮動小数点数を min ～ max の範囲で生成する関数.
	/// </summary>
	/// <param name="min">最小値(float)</param>
	/// <param name="max">最大値(float)</param>
	/// <returns></returns>
	inline float FRange(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);
		return dist(GetEngine());
	}

#pragma endregion

#pragma region Math

	/// <summary>
	/// 値を最小値～最大値の範囲に収める
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="v">値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>
	/// 範囲内に収められた値
	/// </returns>
	template<typename T>
	constexpr T Clamp(T v, T min, T max)
	{
		return (v < min) ? min : (v > max) ? max : v;
	}

	/// <summary>
	/// 絶対値を取得
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="v">値</param>
	/// <returns>
	/// 絶対値
	/// </returns>
	template<typename T>
	constexpr T Abs(T v)
	{
		return (v < 0) ? -v : v;
	}

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="a">開始値</param>
	/// <param name="b">終了値</param>
	/// <param name="t">
	/// 補間値
	/// 0.0 ～ 1.0
	/// </param>
	/// <returns>
	/// 補間後の値
	/// </returns>
	/// <example>
	/// float x = Lerp(0.0f, 100.0f, 0.5f);
	/// → 50.0f
	/// </example>
	template<typename T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	/// <summary>
	/// 符号を取得
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="v">値</param>
	/// <returns>
	/// 正数 : 1
	/// 負数 : -1
	/// 0 : 0
	/// </returns>
	template<typename T>
	constexpr int Sign(T v)
	{
		return (v > 0) - (v < 0);
	}

	/// <summary>
	/// 値が範囲内か判定
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="v">値</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>
	/// true : 範囲内
	/// false : 範囲外
	/// </returns>
	template<typename T>
	constexpr bool IsInRange(T v, T min, T max)
	{
		return v >= min && v <= max;
	}
#pragma endregion

#pragma region Cast

	inline std::wstring Utf8ToWstring(const std::string& str)
	{
		int size = MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			-1,
			nullptr,
			0);

		std::wstring result(size - 1, 0);

		MultiByteToWideChar(
			CP_UTF8,
			0,
			str.c_str(),
			-1,
			&result[0],
			size);

		return result;
	}

	/// <summary>
	/// static_cast を簡単に書くための関数
	/// </summary>
	/// <typeparam name="T">変換後型</typeparam>
	/// <typeparam name="U">変換前型</typeparam>
	/// <param name="value">値</param>
	/// <returns>
	/// 変換後の値
	/// </returns>
	template<typename T, typename U>
	constexpr T Cast(U value)
	{
		return static_cast<T>(value);
	}

	/// <summary>
	/// int 型へ変換
	/// </summary>
	/// <typeparam name="T">変換前型</typeparam>
	/// <param name="value">値</param>
	/// <returns>
	/// int 型の値
	/// </returns>
	template<typename T>
	constexpr int ICast(T value)
	{
		return static_cast<int>(value);
	}

	/// <summary>
	/// float 型へ変換
	/// </summary>
	/// <typeparam name="T">変換前型</typeparam>
	/// <param name="value">値</param>
	/// <returns>
	/// float 型の値
	/// </returns>
	template<typename T>
	constexpr float FCast(T value)
	{
		return static_cast<float>(value);
	}

	/// <summary>
	/// bool 型へ変換
	/// </summary>
	/// <typeparam name="T">変換前型</typeparam>
	/// <param name="value">値</param>
	/// <returns>
	/// bool 型の値
	/// </returns>
	template<typename T>
	constexpr bool BoolCast(T value)
	{
		return static_cast<bool>(value);
	}
#pragma endregion

#pragma region Angle

	/// <summary>
	/// 円周率 PI
	/// </summary>
	constexpr float PI = 3.14159265358979323846f;

	/// <summary>
	/// 度数法をラジアンへ変換
	/// </summary>
	/// <param name="deg">角度</param>
	/// <returns>
	/// ラジアン値
	/// </returns>
	/// <example>
	/// float rad = DegToRad(90.0f);
	/// </example>
	constexpr float DegToRad(float deg)
	{
		return deg * PI / 180.0f;
	}

	/// <summary>
	/// ラジアンを度数法へ変換
	/// </summary>
	/// <param name="rad">ラジアン値</param>
	/// <returns>
	/// 角度
	/// </returns>
	/// <example>
	/// float deg = RadToDeg(rad);
	/// </example>
	constexpr float RadToDeg(float rad)
	{
		return rad * 180.0f / PI;
	}

#pragma endregion

}
