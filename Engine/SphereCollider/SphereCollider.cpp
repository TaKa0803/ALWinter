#include "SphereCollider.h"
#include"InstancingModelManager/InstancingModelManager.h"

void SphereCollider::Initialize(const WorldTransform&world) {
	InstancingGameObject::Initialize("sphere");

	world_.SetParent(&world);

	
}

void SphereCollider::Update() {
	
	world_.scale_ = { wide_,wide_,wide_ };
	world_.UpdateMatrix();
	
}

void SphereCollider::Draw() {
	InstancingModelManager::GetInstance()->SetWorld(tag_, world_);
}

bool SphereCollider::IsHit(const SphereCollider& sphere, Vector3 backVec) {
	Vector3 pos = sphere.world_.GetMatWorldTranslate();
	Vector3 myP = world_.GetMatWorldTranslate();

	float wide = sphere.wide_;
	float myW = wide_;

	Vector3 sub = myP - pos;

	//はんいない
	if (Length(sub) < wide + myW) {

		sub.SetNormalize();

		//基コライダーから自分への向きベクトル
		backVec = sub * (wide, myW);

		return true;
	}


	return false;
}

