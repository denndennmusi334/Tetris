#pragma once

#include <cmath>
#include <type_traits>
#include <limits>
#include "MyStd.h"

/// <summary>
/// 円周率 PI
/// </summary>
static constexpr double VEC_PI = 3.14159265358979323846;

/// <summary>
/// 2次元ベクトル
/// </summary>
/// <typeparam name="T">
/// 座標の型(int,float,doubleなど)
/// </typeparam>
template<typename T>
struct Vector2D
{
    T x;
    T y;

    /// <summary>
    /// デフォルトコンストラクタ
    /// </summary>
    Vector2D() noexcept = default;

    /// <summary>
    /// x,y を指定して生成
    /// </summary>
    /// <param name="x_">x座標</param>
    /// <param name="y_">y座標</param>
    constexpr Vector2D(const T& x_, const T& y_) noexcept
        : x(x_), y(y_) {
    }

    /// <summary>
    /// x,y に同じ値を入れて生成
    /// </summary>
    /// <example>
    /// Vector2D<float> v(1.0f);
    /// → (1.0f,1.0f)
    /// </example>
    explicit constexpr Vector2D(const T& v) noexcept
        : x(v), y(v) {
    }

    /// <summary>
    /// ドット積
    /// </summary>
    /// <param name="v">対象ベクトル</param>
    /// <returns>
    /// ドット積の結果
    /// </returns>
    /// <example>
    /// float d = a.Dot(b);
    /// </example>
    constexpr T Dot(const Vector2D& v) const noexcept {
        return (v.x * x) + (v.y * y);
    }

    /// <summary>
    /// 外積
    /// </summary>
    /// <param name="v">対象ベクトル</param>
    /// <returns>
    /// 外積結果
    /// 0より大きい → 左側
    /// 0より小さい → 右側
    /// 0 → 同一直線
    /// </returns>
    constexpr T Cross(const Vector2D& v) const noexcept {
        return x * v.y - y * v.x;
    }

    /// <summary>
    /// ベクトルの長さの二乗
    /// </summary>
    /// <returns>
    /// x*x + y*y
    /// </returns>
    /// <remarks>
    /// sqrt を使わないので高速
    /// 距離比較などに便利
    /// </remarks>
    constexpr T LengthSq() const noexcept {
        return Dot(*this);
    }

    /// <summary>
    /// ベクトルの長さ
    /// </summary>
    /// <returns>
    /// ベクトルの大きさ
    /// </returns>
    /// <example>
    /// double len = v.Length();
    /// </example>
    double Length() const noexcept {
        return std::sqrt(static_cast<double>(LengthSq()));
    }

    /// <summary>
    /// 2点間の距離
    /// </summary>
    /// <param name="v">対象座標</param>
    /// <returns>
    /// 距離
    /// </returns>
    double Distance(const Vector2D& v) const noexcept {
        return (*this - v).Length();
    }

    /// <summary>
    /// 正規化したベクトルを返す
    /// </summary>
    /// <returns>
    /// 長さ1のベクトル
    /// </returns>
    /// <remarks>
    /// 戻り値は float 型
    /// </remarks>
    /// <example>
    /// auto dir = v.Normalize();
    /// </example>
    inline Vector2D<float> Normalize() const noexcept {
        double len = Length();

        if (len == 0.0) {
            return Vector2D<float>{0.0f, 0.0f};
        }

        return Vector2D<float>{
            static_cast<float>(x / len),
                static_cast<float>(y / len)
        };
    }

    /// <summary>
    /// 同じ型で正規化
    /// </summary>
    /// <returns>
    /// 正規化されたベクトル
    /// </returns>
    /// <remarks>
    /// float,double の時のみ使用可能
    /// </remarks>
    template<typename U = T>
    inline std::enable_if_t<std::is_floating_point_v<U>, Vector2D<U>>
        NormalizedSameType() const noexcept {

        double len = Length();

        if (len == 0.0) {
            return Vector2D<U>{
                static_cast<U>(0),
                    static_cast<U>(0)
            };
        }

        return Vector2D<U>{
            static_cast<U>(x / len),
                static_cast<U>(y / len)
        };
    }

    /// <summary>
    /// ラジアン角度からベクトル生成
    /// </summary>
    /// <param name="rad">ラジアン角度</param>
    /// <returns>
    /// 方向ベクトル
    /// </returns>
    /// <example>
    /// auto dir = Vector2D<float>::FromAngleRad(rad);
    /// </example>
    static inline Vector2D FromAngleRad(const double rad) noexcept {
        return Vector2D(
            static_cast<T>(std::cos(rad)),
            static_cast<T>(std::sin(rad))
        );
    }

