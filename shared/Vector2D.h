#pragma once
#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);
    float Distance(const Vector2D& vec);

	friend bool operator==(const Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	Vector2D& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};

inline std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
    stream << "(" << vec.x << "," << vec.y << ")";
    return stream;
}

inline bool operator==(const Vector2D& v1, const Vector2D& v2)
{
    return (v1.x == v2.x && v1.y == v2.y);
}

inline Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D newVector;
    newVector.x = v1.x + v2.x;
    newVector.y = v1.y + v2.y;
    return newVector;
}

inline Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D newVector;
    newVector.x = v1.x - v2.x;
    newVector.y = v1.y - v2.y;
    return newVector;
}

inline Vector2D operator*(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D newVector;
    newVector.x = v1.x * v2.x;
    newVector.y = v1.y * v2.y;
    return newVector;
}

inline Vector2D operator/(const Vector2D& v1, const Vector2D& v2)
{
    Vector2D newVector;
    newVector.x = v1.x / v2.x;
    newVector.y = v1.y / v2.y;
    return newVector;
}

inline Vector2D operator*(const Vector2D& v1, const int& i)
{
    Vector2D newVector;
    newVector.x = v1.x * i;
    newVector.y = v1.y * i;
    return newVector;
}