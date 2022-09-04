#pragma once
#include "Object3d.h"
#include "Vector3.h"
#include "DirectXMath.h"

class Collision final
{
private:
	Collision() = default;
	~Collision() = default;
	Collision(const Collision& obj) = delete;
	Collision& operator=(const Collision& obj) = delete;

public: //静的メンバ関数
	/// <summary>
	/// インスタンス取得用関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Collision* GetIns();

public: //メンバ関数

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="object1">判定したいオブジェクト1</param>
	/// <param name="object2">判定したいオブジェクト2</param>
	/// <returns>当たった</returns>
	bool SphereCollision(Object3d* object1, Object3d* object2);

private: //メンバ変数
	Vector3 object1Pos = { 0, 0, 0 };
	Vector3 object2Pos = { 0, 0, 0 };

	Vector3 object1Scale = { 0, 0, 0 };
	Vector3 object2Scale = { 0, 0, 0 };

	float objectPosTotal = 0;
	float objectScaleTotal = 0;
};

