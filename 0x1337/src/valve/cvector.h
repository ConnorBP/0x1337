#pragma once
#include <cstdint>
#include <numbers>
#include <cmath>


constexpr float Deg2Rad(const float deg) noexcept
{
	return deg * (std::numbers::pi_v<float> / 180.f);
}


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

	constexpr CVector operator*(const float scale) const noexcept
	{
		return { x * scale, y * scale, z * scale };
	}

	constexpr CVector operator/(const CVector& other) const noexcept
	{
		return { x / other.x, y / other.y, z / other.z };
	}

	constexpr CVector Scale(float factor) const noexcept
	{
		return { x * factor, y * factor, z * factor };
	}

	// convert angles -> vector
	CVector AngleToVector() const noexcept
	{
		return {
			std::cos(Deg2Rad(x)) * std::cos(Deg2Rad(y)),
			std::cos(Deg2Rad(x)) * std::sin(Deg2Rad(y)),
			-std::sin(Deg2Rad(x))
		};
	}

public:

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