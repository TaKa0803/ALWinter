#include "GameScene.h"
#include<imgui.h>

#include"InstancingModelManager/InstancingModelManager.h"
#include"TextureManager/TextureManager.h"

GameScene::GameScene() {
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	player_ = std::make_unique<Player>();

	plane_ = std::make_unique<Plane>();
	
	enemyPopManager_ = std::make_unique<EnemyPopManager>();
	enemyPopManager_->LoadPopdata();

	
	//インスタンシングの複数モデルが同じ画像利用の時の対応
	InstancingModelManager* IMM = InstancingModelManager::GetInstance();
	std::string eModelparts[5] = {
		"Head",
		"LArm",
		"RArm",
		"LFoot",
		"RFoot"
	};
	int texture = TextureManager::LoadTex("resources/Object/enemy.png");
	for (auto& tag : eModelparts) {
		IMM->SetTexture(tag, texture);
	}

	//インスタンシングじゃないモデル
	player_->Initialize();
	plane_->Initialize();


	//初期化
	camera_->Initialize();

}

GameScene::~GameScene() {

}

void GameScene::Initialize() {
	
	//各種設定
	camera_->SetTarget(&player_->GetWorld());
	//camera_->SetMainCameraPos({ 0,0,18.0f });
	camera_->SetCameraDirection(-40.0f);
	player_->SetCamera(camera_.get());
	enemyPopManager_->SetPlayerWorld(&player_->GetWorld());

	
}

void GameScene::Update() {
	//デバッグウィンドウ表示
	DebugWindows();

	//プレイヤー更新
	player_->Update();

	//カメラ更新
	if (input_->IsControllerActive()) {
		Vector2 stick = input_->GetjoyStickR();
		stick.Normalize();
		stick.x *= xrotateNum;
		stick.y *= yrotatenum*-1.0f;
		camera_->AddCameraR_X(stick.y);
		camera_->AddCameraR_Y(stick.x);
	}
	camera_->Update();
	
	//
	enemyPopManager_->Update();

	//敵の生成処理
	if (std::unique_ptr<Enemy>newEnemy = enemyPopManager_->PopEnemy()) {
		enemies_.push_back(std::move(newEnemy));
	}

	for (auto& enemy : enemies_) {
		enemy->Update();
	}

	
}

void GameScene::Draw() {
	plane_->Draw(camera_->GetViewProjectionMatrix());
	
	for (auto& enemy : enemies_) {
		enemy->Draw(camera_->GetViewProjectionMatrix());
	}

	player_->Draw(camera_->GetViewProjectionMatrix());

	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());
}

void GameScene::DebugWindows() {
	//カメラのデバッグ表示
	camera_->DrawDebugWindow("camera");

	//プレイヤーデバッグ表示
	player_->DebugWindow("player");
	
	plane_->DebagWindow();
}
