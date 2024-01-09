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


	texture = TextureManager::LoadTex("resources/UI/skill.png");
	skillSp_.reset(Sprite::Create(texture, { 90,90 }, { 90,90 }, { 90,90 }, { 1000,600 }));



	texture = TextureManager::LoadTex("resources/UI/B.png");
	BButton_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 64,64 }, { 1045, 650 }));


	texture = TextureManager::LoadTex("resources/UI/punch.png");
	punchSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));

	texture = TextureManager::LoadTex("resources/UI/kick.png");
	kickSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));


	texture = TextureManager::LoadTex("resources/UI/ult.png");
	ultSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 90,90 }, { 1070,650 }));


	texture = TextureManager::LoadTex("resources/UI/number64x90.png");
	num1_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 680,90 }));
	num10_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 610,90 }));
	num100_.reset(Sprite::Create(texture, { 640,90 }, { 64,90 }, { 90,90 }, { 540,90 }));


	texture = TextureManager::LoadTex("resources/UI/black.png");
	backScreen_.reset(Sprite::Create(texture, { 64,64 }, { 64,64 }, { 1280,720 }));

	texture = TextureManager::LoadTex("resources/UI/result.png");
	resultText_.reset(Sprite::Create(texture, { 320,90 }, { 320,90 }, { 830,290 }));
}

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	sceneNo = STAGE;
	scene_ = Game;
	limitMinute = maxmilitMinute;

	player_->Initialize();
	plane_->Initialize();

	//初期化
	camera_->Initialize();
	//各種設定
	camera_->SetTarget(&player_->GetWorld());
	camera_->SetCameraDirection(-40.0f);
	player_->SetCamera(camera_.get());

	enemyPopManager_->Initialzie();
	enemyPopManager_->SetPlayerWorld(&player_->GetWorld());

	enemies_.clear();

	backScreen_->SetColorAlpha(0);
	resultText_->SetColorAlpha(0);

	alphaNum_ = 0;
	nowFrameCountEnemy_ = 0;

	num1_->SetPosition({ 680,90 });
	num1_->SetScale({ 90,90 });

	num10_->SetPosition({ 610,90 });
	num10_->SetScale({ 90,90 });

	num100_->SetPosition({ 540,90 });
	num100_->SetScale({ 90,90 });

	serchComplete_ = false;

}

void GameScene::Update() {



	switch (scene_) {
	case GameScene::Game:

#pragma region ゲームシーン
		//デバッグウィンドウ表示
		DebugWindows();

		//プレイヤー更新
		player_->Update();

		//カメラ更新
		Vector2 stick;
		if (input_->IsControllerActive()) {
			stick = input_->GetjoyStickR();
		}
		else {
			Vector3 sti = input_->GetAllArrowKey();
			stick = { sti.x,sti.z };
		}
		stick.Normalize();
		stick.x *= xrotateNum;
		stick.y *= yrotatenum * -1.0f;
		camera_->AddCameraR_X(stick.y);
		camera_->AddCameraR_Y(stick.x);


		camera_->Update();

		//
		enemyPopManager_->Update();

		//敵の生成処理
		if (std::unique_ptr<Enemy>newEnemy = enemyPopManager_->PopEnemy()) {
			enemies_.push_back(std::move(newEnemy));
		}

		for (auto& enemy : enemies_) {
			if (!enemy->GetDead()) {
				enemy->Update();
			}
		}

		Collision();

		LimitUI();
#pragma endregion

		break;
	case GameScene::Clear:

		num1_->DrawDebugImGui("num1");
		num10_->DrawDebugImGui("num10");
		num100_->DrawDebugImGui("num100");
		resultText_->DrawDebugImGui("taosita");


		break;
	default:
		break;
	}


	SceneChange();

}

void GameScene::Draw() {
	//地面
	plane_->Draw(camera_->GetViewProjectionMatrix());
	//敵の旗
	enemyPopManager_->Draw();

	//敵
	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Draw();
		}
	}
	//プレイヤー
	player_->Draw(camera_->GetViewProjectionMatrix());

	//インスタンシングのモデルを全描画
	InstancingModelManager::GetInstance()->DrawAllModel(camera_->GetViewProjectionMatrix());

	switch (scene_) {
	case GameScene::Game:

		UIUpdateDraw();
		break;
	case GameScene::Clear:

		ClearUIUpdate();
		break;
	default:
		break;
	}




}

void GameScene::DebugWindows() {
	//カメラのデバッグ表示
	camera_->DrawDebugWindow("camera");

	//プレイヤーデバッグ表示
	player_->DebugWindow("player");

	plane_->DebagWindow();

	//skillSp_->DrawDebugImGui("skill");
	//BButton_->DrawDebugImGui("BButton");
	//punchSp_->DrawDebugImGui("punch");


}

void GameScene::Collision() {

	for (auto& enemy : enemies_) {
		if (!enemy->GetDead()) {
			enemy->Collision(*player_->GetCollider());
		}
	}

}

void GameScene::SceneChange() {

	switch (scene_) {
	case GameScene::Game:
		if (limitMinute-- <= 0) {
			scene_ = Clear;
		}

		//デバッグ用シーンチェンジ
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_P)) {
			scene_ = Clear;
		}
