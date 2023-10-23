#include "Entity.h"
#include "Component/ModelComp.h"


void Entity::Init() {
	isActive_ = true;
	transform_.UpdateMatrix();
}

void Entity::Reset() {
	for (auto &component : componentMap_) {
		component.second->Reset();
	}
}

void Entity::Update(float deltaTime) {
	auto *const modelComp = GetComponent<ModelComp>();
	for (auto &component : componentMap_) {
		if (modelComp != component.second.get())
			component.second->Update(deltaTime);
	}
	transform_.UpdateMatrix();
	if (modelComp)
		modelComp->Update(deltaTime);
}

void Entity::Draw(const Camera<Render::CameraType::Projecction> &vp) const {
	for (auto &component : componentMap_) {
		component.second->Draw(vp);
	}
}

void Entity::Draw2D() const {
	for (auto &component : componentMap_) {
		component.second->Draw2D();
	}
}

void Entity::Destroy() {
	for (auto &component : componentMap_) {
		component.second->Destroy();
	}
}


const Vector3 &Entity::GetWorldPos() {
	transform_.CalcMatrix();
	return *reinterpret_cast<Vector3 *>(transform_.matWorld_.m[3]);
}

void Entity::OnCollision(Entity *const other) {
	for (auto &component : componentMap_) {
		component.second->OnCollision(other);
	}
}

void Entity::ImGuiWidget() {
	transform_.ImGuiWidget();
	for (auto &component : componentMap_) {
		component.second->ImGuiWidget();
	}
}