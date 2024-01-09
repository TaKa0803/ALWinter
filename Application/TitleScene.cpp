#include "TitleScene.h"
#include"TextureManager/TextureManager.h"
#include<imgui.h>

TitleScene::TitleScene() {
	input_=Input::GetInstance();

	int texture = TextureManager::LoadTex("resources/Title/Title.png");
	sprite_.reset(Sprite::Create(texture,{320,180},{320,180},{1280,720}));

	texture = TextureManager::LoadTex("resources/Title/Press.png");
	pressSp_.reset(Sprite::Create(texture, { 160,90 }, { 160,90 }, { 100,70 }));
	pressSp_->SetPosition({570,500});
	pressSp_->SetScale({170,100});

	texture = TextureManager::LoadTex("resources/UI/B.png");
	BButtonSp_.reset(Sprite::Create(texture, { 180,90 }, { 90,90 }, { 70,70 }));
	BButtonSp_->SetPosition({ 750,500 });
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() {
}

void TitleScene::Update() {

	
	Debug();

	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = STAGE;
	}

	if (input_->IsControllerActive() && input_->IsTriggerButton(kButtonB)) {
		sceneNo = STAGE;
	}
}

void TitleScene::Draw() {
	sprite_->Draw();

	pressSp_->Draw();

	BButtonSp_->Draw();
}

void TitleScene::Debug() {

	Vector3 pos = pressSp_->GetPosition();
	Vector3 scale = pressSp_->GetScale();

#ifdef _DEBUG
	ImGui::Begin("sprite");
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("scale", &scale.x);
	ImGui::End();
#endif // _DEBUG

	pressSp_->SetPosition(pos);
	pressSp_->SetScale(scale);

}