#endif // _DEBUG


		break;
	case GameScene::Clear:
		if (input_->TriggerKey(DIK_SPACE)||input_->IsTriggerButton(kButtonA)) {
			sceneNo = TITLE;
		}
		break;
	default:
		break;
	}
}

void GameScene::UIUpdateDraw() {

	if (player_->GetConboCount() == 0) {
		BState_ = Wait;
	}
	if (player_->GetConboCount() == 1) {
		BState_ = Punch;
	}if (player_->GetConboCount() == 2) {
		BState_ = Kick;
	}
	if (player_->GetConboCount() == 3) {
		BState_ = Ult;
	}

	switch (BState_) {
	case GameScene::Wait:
		skillSp_->SetPosition(ATKpos);
		skillSp_->SetScale(ATKscale);

		BButton_->SetPosition({ 1155,635 });

		punchSp_->SetTVTranslate({ 0.5f,0 });
		punchSp_->SetPosition(waitATKpos);
		punchSp_->SetScale(waitATKscale);


		skillSp_->Draw();

		punchSp_->Draw();


		break;
	case GameScene::Punch:

		punchSp_->SetTVTranslate({ 0.0f,0.0f });
		punchSp_->SetPosition(ATKpos);
		punchSp_->SetScale(ATKscale);

		kickSp_->SetTVTranslate({ 0.5f,0.0f });
		kickSp_->SetPosition(waitATKpos);
		kickSp_->SetScale(waitATKscale);

		punchSp_->Draw();

		kickSp_->Draw();

		break;
	case GameScene::Kick:
		kickSp_->SetTVTranslate({ 0.0f,0.0f });
		kickSp_->SetPosition(ATKpos);
		kickSp_->SetScale(ATKscale);

		ultSp_->SetTVTranslate({ 0.5f,0.0f });
		ultSp_->SetPosition(waitATKpos);
		ultSp_->SetScale(waitATKscale);

		kickSp_->Draw();

		ultSp_->Draw();


		break;
	case GameScene::Ult:
		ultSp_->SetTVTranslate({ 0.0f,0.0f });
		ultSp_->SetPosition(ATKpos);
		ultSp_->SetScale(ATKscale);


		ultSp_->Draw();



		break;
	default:
		break;
	}

	BButton_->Draw();



	num1_->Draw();
	num10_->Draw();

}

void GameScene::ClearUIUpdate() {


	//数字検索
#pragma region 数計算
	if (!serchComplete_) {

		num1_->SetPosition({ 368,285 });
		num1_->SetScale({ 160,160 });

		num10_->SetPosition({ 238,285 });
		num10_->SetScale({ 160,160 });
		
		num100_->SetPosition({ 114,285 });
		num100_->SetScale({ 160,160 });

		resultText_->SetPosition({ 830,290 });

		nowFrameCountEnemy_ += frameCountEnemy_;

		serchComplete_ = true;
		int Count = 0;
		for (auto& enemy : enemies_) {
			if (enemy->GetDead()) {
				Count++;

				if (Count >= nowFrameCountEnemy_) {
					serchComplete_ = false;
					break;
				}
			}
		}


		int num1 = Count % 10;


		//一桁目決定

		num1_->SetTVTranslate({ ((float)num1 / 10.0f) - 0.1f ,0 });

		int count2 = (int)(Count - num1) / 10;

		int num2 = count2 % 10;

		if (num2 != 0.0f) {

			num10_->SetTVTranslate({ ((float)num2 / 10.0f) - 0.1f ,0 });


			int count3 = (int)(Count - num1 - num2) / 100;

			int num3 = count3 % 10;

			num100_->SetTVTranslate({ ((float)num3 / 10.0f) - 0.1f ,0 });

		}
		else {
			num100_->SetTVTranslate({ 1.0f, 0 });
			num10_->SetTVTranslate({ 1.0f, 0 });
		}


		if (serchComplete_ == true) {
			int num = 0;
		}
	}
#pragma endregion





	if (alphaNum_ < addAlphaNum_) {
		alphaNum_++;

		float screenAlpha = alphaNum_ / addAlphaNum_;
		if (screenAlpha > screenmaxAlphaNum_) {
			screenAlpha = screenmaxAlphaNum_;
		}
		backScreen_->SetColorAlpha(screenAlpha);


		float resultAlpha = alphaNum_ / addAlphaNum_;
		resultText_->SetColorAlpha(resultAlpha);

	}
	else {
		resultText_->SetColorAlpha(1.0f);

	}


	backScreen_->Draw();
	resultText_->Draw();

	num1_->Draw();
	num10_->Draw();
	num100_->Draw();

}

void GameScene::LimitUI() {

	//分に変換
	float minute = (float)limitMinute / 60.0f;

	int second = (int)minute % 10;

	num1_->SetTVTranslate({ (float)second / 10.0f, 0 });

	//二桁目のみの情報取得
	minute -= second;

	minute /= 100;

	minute -= 0.1f;

	num10_->SetTVTranslate({ float(minute),0 });
}




