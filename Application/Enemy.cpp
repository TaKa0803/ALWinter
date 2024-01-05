#include "Enemy.h"
#include"RandomNum/RandomNum.h"
#include"TextureManager/TextureManager.h"

void Enemy::Initialize(const Vector3&position,const WorldTransform*playerWorld) {
	InstancingGameObject::Initialize("player");
	
	
	//model_->IsEnableTexture(false);
	//model_->SetBlendMode(BlendMode::kNone);
	world_.translate_ = position;

	playerWorld_ = playerWorld;
	//idou
	moveSPD_=RandomNumber::Get(minSPD_, maxSPD_);

	mWorlds[HEAD].SetParent(&world_);
	mWorlds[LARM].SetParent(&mWorlds[HEAD]);
	mWorlds[RARM].SetParent(&mWorlds[HEAD]);
	mWorlds[LFOOT].SetParent(&mWorlds[HEAD]);
	mWorlds[RFOOT].SetParent(&mWorlds[HEAD]);

	mWorlds[LFOOT].translate_ = { -0.5f,-0.8f,0 };
	mWorlds[RFOOT].translate_ = { 0.5f,-0.8f,0 };

}

void Enemy::Update() {

	//pk
	Vector3 p_eVelo = playerWorld_->GetMatWorldTranslate() - world_.GetMatWorldTranslate();

	float p_eLength = Length(playerWorld_->GetMatWorldTranslate() - world_.GetMatWorldTranslate());

	//hanigai
	if (p_eLength > stopRange_) {

		//プレイヤーの方向に移動
		Vector3 moveVelo{};
		moveVelo = p_eVelo;
		//ノーマライズ
		moveVelo.SetNormalize();
		//移動領分書ける
		moveVelo *= moveSPD_;
		//割る
		moveVelo /= maxSPDFrame;
		//速度に追加
		velocity_ += moveVelo;
		//速度ベクトルの量を取得
		float veloSPD = velocity_.GetLength();
		//プレイヤーへの向きベクトルに書ける
		velocity_ = p_eVelo.SetNormalize() * veloSPD;

		//最大速度に達していたら移動量もどす
		float spd = Length(velocity_);
		if (spd > moveSPD_) {
			velocity_.SetNormalize();
			velocity_ *= moveSPD_;
		}

		//Yいらない
		velocity_.y = 0;

		//加算処理
		world_.translate_ += velocity_;

		//muki
		if (moveVelo != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ moveVelo.x,moveVelo.z });
		}

	}
	else {
		velocity_.SetZero();
	}


	//InstancingGameObject::Update();
	world_.UpdateMatrix();

	int Index = 0;
	for (auto& world : mWorlds) {

		world.UpdateMatrix();

		IMM_->SetWorld(tags[Index], world);

		Index++;
	}
}

void Enemy::Draw(const Matrix4x4& view) {
}
