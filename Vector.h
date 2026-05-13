#pragma once

#include <cmath>
#include <type_traits>
#include <limits>

// PI 定数（確実に使えるように自前定義）
static constexpr double VEC_PI = 3.14159265358979323846;

template<typename T>
struct Vector2D
{
    T x;
    T y;

    Vector2D() noexcept = default;
    constexpr Vector2D(const T& x_, const T& y_) noexcept : x(x_), y(y_) {}
    explicit constexpr Vector2D(const T& v) noexcept : x(v), y(v) {}

    // ドット / クロス（2D のクロスはスカラー）
    constexpr T Dot(const Vector2D& v) const noexcept {
        return (v.x * x) + (v.y * y);
    }

    constexpr T Cross(const Vector2D& v) const noexcept {
        return x * v.y - y * v.x;
    }

    constexpr T LengthSq() const noexcept {
        return Dot(*this);
    }

    // Length は sqrt を使うため constexpr にはしない
    double Length() const noexcept {
        return std::sqrt(static_cast<double>(LengthSq()));
    }

    // 常に double を返す
    double Distance(const Vector2D& v) const noexcept {
        return (*this - v).Length();
    }

    // float で正規化して返す（元の実装互換）
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

    // 同じ型で正規化（T が浮動小数点型のときのみ有効）
    template<typename U = T>
    inline std::enable_if_t<std::is_floating_point_v<U>, Vector2D<U>> NormalizedSameType() const noexcept {
        double len = Length();
        if (len == 0.0) {
            return Vector2D<U>{static_cast<U>(0), static_cast<U>(0)};
        }
        return Vector2D<U>{ static_cast<U>(x / len), static_cast<U>(y / len) };
    }

    // 角度（ラジアン）からベクトル生成（cos/sin を使うため constexpr にしていない）
    static inline Vector2D FromAngleRad(const double rad) noexcept {
        return Vector2D(static_cast<T>(std::cos(rad)), static_cast<T>(std::sin(rad)));
    }
    static inline Vector2D FromAngleDeg(const double deg) noexcept {
        return FromAngleRad(deg * (VEC_PI / 180.0));
    }

    double AngleRad() const noexcept {
        return std::atan2(static_cast<double>(y), static_cast<double>(x));
    }

    double AngleDeg() const noexcept {
        return AngleRad() * (180.0 / VEC_PI);
    }

    bool IsNormalized(double eps = 1e-6) const noexcept {
        return std::abs(LengthSq() - 1.0) < eps;
    }

    constexpr bool IsLeftOf(const Vector2D& v) const noexcept {
        return Cross(v) > 0;
    }

    constexpr bool IsRightOf(const Vector2D& v) const noexcept {
        return Cross(v) < 0;
    }

    T CrossLine(const Vector2D& A, const Vector2D& B) const noexcept {
        // ベクトル AB
        Vector2D AB = B - A;
        // ベクトル AC
        Vector2D AC = *this - A;
        // Cross(AB, AC) で左右判定
        return AB.Cross(AC);
    }

    bool IsLeftOfLine(const Vector2D& A, const Vector2D& B) const noexcept {
        return CrossLine(A, B) > 0;
    }

    bool IsRightOfLine(const Vector2D& A, const Vector2D& B) const noexcept {
        return CrossLine(A, B) < 0;
    }

    bool IsOnLine(const Vector2D& A, const Vector2D& B) const noexcept {
        return CrossLine(A, B) == 0;
    }
};

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

// aliases
using Vec2f = Vector2D<float>;
using Vec2i = Vector2D<int>;