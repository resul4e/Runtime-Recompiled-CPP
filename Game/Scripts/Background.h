#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include "Core/Object.h"
#include "Transform/Vector.h"

class Background : public Object
{
public:
	Background();
	~Background();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;
	void CreateHorizontalStaircase(int s, int x, int y);
	PhysicsCompHandle GetPipe(size_t index);

	std::vector<SpriteCompHandle> background;
	std::vector<glm::vec4> spriteSheet;
	std::vector<glm::vec2> offset;

	//physics
	const glm::vec2 floor1Size = { 11.04f, 0.5f };
	const glm::vec2 floor2Size = { 2.41f, 0.5f };
	const glm::vec2 floor3Size = { 10.24f, 0.5f };
	const glm::vec2 floor4Size = { 9.12f, 0.5f };

	glm::vec2 floor1Pos = {11.04f, 4.5f};
	glm::vec2 floor2Pos = {25.11f, 4.5f};
	glm::vec2 floor3Pos = { 38.72f, 4.5f };
	glm::vec2 floor4Pos = { 58.72f, 4.5f };

	std::vector<PhysicsCompHandle> floors;
	std::vector<PhysicsCompHandle> pipes;
	std::vector<PhysicsCompHandle> pyramids;
	//PhysicsCompHandle floor2;
	//PhysicsCompHandle floor3;
	//PhysicsCompHandle floor4;
};

INTERFACE_START(Background)
	PUBLIC_FUNCTION(GetPipe, Background)
INTERFACE_END()