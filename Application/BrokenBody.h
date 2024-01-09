#pragma once
#include"Instancing/InstancingGameObject.h"

class BrokenBody :public InstancingGameObject {

public://シングルトンパターン
	static BrokenBody* GetInstance();

private://シングルトンパターン

	BrokenBody() = default;
	~BrokenBody() = default;
	BrokenBody(const BrokenBody& o) = delete;
	const BrokenBody& operator=(const BrokenBody& o) = delete;



public:

	void Initialize();

	void Update();

	void EffectOccurred(const WorldTransform& world, int spawnNum);

	struct EffectData {
		WorldTransform world;
		Vector3 velocity_;
		Vector3 accce_ = {0,-0.1f,0};
		float BulletForce=0.4f;
		const int maxDeadCount = 480;
		int deadCount = 0;
	};


	std::vector<std::unique_ptr<EffectData>>effectDatas_;

};