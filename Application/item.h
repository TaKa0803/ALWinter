#pragma once
#include"WorldTransform/WorldTransform.h"


//モデルの数
static const int modelNum_ = 5;


struct ModelAnimeParts {
	int RoopFrame;
	WorldTransform stPartsWorlds[modelNum_];
	WorldTransform edPartsWorlds[modelNum_];
};

Vector3 Esing(const Vector3& st, const Vector3& ed, const float t);