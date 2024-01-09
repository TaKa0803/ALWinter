#pragma once
#include "IScene/IScene.h"
#include"Input/Input.h"
#include"Sprite/Sprite.h"

class TitleScene : public IScene {

public:

	TitleScene();

	~TitleScene();

	void Initialize() override;

	void Update() override;

	void Draw() override;

private:

	void Debug();

private:
	//キー入力
	Input* input_=nullptr;

	std::unique_ptr<Sprite> sprite_;

	std::unique_ptr<Sprite>pressSp_;

	std::unique_ptr<Sprite>BButtonSp_;
};

