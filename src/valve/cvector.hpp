#pragma once

#include <cmath>
#include <format>
#include <numbers>
#include <ostream>

class CVector
{
public:
	constexpr CVector operator+(const CVector& other) const noexcept
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	constexpr CVector operator-(const CVector& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	constexpr CVector operator*(const CVector& other) const noexcept
	{
		return { x * other.x, y * other.y, z * other.z };
	}

	constexpr CVector operator/(const CVector& other) const noexcept
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	constexpr bool operator==(const CVector& other) const noexcept {
		return x == other.x && y == other.y && z == other.z;
	}

	constexpr bool operator!=(const CVector& other) const noexcept {
		return x != other.x && y != other.y && z != other.z;
	}

	constexpr CVector Scale(float factor) const noexcept
	{
		return { x * factor, y * factor, z * factor };
	}

	float Length() const noexcept {
		return sqrt(LengthSqr());
	}

	float LengthSqr() const noexcept {
		return x * x + y * y;
	}

	constexpr float Dot(const CVector& other) noexcept {
		return x * other.x + y * other.y + z * other.z;
	}

	constexpr float Dot(float* other)  noexcept {
		return x * other[0] + y * other[1] + z * other[2];
	}

	inline CVector ToAngle() const noexcept
	{
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}

	friend std::ostream& operator<<(std::ostream& os, const CVector& vec) noexcept
	{
		os << std::format("{},{},{}", vec.x, vec.y, vec.z);
		return os;
	}

	float x{ }, y{ }, z{ };
};

__declspec(align(16)) class CVectorAligned : public CVector
{
public:
	constexpr CVectorAligned operator-(const CVectorAligned& other) const noexcept
	{
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}

	float w{ };
};