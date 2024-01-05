#include "Player.h"

#include"TextureManager/TextureManager.h"
#include<imgui.h>
#include<json.hpp>
#include<cassert>
#include<fstream>

ATKData LoadATKData(nlohmann::json::iterator& itGroup) {
	ATKData data;

	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		//アイテム名
		const std::string& itemName = itItem.key();

		//各データ
		if (itemName == "data") {
			data.extraTime = (float)itItem->at(0);
			data.AttackTime = (float)itItem->at(1);
			data.RigorTime = (float)itItem->at(2);
			data.isYATK = (int)itItem->at(3);
		}
		else if (itemName == "ATKDerivation") {
			//皇族データ保存
			data.ATKDerivation.push_back(LoadATKData(itItem));
		}
		else if (itemName == "ATKDerivation2") {
			data.ATKDerivation.push_back(LoadATKData(itItem));
		}
	}

	return data;
}

void Player::LoadATKDatas() {

	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(atkDataPass_);

	//読み込み失敗
	if (ifs.fail()) {
		assert(false);
		return ;
	}

	nlohmann::json root;
	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//移したのでファイルを閉じる
	ifs.close();


	

	int Index = 0;
	//攻撃グループ複数読み込み
	for (auto& groupname : groupNames_) {

		//グループ検索
		nlohmann::json::iterator itGroup = root.find(groupname);

		//未登録チェック
		assert(itGroup != root.end());

		
		startATKData_[Index]=LoadATKData(itGroup);

		

		Index++;
	}
}



Player::Player() {
	//一回しかしない初期化情報
	input_ = Input::GetInstance();

	input_->SetDeadLine(0.3f);

	//攻撃データの初期化
	LoadATKDatas();

	//GameObject::Initialize("player");

	int Index = 0;
	for (auto& tag : tags) {
		models[Index].reset(Model::CreateFromOBJ(tag));
		Index++;
	}

	textureData = TextureManager::LoadTex("resources/Object/player.png");

	

	mWorlds[HEAD].SetParent(&world_);
	mWorlds[LARM].SetParent(&mWorlds[HEAD]);
	mWorlds[RARM].SetParent(&mWorlds[HEAD]);
	mWorlds[LFOOT].SetParent(&mWorlds[HEAD]);
	mWorlds[RFOOT].SetParent(&mWorlds[HEAD]);

}

Player::~Player() {
	
}

void Player::Initialize() {
	world_.translate_.y = 1.5f;

	mWorlds[LFOOT].translate_ = { -0.5f,-0.8f,0 };
	mWorlds[RFOOT].translate_ = { 0.5f,-0.8f,0 };

}

void Player::Update() {

	
	//状態の初期化処理
	if (stateRequest_) {
		state_ = stateRequest_.value();
		stateRequest_ = std::nullopt;

		//実際の初期化処理
		(this->*StateInitialize[(int)state_])();
	}

	//状態の更新
	(this->*StateUpdate[(int)state_])();

	//行列更新
	
	world_.UpdateMatrix();

	int Index = 0;
	for (auto& world : mWorlds) {
		world.UpdateMatrix();
	}


}

void (Player::* Player::StateInitialize[])() = {
	&Player::InitializeMove,
	&Player::InitializeATK,
	&Player::InitializeHitAction,
	&Player::InitializeSpecialATK
};

void (Player::* Player::StateUpdate[])() = {
	&Player::UpdateMove,
	&Player::UpdateATK,
	&Player::UpdateHitAction,
	&Player::InitializeSpecialATK
};

void Player::Draw(const Matrix4x4& viewprojection) {
	//IGameObject::Draw(viewprojection);
	//model_->Draw(world_.matWorld_, viewprojection, TextureManager::uvChecker_);

	
	//各モデル描画
	int Index = 0;
	for (auto& model : models) {
		model->Draw(mWorlds[Index].matWorld_, viewprojection, textureData);

		Index++;
	}
}

void Player::DebugWindow(const char* name) {
	ImGui::Begin(name);
	world_.DrawDebug(name);

	mWorlds[HEAD].DrawDebug("head");
	mWorlds[LARM].DrawDebug("LA");
	mWorlds[RARM].DrawDebug("RA");
	mWorlds[LFOOT].DrawDebug("LF");
	mWorlds[RFOOT].DrawDebug("RF");

	
	//model_->DebugParameter(name);
	ImGui::End();
}



