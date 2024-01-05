#pragma once
#include"Instancing/InstancingGameObject.h"

class Enemy : public InstancingGameObject {

public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(const Vector3& position, const WorldTransform* playerWorld);

	/// <summary>
	/// 
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	void Draw(const Matrix4x4& view);

private:
	//プレイヤーのワールド
	const WorldTransform* playerWorld_;

#pragma region モデル関係
	//モデルの数
	static const int modelNum_ = 5;

	//タグ軍
	std::string tags[modelNum_] = {
		"Head",
		"LArm",
		"RArm",
		"LFoot",
		"RFoot"
	};

	enum Parts {
		HEAD,
		LARM,
		RARM,
		LFOOT,
		RFOOT
	};

	WorldTransform mWorlds[modelNum_] = {};

#pragma endregion



	//移動速度
	Vector3 velocity_{};

	//最大速度になるまでのF数
	float maxSPDFrame = 60.0f;

	//個体差用
	const float maxSPD_ = 0.5f;
	const float minSPD_ = 0.3f;

	//移動速度
	float moveSPD_ = 0.5f;


	//ugokanakunarukyori
	float stopRange_ =10.0f;
};
