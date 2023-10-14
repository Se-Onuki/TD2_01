#include "titleLogo.h"

void TitleLogo::OnEnter() {
	sprite_.reset(Sprite::Create(TextureManager::Load("uvChecker.png")));
	sprite_->SetScale({ 540.f,540.f });
	sprite_->SetPivot({ 0.5f, 0.5f });

}

void TitleLogo::Update() {
	ImGui::Begin("Logo");
	sprite_->ImGuiWidget();
	ImGui::End();

}

void TitleLogo::Draw() {
	sprite_->Draw();
}