void Player::Move() {

	Vector3 move{};

	//移動取得
	move = input_->GetWASD();

	if (input_->IsControllerActive()) {
		move = input_->GetjoyStickLV3();
	}

	move.SetNormalize();
	move *= spd_;

	//カメラ方向に向ける
	move = TransformNormal(move, camera_->GetMainCamera().matWorld_);

	move.y = 0;

	if (move != Vector3(0, 0, 0)) {
		world_.rotate_.y = GetYRotate({ move.x,move.z });
	}
	//加算
	world_.translate_ += move;


}

#pragma region 各状態初期化処理


void Player::InitializeMove() {

}

void Player::InitializeATK() {

	atkState_ = ATKState::Extra;

	updateATKData_ = ATKUpdateData{};

	//どちらのボタンが最初かで攻撃ツリー変更
	if (isPressY) {
		ATKData_ = startATKData_[0];
	}
	else {
		ATKData_ = startATKData_[0];
	}

}

void Player::InitializeHitAction() {

}

void Player::InitializeSpecialATK() {
}

void Player::UpdateSpecialATK() {
}

#pragma endregion

#pragma region 各状態更新処理

void Player::UpdateMove() {
	//移動処理
	Move();

	//攻撃状態に移るかの処理
	bool isATK = false;

	//スペシャル攻撃
	bool isSpecialATK = false;


	if (isATK = input_->TriggerKey(DIK_Z)) {
		isPressY = true;
	}
	else if (isATK = input_->TriggerKey(DIK_X)) {
		isPressY = false;
	}
	else {
		isSpecialATK = input_->TriggerKey(DIK_C);
	}

	//コントローラーがあるときの処理
	if (input_->IsControllerActive()) {
		if (isATK = input_->IsTriggerButton(kButtonY)) {
			isPressY = true;
		}
		else if (isATK = input_->IsTriggerButton(kButtonB)) {
			isPressY = false;
		}
		else {
			isSpecialATK = input_->IsTriggerButton(kButtonX);
		}		
	}

	if (isATK) {
		stateRequest_ = State::ATK;
	}

}

void Player::UpdateATK() {


#pragma region 実行処理
	if (atkState_ == ATKState::Extra) {

		//予備動作中向き変更
		Vector3 move{};
		//データ
		move = input_->GetWASD();
		if (input_->IsControllerActive()) {
			move = input_->GetjoyStickLV3();
		}

		move.SetNormalize();
		//カメラ方向に向ける
		move = TransformNormal(move, camera_->GetMainCamera().matWorld_);
		move.y = 0;

		if (move != Vector3(0, 0, 0)) {
			world_.rotate_.y = GetYRotate({ move.x,move.z });
		}

		updateATKData_.count++;
		//条件を満たしたら次の状態へ
		if (updateATKData_.count >= ATKData_.extraTime) {
			atkState_ = ATKState::ATK;
			updateATKData_.count=0;

		}
	}

	if (atkState_ == ATKState::ATK) {
		updateATKData_.count++;
		//条件を満たしたら次の状態へ
		if (updateATKData_.count >= ATKData_.AttackTime) {
			atkState_ = ATKState::Rigor;
			updateATKData_.count=0;

		}
	}

	if (atkState_ == ATKState::Rigor) {
		updateATKData_.count++;
		//条件を満たしたら次の状態へ
		if (updateATKData_.count >= ATKData_.RigorTime) {
#pragma region 条件によるシーン転換


			//攻撃入力フラグON
			if (updateATKData_.nextATK && ATKData_.ATKDerivation.size() != 0) {
				
				for (auto& ATKdata : ATKData_.ATKDerivation) {
					if (updateATKData_.isPushY&&ATKdata.isYATK) {
						//新しいデータ代入
						ATKData_ = ATKdata;
						updateATKData_ = ATKUpdateData{};
					}
					else if (!updateATKData_.isPushY && !ATKdata.isYATK) {
						ATKData_ = ATKdata;
						updateATKData_ = ATKUpdateData{};
					}
				}

			}
			else {
				//移動状態に変更
				stateRequest_ = State::Move;
			}
#pragma endregion

		}
	}
#pragma endregion

#pragma region 実行中のキー入力受付
	if (updateATKData_.nextATK = input_->IsTriggerButton(kButtonY)) {
		updateATKData_.isPushY = true;
	}else if (updateATKData_.nextATK = input_->IsTriggerButton(kButtonB)) {
		updateATKData_.isPushY = false;
	}

#pragma endregion




}

void Player::UpdateHitAction() {

}

#pragma endregion

