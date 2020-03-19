#pragma once

#include "ExportDetails.h"
#include <vector>


struct PhysicsCompHandle;
class b2World;
class b2Body;
class PairContactListener;

/**
 * \brief Class that acts as the API between game code and the plugin.
 * 
 * Has support for dynamic and static axis-aligned boxes.
 */
class PhysicsComponent
{
	PhysicsComponent() {};
	~PhysicsComponent(){};
public:
	/**
	 * \brief Creates a physics cube that will never move.
	 * \param aPosition The position of the centerpoint.
	 * \param aHalfSize The width and hight divided by 2.
	 * \return A PhysicsCompHandle used by the other methods to identify the PhysicsComponent.
	 */
	PHYSICS_API static PhysicsCompHandle AddStatic(glm::vec2 aPosition, glm::vec2 aHalfSize, bool aIsSensor = false);
	/**
	 * \brief Creates a physics cube that can move.
	 * \param aPosition The position of the centerpoint
	 * \param aHalfSize The width and height divided by two.
	 * \param aLinearDamping Reduces the linear velocity.
	 * \param aGravityScale How much gravity affects this object.
	 * \param density The density.
	 * \param aRestitution How bouncy the object is.
	 * \param aFriction The friction. Usually in the range [0,1]
	 * \param aIsSensor If this is true it will not react to collisions, only generate collision events.
	 * \return A PhysicsCompHandle used by the other methods to identify the PhysicsComponent.
	 */
	PHYSICS_API static PhysicsCompHandle AddDynamic(glm::vec2 aPosition, glm::vec2 aHalfSize,float aLinearDamping = 0.0f,
		float aGravityScale = 1.0f, float density = 1.0f, float aRestitution = 0.0f, float aFriction = 0, bool aIsSensor = false);
	/**
	 * \brief Removes a physicsComponent.
	 * \param aHandle The PhysicsCompHandle that you want to remove
	 */
	PHYSICS_API static void RemovePhysicsBody(PhysicsCompHandle& aHandle);
	/**
	 * \brief Gets the position of the centerpoint of the object.
	 * \param aHandle The PhysicsCompHandle that you want the position of.
	 * \return The position of the physicsComponent.
	 */
	PHYSICS_API static glm::vec2 GetPosition(PhysicsCompHandle aHandle);
	/**
	 * \brief Moves the centerpoint to the supplied position.
	 * \param aHandle The PhysicsCompHandle you want to move.
	 * \param aPos The new position of the physicsComponent.
	 */
	PHYSICS_API static void SetPosition(PhysicsCompHandle aHandle, glm::vec2 aPos);
	/**
	 * \brief Sets the velocity of the PhysicsComponent.
	 * \param aHandle The Handle you want to change the velocity of.
	 * \param aVel The new velocity of the physicsComponent.
	 */
	PHYSICS_API static void SetLinearVelocity(PhysicsCompHandle aHandle, glm::vec2 aVel);
	/**
	 * \brief Gets the velocity of the PhysicsComponent.
	 * \param aHandle The Handle you want the velocity of.
	 * \return The velocity of the PhysicsComponent.
	 */
	PHYSICS_API static glm::vec2 GetLinearVelocity(PhysicsCompHandle aHandle);
	/**
	 * \brief Casts a ray from a particular physicsComponent to see if it is hitting something.
	 * \param aHandle The Handle from which the rays will originate.
	 * \param aOffset The offset from the centerpoint of the PhysicsComponent.
	 * \param aDir The direction of the ray.	
	 * \param length The length of the ray.
	 * \return if the ray is hitting something. Returns true if hit, otherwise false.
	 */
	PHYSICS_API static bool RayCast(PhysicsCompHandle aHandle, glm::vec2 aOffset, glm::vec2 aDir, float length);
	/**
	 * \brief Creates a pair from the two Handles supplied. This is necessary to use GetAreColliding() and GetContactNormal.
	 * \param aLhs The first PhysicsComponent.
	 * \param aRhs The second PhysicsComponent.
	 */
	PHYSICS_API static void RegisterCollisionPair(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs);
	/**
	 * \brief Removes a pair from the registered collision Pairs.
	 * \param aLhs The first PhysicsComponent.
	 * \param aRhs The second PhysicsComponent.
	 */
	PHYSICS_API static void UnregisterCollisionPair(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs);
	/**
	 * \brief If the two PhysicsComponents are colliding.
	 * \param aLhs The first PhysicsComponent.
	 * \param aRhs The second PhysicsComponent.
	 * \return If the two PhysicsComponents are colliding. If they are colliding returns true, else false.
	 * \see GetContactNormal()
	 * \see RegisterCollisionPair()
	 */
	PHYSICS_API static bool GetAreColliding(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs);
	/**
	 * \brief Returns the Normal of the collision.
	 * \param aLhs The first PhysicsComponent.
	 * \param aRhs The second PhysicsComponent.
	 * \return The contactNormal.
	 * \warning Only works if GetAreColliding is true.
	 * \see GetAreColliding()
	 * \see RegisterCollisionPair()
	 */
	PHYSICS_API static glm::vec2 GetContactNormal(PhysicsCompHandle aLhs, PhysicsCompHandle aRhs);
	/**
	 * \brief 
	 * \param aHandle The Handle you want to lock on an axis
	 * \param aAnchor A static PhysicsComponent that acts like an anchor.
	 * \param worldAxis The axis you want to bind the PhysicsComponent in.	
	 * \param aBounds The upper and lower bound, offset from the centerpoint of aHandle
	 */
	PHYSICS_API static void PrismaticJoint(PhysicsCompHandle aHandle, PhysicsCompHandle aAnchor, glm::vec2 worldAxis, glm::vec2 aBounds);

	//test
	PHYSICS_API static void SetCategory(PhysicsCompHandle aHandle, unsigned short bits);
	PHYSICS_API static void SetFilter(PhysicsCompHandle aHandle, unsigned short bits);

private:

public:
private:
	friend class PhysicsPlugin;

	///The world
	static std::shared_ptr<b2World> world;
	///All of the physics bodies
	static std::vector<std::shared_ptr<b2Body>> bodies;
	///The contact listener resposible for checking for contacts
	static std::shared_ptr<PairContactListener> contactListener;
};

