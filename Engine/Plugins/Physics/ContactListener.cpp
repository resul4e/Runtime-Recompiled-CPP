#include "ContactListener.h"
#include "Logger.h"
#include "Box2D/Box2D.h"

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

void PairContactListener::AddCollisionPair(CollisionPair aPair)
{
	//collisionPairs.push_back(aPair);
	collisionDetails.insert(std::pair<CollisionPair, CollisionDetail>(aPair, { false,b2Vec2_zero }));
}

bool PairContactListener::GetIsColliding(CollisionPair aPair)
{
	if(collisionDetails.find(aPair) == collisionDetails.end())
	{
		CollisionPair switchedPair = CollisionPair(aPair.first, aPair.second);
		if(collisionDetails.find(switchedPair) == collisionDetails.end())
		{
			return false;
		}
		return collisionDetails.at(switchedPair).isColliding;
	}
	return collisionDetails.at(aPair).isColliding;
}

b2Vec2 PairContactListener::GetCollisionNormal(CollisionPair aPair)
{
	if(GetIsColliding(aPair))
	{
		return collisionDetails.at(aPair).localCollisionNormal;
	}
	return b2Vec2_zero;
}


void PairContactListener::BeginContact(b2Contact* contact)
{
	for(auto p : collisionDetails)
	{
		if (reinterpret_cast<size_t>(contact->GetFixtureA()->GetBody()->GetUserData()) == p.first.first)
		{
			if (reinterpret_cast<size_t>(contact->GetFixtureB()->GetBody()->GetUserData()) == p.first.second)
			{
				if (collisionDetails.find(p.first) != collisionDetails.end())
				{
					collisionDetails.at(p.first).localCollisionNormal = contact->GetManifold()->localNormal;
					collisionDetails.at(p.first).isColliding = true;
				}
			}
		}
		else if (reinterpret_cast<size_t>(contact->GetFixtureB()->GetBody()->GetUserData()) == p.first.first)
		{
			if (reinterpret_cast<size_t>(contact->GetFixtureA()->GetBody()->GetUserData()) == p.first.second)
			{
				if (collisionDetails.find(p.first) != collisionDetails.end())
				{
					collisionDetails.at(p.first).localCollisionNormal = -contact->GetManifold()->localNormal;
					collisionDetails.at(p.first).isColliding = true;
				}
			}
		}
	}
}

void PairContactListener::EndContact(b2Contact* contact)
{
	for (auto p : collisionDetails)
	{
		if (reinterpret_cast<size_t>(contact->GetFixtureA()->GetBody()->GetUserData()) == p.first.first)
		{
			if (reinterpret_cast<size_t>(contact->GetFixtureB()->GetBody()->GetUserData()) == p.first.second)
			{
				if(collisionDetails.find(p.first) != collisionDetails.end())
				{
					collisionDetails.at(p.first).isColliding = false;
				}

			}
		}
		else if (reinterpret_cast<size_t>(contact->GetFixtureB()->GetBody()->GetUserData()) == p.first.first)
		{
			if (reinterpret_cast<size_t>(contact->GetFixtureA()->GetBody()->GetUserData()) == p.first.second)
			{
				if (collisionDetails.find(p.first) != collisionDetails.end())
				{
					collisionDetails.at(p.first).isColliding = false;
				}
			}
		}
	}
}
