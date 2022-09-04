#include "Collision.h"

Collision* Collision::GetIns()
{
	static Collision instance;
	return &instance;
}

bool Collision::SphereCollision(Object3d* object1, Object3d* object2) {
	object1Pos = object1->GetMatWorld().r[3];
	object1Scale = object1->GetScale();

	object2Pos = object2->GetMatWorld().r[3];
	object2Scale = object2->GetScale();

	objectPosTotal = (object2Pos.x - object1Pos.x) * (object2Pos.x - object1Pos.x) + (object2Pos.y - object1Pos.y) * (object2Pos.y - object1Pos.y) + (object2Pos.z - object1Pos.z) * (object2Pos.z - object1Pos.z);
	objectScaleTotal = (object1Scale.x + object2Scale.x) * (object1Scale.x + object2Scale.x);

	if (!(objectPosTotal <= objectScaleTotal)) return false;

	return true;
}