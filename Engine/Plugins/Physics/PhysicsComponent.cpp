#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Handle.h"
#include "Logger.h"
#include "ContactListener.h"

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

std::shared_ptr<b2World> PhysicsComponent::world;
std::vector<std::shared_ptr<b2Body>> PhysicsComponent::bodies;
std::shared_ptr<PairContactListener> PhysicsComponent::contactListener;

extern "C" PHYSICS_API PhysicsCompHandle PhysicsComponent::AddStatic(glm::vec2 aPos, glm::vec2 aSize, bool aIsSensor)
{
	//set all relevant data for the body
	b2BodyDef staticBodyDef;
	staticBodyDef.position.Set(aPos.x, aPos.y);
	staticBodyDef.userData = reinterpret_cast<void*>(bodies.size());

	//create the body
	bodies.push_back(std::shared_ptr<b2Body>(world->CreateBody(&staticBodyDef), [](b2Body* ptr) {world->DestroyBody(ptr); }));

	//set the shape to be a box
	b2PolygonShape staticShape;
	staticShape.SetAsBox(aSize.x, aSize.y);

	//Create the fixture and add it to the body.
		b2FixtureDef fixtureDef;
	fixtureDef.shape = &staticShape;
	fixtureDef.density = 0.0f;
	fixtureDef.isSensor = aIsSensor;

	bodies.back()->CreateFixture(&fixtureDef);

	//return the PhysicsCompHandle
	return { bodies.size() - 1 };
}

extern "C" PHYSICS_API PhysicsCompHandle PhysicsComponent::AddDynamic(glm::vec2 aPosition, glm::vec2 aSize, float aLinearDamping,
	float aGravityScale, float aDensity, float aRestitution, float aFriction, bool aIsSensor)
{
	b2BodyDef dynamicBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.fixedRotation = true;
	dynamicBodyDef.position.Set(aPosition.x, aPosition.y);
	dynamicBodyDef.linearDamping = aLinearDamping;
	dynamicBodyDef.gravityScale = aGravityScale;
	dynamicBodyDef.userData = reinterpret_cast<void*>(bodies.size());

	bodies.push_back(std::shared_ptr<b2Body>(world->CreateBody(&dynamicBodyDef), [](b2Body* ptr) {world->DestroyBody(ptr); }));

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(aSize.x, aSize.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = aDensity;
	fixtureDef.restitution = aRestitution;
	fixtureDef.friction = aFriction;
	fixtureDef.isSensor = aIsSensor;

	bodies.back()->CreateFixture(&fixtureDef);

	return { bodies.size() - 1 };
}

void PhysicsComponent::RemovePhysicsBody(PhysicsCompHandle& aHandle)
{
	if(aHandle.index == 1231234)
	{
		return;
	}
	//contactListener->RemoveAllCollisionPairsWithCollider(aHandle.index);
	bodies.at(aHandle.index).reset();
	aHandle = { 1231234 };
}

extern "C" PHYSICS_API glm::vec2 PhysicsComponent::GetPosition(PhysicsCompHandle aHandle)
{
	b2Vec2 pos = bodies.at(aHandle.index)->GetPosition();
	return { pos.x,pos.y };
}

void PhysicsComponent::SetPosition(PhysicsCompHandle aHandle, glm::vec2 pos)
{
	bodies.at(aHandle.index)->SetTransform(b2Vec2(pos.x, pos.y), 0);
}

extern "C" PHYSICS_API void PhysicsComponent::SetLinearVelocity(PhysicsCompHandle aHandle, glm::vec2 aVel)
{
	bodies.at(aHandle.index)->SetLinearVelocity({ aVel.x,aVel.y });
}

extern "C" PHYSICS_API glm::vec2 PhysicsComponent::GetLinearVelocity(PhysicsCompHandle aHandle)
{
	b2Vec2 pos = bodies.at(aHandle.index)->GetLinearVelocity();
	return { pos.x,pos.y };
}

/**
 * \brief finds if the ray has hit an object.
 * Also records the point and normal of the hit.
 */
class RayCastClosestCallback : public b2RayCastCallback
{
public:
	RayCastClosestCallback()
	{
		m_hit = false;
	}

	///Used by Box2D to find the closest object intersected by the ray.
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) override
	{
		b2Body* body = fixture->GetBody();

		m_hit = true;
		m_point = point;
		m_normal = normal;

		// By returning the current fraction, we instruct the calling code to clip the ray and
		// continue the ray-cast to the next fixture. WARNING: do not assume that fixtures
		// are reported in order. However, by clipping, we can always get the closest fixture.
		return fraction;
	}

	bool m_hit;
	b2Vec2 m_point;
	b2Vec2 m_normal;
};