    /// <summary>
    /// 度数法角度からベクトル生成
    /// </summary>
    /// <param name="deg">角度</param>
    /// <returns>
    /// 方向ベクトル
    /// </returns>
    static inline Vector2D FromAngleDeg(const double deg) noexcept {
        return FromAngleRad(deg * (VEC_PI / 180.0));
    }

    /// <summary>
    /// ベクトルの角度をラジアンで取得
    /// </summary>
    /// <returns>
    /// ラジアン角度
    /// </returns>
    double AngleRad() const noexcept {
        return std::atan2(
            static_cast<double>(y),
            static_cast<double>(x)
        );
    }

    /// <summary>
    /// ベクトルの角度を度数法で取得
    /// </summary>
    /// <returns>
    /// 度数法角度
    /// </returns>
    double AngleDeg() const noexcept {
        return AngleRad() * (180.0 / VEC_PI);
    }

    /// <summary>
    /// 正規化されているか判定
    /// </summary>
    /// <param name="eps">
    /// 許容誤差
    /// </param>
    /// <returns>
    /// true : 正規化済み
    /// false : 未正規化
    /// </returns>
    bool IsNormalized(double eps = 1e-6) const noexcept {
        return std::abs(LengthSq() - 1.0) < eps;
    }

    /// <summary>
    /// 対象ベクトルの左側にあるか
    /// </summary>
    /// <param name="v">対象ベクトル</param>
    /// <returns>
    /// true : 左側
    /// false : 左側ではない
    /// </returns>
    constexpr bool IsLeftOf(const Vector2D& v) const noexcept {
        return Cross(v) > 0;
    }

    /// <summary>
    /// 対象ベクトルの右側にあるか
    /// </summary>
    /// <param name="v">対象ベクトル</param>
    /// <returns>
    /// true : 右側
    /// false : 右側ではない
    /// </returns>
    constexpr bool IsRightOf(const Vector2D& v) const noexcept {
        return Cross(v) < 0;
    }

    /// <summary>
    /// 線分 AB に対する外積
    /// </summary>
    /// <param name="A">線分開始点</param>
    /// <param name="B">線分終了点</param>
    /// <returns>
    /// 外積結果
    /// </returns>
    T CrossLine(const Vector2D& A, const Vector2D& B) const noexcept {

        Vector2D AB = B - A;
        Vector2D AC = *this - A;

        return AB.Cross(AC);
    }

    /// <summary>
    /// 線分 AB の左側にあるか
    /// </summary>
    /// <param name="A">線分開始点</param>
    /// <param name="B">線分終了点</param>
    /// <returns>
    /// true : 左側
    /// </returns>
    bool IsLeftOfLine(
        const Vector2D& A,
        const Vector2D& B
    ) const noexcept {

        return CrossLine(A, B) > 0;
    }

    /// <summary>
    /// 線分 AB の右側にあるか
    /// </summary>
    /// <param name="A">線分開始点</param>
    /// <param name="B">線分終了点</param>
    /// <returns>
    /// true : 右側
    /// </returns>
    bool IsRightOfLine(
        const Vector2D& A,
        const Vector2D& B
    ) const noexcept {

        return CrossLine(A, B) < 0;
    }

    /// <summary>
    /// 線分 AB 上にあるか
    /// </summary>
    /// <param name="A">線分開始点</param>
    /// <param name="B">線分終了点</param>
    /// <returns>
    /// true : 線上
    /// </returns>
    bool IsOnLine(
        const Vector2D& A,
        const Vector2D& B
    ) const noexcept {

        return CrossLine(A, B) == 0;
    }
};

#pragma region Vec2 計算用.

// unary -
template<typename T>
constexpr Vector2D<T> operator-(const Vector2D<T>& v) noexcept {
    return Vector2D<T>{ -v.x, -v.y };
}

