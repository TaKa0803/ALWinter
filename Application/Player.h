#pragma once
#include"IGameObject/GameObject.h"
#include"Input/Input.h"
#include"Camera/Camera.h"

#include<vector>


//攻撃のデータ構造体
struct ATKData {
	float extraTime = 0;	//予備動作
	float AttackTime = 0;	//攻撃動作
	float RigorTime = 0;	//硬直時間

	int isYATK = false;	//Yボタン攻撃か

	std::vector<ATKData>ATKDerivation;	//攻撃の派生
};

class Player :public GameObject {

public:
	Player();
	~Player();

	void Initialize();

	void Update();

	void Draw(const Matrix4x4&viewprojection);

	void DebugWindow(const char*name);

	void SetCamera(Camera* camera) { camera_ = camera; }

private://メンバ関数


	//移動
	void Move();


	
#pragma region 状態管理とメンバ関数ポインタテーブル

	enum class State {
		Move,		//移動
		ATK,		//攻撃
		HITACTION,	//被攻撃時
		SPECIALATK,	//特別攻撃
		kNumStates	//状態の数
	};

	//プレイヤーの状態
	State state_=State::Move;
	//状態リクエスト
	std::optional<State>stateRequest_=std::nullopt;

	//状態ごとの初期化テーブル
	static void (Player::* StateInitialize[])();

	//状態ごとの更新テーブル
	static void (Player::* StateUpdate[])();


	void InitializeMove();

	void InitializeATK();
	
	void InitializeHitAction();

	void InitializeSpecialATK();

	void UpdateMove();

	void UpdateATK();
	
	void UpdateHitAction();

	void UpdateSpecialATK();
#pragma endregion

	void LoadATKDatas();

private:
	Input* input_ = nullptr;

	const Camera* camera_=nullptr;

#pragma region モデルに関する
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

	std::unique_ptr<Model>models[modelNum_];

	int textureData = 0;
#pragma endregion





	//移動速度
	float spd_ = 1.0f;




#pragma region 攻撃に関する変数
	


	//攻撃に関するデータ
	std::string atkDataPass_ = "resources/PlayerATKData.json";

	static const uint32_t groupNum_ = 2;

	//グループ名
	std::string groupNames_[groupNum_] = { "YButtonATK","BButtonATK"};

	static const int itemNum = 5;

	//アイテムの名前
	std::string keyNames_[itemNum] = {
		"data",
		"ATKDerivation"
	};

	
	//ボタンを押したときからの攻撃一覧
	ATKData startATKData_[groupNum_];


	//Yを押したか否か（攻撃分岐に利用
	bool isPressY=false;

	//攻撃ボタン数
	const int atkButtonNum_ = 2;

	//最大コンボ数
	const int maxATKConBo = 3;

	int ATKConboCount = 0;

	enum class ATKState {
		Extra,//準備
		ATK,	//攻撃実行
		Rigor//硬直
	};
	//攻撃の状態enum
	ATKState atkState_;

	//攻撃時に使うデータまとめ
	struct ATKUpdateData {
		//次の攻撃をするか
		bool nextATK = false;
		bool isPushY = false;
		int count=0;
	};

	//攻撃の更新データ
	ATKUpdateData updateATKData_;

	//実行する攻撃動作
	ATKData ATKData_;

	
	
#pragma endregion


};