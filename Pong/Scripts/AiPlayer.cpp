#include "AiPlayer.h"
#include "Core/Level.h"
#include "Core/Storage.h"

#include "Physics/PhysicsComponent.h"

#include "Ball.h"
#include "Paddle.h"

SCRIPT_START(AiPlayer)

AiPlayer::AiPlayer()
{
}


AiPlayer::~AiPlayer()
{
}

void AiPlayer::Start()
{
	aiPaddle = lvl->CreateObject("Paddle", "AIPaddle");
	ballHandle = lvl->GetObjectWithName("TheBall");
	PaddleInterface::Setup(aiPaddle, 4.19);
}

void AiPlayer::Restart()
{
}

void AiPlayer::FixedUpdate()
{
}

void AiPlayer::Update(float aDeltatime)
{
	if(BallInterface::GetPosition(ballHandle).y < PaddleInterface::GetPhysicsPosition(aiPaddle).y)
	{
		PaddleInterface::MoveUp(aiPaddle);
	}
	else if (BallInterface::GetPosition(ballHandle).y > PaddleInterface::GetPhysicsPosition(aiPaddle).y)
	{
		PaddleInterface::MoveDown(aiPaddle);
	}
}

void AiPlayer::Delete()
{
}

void AiPlayer::Serialize(Storage& storage)
{
	storage.Serialize(aiPaddle, ballHandle);
}

SCRIPT_END(AiPlayer)