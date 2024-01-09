#pragma once
#include"Instancing/InstancingGameObject.h"

class SphereCollider : InstancingGameObject {

public:

	void Initialize(const WorldTransform& world);

	void Update();

	void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sphere">基の円</param>
	/// <param name="backVec">基の円の戻る量</param>
	/// <returns></returns>
	bool IsHit(const SphereCollider& sphere, Vector3 backVec = Vector3{ 0,0,0 });


	void SetScale(float wide) { wide_ = wide; }

	void SetTranslate(const Vector3& pos) { world_.translate_ = pos; }

	float GetScale() { return wide_; }

	Vector3 GetTranslate() { return world_.translate_; }
private:

	float wide_=1;

};