#pragma once

#include <memory>
#include "../../Engine/DirectBase/2D/Sprite.h"

class Map {
public:
	Map() = default;
	~Map() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	const Vector2 kMapCentor_ = Vector2{ 1280.f, 720.f } / 2.f;

private:

	std::unique_ptr<Sprite> pizza_ = nullptr;

};
