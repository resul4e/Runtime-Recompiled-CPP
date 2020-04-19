#include "BrickBlock.h"
#include "Rendering/SpriteComponent.h"
#include "Physics/PhysicsComponent.h"
#include "Rendering/AnimationComponent.h"
#include "Core/Level.h"
#include "Core/Storage.h"
#include "Audio/AudioComponent.h"
#include "Game.h"

SCRIPT_START(BrickBlock)

void BrickBlock::Start()
{

}

void BrickBlock::Setup(glm::vec2 aPos)
{
	position = aPos;

	//block
	block = AnimationComponent::AddSprite(180);
	AnimationComponent::AddAnimation("BrickIdle", "Assets\\Sprites\\NES - Super Mario Bros - Item and Brick Blocks.png");
	AnimationComponent::AddAnimationFrame("BrickIdle", glm::ivec2(544, 384), glm::ivec2(32, 32));

	AnimationComponent::AddAnimation("BrickHit", "Assets\\Sprites\\NES - Super Mario Bros - Item and Brick Blocks.png");
	AnimationComponent::AddAnimationFrame("BrickHit", glm::ivec2(576, 384), glm::ivec2(32, 32));

	AnimationComponent::SetAnimation(block, "BrickIdle");
	AnimationComponent::SetPosition(block, aPos);
	AnimationComponent::SetOrigin(block, glm::vec2(16, 16));

	PhysicsCompHandle anchor = PhysicsComponent::AddStatic(glm::vec2(aPos.x/100.f, -3.f), glm::vec2(0.16, 0.16));
	blockCollider = PhysicsComponent::AddDynamic((aPos+glm::vec2(16,27))/100.f, glm::vec2(0.10, 0.05f), 0, 0.5f, 1, 0, 0.07f);
	PhysicsComponent::PrismaticJoint(anchor, blockCollider, glm::vec2(0, 1), glm::vec2(-0.1f, 0.00f));

	AnimationComponent::AddAnimation("DestroyedBlock", "Assets\\Sprites\\NES - Super Mario Bros - Item and Brick Blocks.png");
	AnimationComponent::AddAnimationFrame("DestroyedBlock", glm::ivec2(608, 224), glm::ivec2(16, 16));

	bumpSound = AudioComponent::AddSound("Assets\\Audio\\smb\\sound effects\\smb_bump.wav");
	canBeHit = true;
	marioPoweredUp = false;
	destroyed = false;
	removedCollider = false;
	startPos = (aPos + glm::vec2(16, 27)) / 100.f;
}

void BrickBlock::Restart()
{
}

void BrickBlock::FixedUpdate()
{
}

void BrickBlock::Update(float aDeltatime)
{
	if(!destroyed)
	{
		glm::vec2 pos = PhysicsComponent::GetPosition(blockCollider);
		if (block.spriteIndex != 1231234)
		{
			AnimationComponent::SetPosition(block, (pos*100.f) - glm::vec2(0, 11));
		}


		if (pos.y >= startPos.y)
		{
			canBeHit = true;
		}
	}
	else
	{
		if (!removedCollider)
		{
			glm::vec2 pos = PhysicsComponent::GetPosition(blockCollider);
			if(pos.y >= ((position + glm::vec2(16, 27)) / 100.f).y)
			{
				PhysicsComponent::RemovePhysicsBody(blockCollider);
				removedCollider = true;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			AnimationComponent::SetPosition(destroyedBlocks.anim.at(i), PhysicsComponent::GetPosition(destroyedBlocks.collider.at(i))*100.f);
		}
	}
}

void BrickBlock::Delete()
{
}

void BrickBlock::Serialize(Storage& storage)
{
	storage.Serialize(blockCollider, block, bumpSound,startPos, destroyedBlocks.anim,destroyedBlocks.collider, destroyed, removedCollider, position, marioPoweredUp);
}

void BrickBlock::HitBlock()
{
	if (marioPoweredUp && !destroyed)
	{
		bumpSound = AudioComponent::AddSound("..\\Game\\Assets\\Audio\\smb\\sound effects\\smb_breakblock.wav");
		AudioComponent::Play(bumpSound);
		AnimationComponent::RemoveSprite(block);
		destroyed = true;
		GameInterface::ResizeCollider(lvl->GetObjectWithName("Game"), floorColliderIndex, blockIndex);

		glm::vec2 shiftedPos(0);
		for (int i = 0; i < 4; i++)
		{
			int y = 8;
			int scaleY = 1;
			int scaleX = 1;
			if(i > 1)
			{
				y += 16;
				scaleY = -1;
			}
			if(i%2 !=0)
			{
				scaleX = -1;
			}

			shiftedPos = position + glm::vec2((((i % 2)) * 16)+8, y);
			destroyedBlocks.anim.push_back(AnimationComponent::AddSprite(1));
			AnimationComponent::SetAnimation(destroyedBlocks.anim.at(i), "DestroyedBlock");
			AnimationComponent::SetPosition(destroyedBlocks.anim.at(i), shiftedPos);
			AnimationComponent::SetScale(destroyedBlocks.anim.at(i), glm::vec2(scaleX, scaleY));
			AnimationComponent::SetOrigin(destroyedBlocks.anim.at(i), glm::vec2(8, 8));

			destroyedBlocks.collider.push_back(PhysicsComponent::AddDynamic(shiftedPos / 100.f, glm::vec2(0.04), 0, 1, 1, 0, 0, true));
			
			glm::vec2 linearVel(2,-7);
			if(scaleX == -1)
			{
				linearVel.x = -2;
			}
			if(scaleY == -1)
			{
				linearVel.y = -4;
			}
			PhysicsComponent::SetLinearVelocity(destroyedBlocks.collider[i], linearVel);
			
		}
	}
	else if(canBeHit && !destroyed)
	{
		AnimationComponent::SetAnimation(block, "BrickHit");
		AudioComponent::Play(bumpSound);
		canBeHit = false;
	}
}

void BrickBlock::SetPoweredUp(bool aMarioPoweredUp)
{
	marioPoweredUp = aMarioPoweredUp;
}

SCRIPT_END(BrickBlock)