// vector <op> vector
template<typename T>
inline constexpr Vector2D<T> operator+ (Vector2D<T> v1, const Vector2D<T>& v2) noexcept {
    v1.x += v2.x; v1.y += v2.y; return v1;
}
template<typename T>
inline constexpr Vector2D<T> operator- (Vector2D<T> v1, const Vector2D<T>& v2) noexcept {
    v1.x -= v2.x; v1.y -= v2.y; return v1;
}
template<typename T>
inline constexpr Vector2D<T> operator* (Vector2D<T> v1, const Vector2D<T>& v2) noexcept {
    v1.x *= v2.x; v1.y *= v2.y; return v1;
}
template<typename T>
inline constexpr Vector2D<T> operator/ (Vector2D<T> v1, const Vector2D<T>& v2) noexcept {
    v1.x /= v2.x; v1.y /= v2.y; return v1;
}

// equality (注意: 浮動小数点は近似比較を検討)
template<typename T>
inline bool operator== (const Vector2D<T>& v1, const Vector2D<T>& v2) noexcept {
    return (v1.x == v2.x) && (v1.y == v2.y);
}
template<typename T>
inline bool operator!= (const Vector2D<T>& v1, const Vector2D<T>& v2) noexcept {
    return !(v1 == v2);
}

// compound assignments (vector)
template<typename T>
constexpr Vector2D<T>& operator+=(Vector2D<T>& lhs, const Vector2D<T>& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}
template<typename T>
constexpr Vector2D<T>& operator-=(Vector2D<T>& lhs, const Vector2D<T>& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}
template<typename T>
constexpr Vector2D<T>& operator*=(Vector2D<T>& lhs, const Vector2D<T>& rhs) noexcept {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}
template<typename T>
constexpr Vector2D<T>& operator/=(Vector2D<T>& lhs, const Vector2D<T>& rhs) noexcept {
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

// vector <op> scalar
template<typename T>
constexpr Vector2D<T> operator+(Vector2D<T> v, const T& s) noexcept {
    v.x += s; v.y += s; return v;
}
template<typename T>
constexpr Vector2D<T> operator-(Vector2D<T> v, const T& s) noexcept {
    v.x -= s; v.y -= s; return v;
}
template<typename T>
constexpr Vector2D<T> operator*(Vector2D<T> v, const T& s) noexcept {
    v.x *= s; v.y *= s; return v;
}
template<typename T>
constexpr Vector2D<T> operator/(Vector2D<T> v, const T& s) noexcept {
    v.x /= s; v.y /= s; return v;
}

// scalar <op> vector
template<typename T>
constexpr Vector2D<T> operator+(const T& s, Vector2D<T> v) noexcept {
    v.x += s; v.y += s; return v;
}
template<typename T>
constexpr Vector2D<T> operator*(const T& s, Vector2D<T> v) noexcept {
    v.x *= s; v.y *= s; return v;
}

// compound assignments (scalar)
template<typename T>
constexpr Vector2D<T>& operator+=(Vector2D<T>& v, const T& s) noexcept {
    v.x += s; v.y += s; return v;
}
template<typename T>
constexpr Vector2D<T>& operator-=(Vector2D<T>& v, const T& s) noexcept {
    v.x -= s; v.y -= s; return v;
}
template<typename T>
constexpr Vector2D<T>& operator*=(Vector2D<T>& v, const T& s) noexcept {
    v.x *= s; v.y *= s; return v;
}
template<typename T>
constexpr Vector2D<T>& operator/=(Vector2D<T>& v, const T& s) noexcept {
    v.x /= s; v.y /= s; return v;
}
#pragma endregion

// aliases
using Vec2f = Vector2D<float>;
using Vec2i = Vector2D<int>;

#pragma region Dxlibの関数Vec2

namespace MyDrawDxlib
{
	inline void M_DrawCircle(const Vec2i& center, int radius, unsigned int color, bool fill = false,int LineThickness = 1) {
        DrawCircle(center.x, center.y, radius, color, MyStd::Cast<int>(fill), LineThickness);
	}

    inline void M_DrawCircleAA(const Vec2f& center, float radius, int posnum, unsigned int color, bool fill = false, float lineThickness = 1, float angle = 0) {
        DrawCircleAA(center.x, center.y, radius, posnum, color, MyStd::Cast<int>(fill), lineThickness, angle);
    }

	inline void M_DrawLine(const Vec2i& start, const Vec2i& end, unsigned int color, int LineThickness = 1) {
		DrawLine(start.x, start.y, end.x, end.y, color, LineThickness);
	}

	inline void M_DrawLineAA(const Vec2f& start, const Vec2f& end, unsigned int color, float LineThickness = 1) {
		DrawLineAA(start.x, start.y, end.x, end.y, color, LineThickness);
	}
}

#pragma endregion
