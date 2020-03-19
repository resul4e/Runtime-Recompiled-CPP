#include "Player.h"
#include "Core/Storage.h"
#include "Core/Level.h"

#include "Rendering/RenderComponent.h"
#include <Physics/PhysicsComponent.h>
#include "Input/InputComponent.h"

#include "Paddle.h"

SCRIPT_START(Player)

Player::Player()
{
}


Player::~Player()
{
}

void Player::Start()
{
	playerPaddle = lvl->CreateObject("Paddle", "PlayerPaddle");
	PaddleInterface::Setup(playerPaddle, 0.2f);
}

void Player::Restart()
{
}

void Player::FixedUpdate()
{
}

void Player::Update(float aDeltatime)
{
	if(InputComponent::GetKeyPressed(InputComponent::Key::S))
	{
		PaddleInterface::MoveDown(playerPaddle);
	}
	
	if (InputComponent::GetKeyPressed(InputComponent::Key::W))
	{
		PaddleInterface::MoveUp(playerPaddle);
	}
}

void Player::Delete()
{
}

void Player::Serialize(Storage& storage)
{
	storage.Serialize(playerPaddle);
}

SCRIPT_END(Player)