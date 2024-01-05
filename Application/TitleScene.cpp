#include "TitleScene.h"

TitleScene::TitleScene() {
	input_=Input::GetInstance();
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() {
}

void TitleScene::Update() {



	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = STAGE;
	}

	if (input_->IsControllerActive() && input_->IsPushSomeButtons()) {
		sceneNo = STAGE;
	}
}

void TitleScene::Draw() {
}
