#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vector3 : public XMFLOAT3 {
public:

	Vector3() = default;
	Vector3(const Vector3&) = default;
	Vector3& operator=(const Vector3&) = default;
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(XMFLOAT3 r) {
		this->x = r.x;
		this->y = r.y;
		this->z = r.z;
	}
	Vector3(const XMVECTOR &vec) : XMFLOAT3() {
		XMStoreFloat3(this, vec);
	}
	
	//2€‰‰Zq
	Vector3 operator*(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	Vector3 operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3 operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3 operator*=(const Vector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vector3 operator/=(const Vector3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vector3 operator+(XMFLOAT3& r) {
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}

	Vector3 operator-(XMFLOAT3& r) {
		x += r.x;
		y += r.y;
		z += r.z;
		return *this;
	}

	Vector3 operator*(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3 operator/(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	Vector3 operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector3 operator/=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	/// <summary>
	/// ’·‚³‚ğ‹‚ß‚é
	/// </summary>
	/// <returns>’·‚³</returns>
	float length() const {
		return(float)sqrtf(x * x + y * y + z * z);
	}

	/// <summary>
	/// “àÏ
	/// </summary>
	/// <returns>“àÏ</returns>
	float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	/// <summary>
	/// ŠOÏ‚ğ‹‚ß‚é
	/// </summary>
	/// <returns>ŠOÏ</returns>
	Vector3 cross(const Vector3& v) const {
		Vector3 temp;
		temp.x = this->y * v.z - this->z * v.y;
		temp.y = this->z * v.x - this->x * v.z;
		temp.z = this->x * v.y - this->y * v.x;
	}

	/// <summary>
	/// ³‹K‰»
	/// </summary>
	/// <returns>³‹K‰»‚µ‚½’l</returns>
	Vector3 normalize() {
		float len = length();
		if (len != 0) {
			return *this /= len;
		}
		return *this;
	}
};