extern "C" PHYSICS_API bool PhysicsComponent::RayCast(PhysicsCompHandle aHandle, glm::vec2 aOffset, glm::vec2 aDir, float aLength)
{
	aDir = glm::normalize(aDir);

	b2Vec2 point1 = bodies.at(aHandle.index)->GetPosition() + b2Vec2(aOffset.x, aOffset.y);
	b2Vec2 point2 = { aDir.x * aLength,aDir.y * aLength };
	point2 += point1;

	RayCastClosestCallback callback;
	world->RayCast(&callback, point1, point2);
	return callback.m_hit;
}

void PhysicsComponent::RegisterCollisionPair(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs)
{
	contactListener->AddCollisionPair({aLhs.index, aRhs.index});
}

void PhysicsComponent::UnregisterCollisionPair(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs)
{
	CollisionPair removingPair(aLhs.index, aRhs.index);
	if (contactListener->collisionDetails.find(removingPair) == contactListener->collisionDetails.end())
	{
		LOG_WARN(Logger::Get("Physics"), "There is no collisionPair with the following indices {} and {}, so we can't delete anything", aLhs.index, aRhs.index);
		return;
	}
	contactListener->collisionDetails.erase(removingPair);

	const auto it = contactListener->collisionDetails.find(removingPair);
	if(it != contactListener->collisionDetails.end())
	{
		contactListener->collisionDetails.erase(it);
	}
}

bool PhysicsComponent::GetAreColliding(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs)
{
	return contactListener->GetIsColliding({ aLhs.index, aRhs.index });
}

glm::vec2 PhysicsComponent::GetContactNormal(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs)
{
	b2Vec2 tempNormal = contactListener->GetCollisionNormal({ aLhs.index, aRhs.index });
	return { tempNormal.x,tempNormal.y };
}

extern "C" PHYSICS_API void PhysicsComponent::PrismaticJoint(PhysicsCompHandle aLhs, PhysicsCompHandle aAnchor, glm::vec2 worldAxis,
	glm::vec2 aBounds)
{
	b2PrismaticJointDef jointDef;
	jointDef.Initialize(bodies.at(aLhs.index).get(), bodies.at(aAnchor.index).get(), bodies.at(aAnchor.index)->GetWorldCenter(), b2Vec2(worldAxis.x, worldAxis.y));
	jointDef.lowerTranslation = aBounds.x;
	jointDef.upperTranslation = aBounds.y;
	jointDef.enableLimit = true;
	
	world->CreateJoint(&jointDef);
}

void PhysicsComponent::SetCategory(PhysicsCompHandle aHandle, unsigned short bits)
{
	b2Filter newFilter = bodies.at(aHandle.index)->GetFixtureList()[0].GetFilterData();
	newFilter.categoryBits = bits;
	bodies.at(aHandle.index)->GetFixtureList()[0].SetFilterData(newFilter);
}

void PhysicsComponent::SetFilter(PhysicsCompHandle aHandle, unsigned short bits)
{
	b2Filter newFilter = bodies.at(aHandle.index)->GetFixtureList()[0].GetFilterData();
	newFilter.maskBits = bits;
	bodies.at(aHandle.index)->GetFixtureList()[0].SetFilterData(newFilter);
}

