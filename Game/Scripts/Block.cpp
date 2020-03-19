#include "Block.h"
#include "Rendering/SpriteComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/AnimationComponent.h"
#include "Core/Level.h"

SCRIPT_START(Block)

void Block::Start()
{

}

void Block::Restart()
{
}

void Block::SetColliderIndex(size_t index)
{
	floorColliderIndex = index;
}

size_t Block::GetColliderIndex()
{
	return floorColliderIndex;
}

void Block::SetBlockIndex(size_t index)
{
	blockIndex = index;
}

size_t Block::GetBlockIndex()
{
	return blockIndex;
}

glm::vec2 Block::GetPosition()
{
	return position;
}

void Block::FixedUpdate()
{
}

void Block::Update(float aDeltatime)
{
}

void Block::Delete()
{
}

void Block::Serialize(Storage& storage)
{
}

void Block::HitBlock()
{
	AnimationComponent::SetPosition(block, glm::vec2(512, 256));
}

PhysicsCompHandle Block::GetPhysicsComponent()
{
	return blockCollider;
}

SCRIPT_END(Block)
