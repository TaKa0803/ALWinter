#pragma once
#include "IScene/IScene.h"

#include<iostream>
#include<list>

#include"Input/Input.h"
#include"Camera/Camera.h"
#include"Player.h"
#include"Plane.h"
#include"Enemy.h"
#include"EnemyPopManager.h"
#include"Sprite/Sprite.h"


class GameScene : public IScene {

public:

	GameScene();

	~GameScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:
	void DebugWindows();

	void Collision();

	void SceneChange();

	void UIUpdateDraw();

	void ClearUIUpdate();

	void LimitUI();
private:
	//キー入力
	Input* input_ = nullptr;
	//カメラクラス
	std::unique_ptr<Camera> camera_;

	float xrotateNum = 0.05f;
	float yrotatenum = 0.02f;

	///以下ゲーム実装
	std::unique_ptr<Player>player_;

	std::unique_ptr<Plane>plane_;

	std::list<std::unique_ptr<Enemy>>enemies_;

	std::unique_ptr<EnemyPopManager>enemyPopManager_;

	int limitMinute = 0;
	const int maxmilitMinute = 60 * 60;

	enum Scene {
		Game,
		Clear,
	};

	Scene scene_ = Game;

	std::unique_ptr<Sprite>skillSp_;

	std::unique_ptr<Sprite>BButton_;

	std::unique_ptr<Sprite>punchSp_;

	std::unique_ptr<Sprite>kickSp_;

	std::unique_ptr<Sprite>ultSp_;

	Vector3 waitATKpos = { 1000,622,0 };
	Vector3 waitATKscale = { 90,90 };

	Vector3 ATKpos = {1085,560};
	Vector3 ATKscale = { 200,200 };

	enum ButtonState {
		Wait,
		Punch,
		Kick,
		Ult
	};

	ButtonState BState_ = Wait;


	std::unique_ptr<Sprite>num1_;
	std::unique_ptr<Sprite>num10_;


	std::unique_ptr<Sprite>num100_;

	std::unique_ptr<Sprite>backScreen_;
	std::unique_ptr<Sprite>resultText_;

	float alphaNum_ = 0;

	const float addAlphaNum_ = 120.0f;

	const float screenmaxAlphaNum_ = 0.7f;


	//1フレームごとに計算する敵の数
	const float frameCountEnemy_ = 1.0f;

	float nowFrameCountEnemy_ = 0;

	bool serchComplete_ = false;
};