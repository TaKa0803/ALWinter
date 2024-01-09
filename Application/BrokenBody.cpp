#include "BrokenBody.h"

BrokenBody* BrokenBody::GetInstance() {
	static BrokenBody instance;
	return&instance;
}

void BrokenBody::Initialize() {
	InstancingGameObject::Initialize("EBox");
}

void BrokenBody::Update() {
	for (auto& eData : effectDatas_) {
		eData->velocity_ += eData->accce_;
		eData->world.translate_ += eData->velocity_;

		eData->world.UpdateMatrix();

		IMM_->SetWorld(tag_, eData->world);

	}
}

void BrokenBody::EffectOccurred(const WorldTransform& world,int spawnNum) {

	for (int count = 0; count < spawnNum; count) {

		std::unique_ptr<EffectData>edata;

		//ワールド
		edata->world = world;

	}


}

