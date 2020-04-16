#pragma once

#include <vector>
#include "Handle.h"
#include <unordered_map>
#include <map>
#include "Box2D/Box2D.h"

typedef std::pair<size_t, size_t> CollisionPair;

/**
 * \brief The data that the b2contactListener registers
 */
struct CollisionDetail
{
	bool isColliding;
	b2Vec2 localCollisionNormal;
};

/**
 * \brief Checks two physicsComponents and checks if they are 
 *		colliding or not and the collisionNormal between them.
 *		
 *	This is used internally to check if two objects are colliding.
 */
class PairContactListener : public b2ContactListener
{
public:
	PairContactListener() {};
	/**
	 * \brief Registers a CollisionPair so that they can send signals back when they collide.
	 * \param aPair The collisionPair that you want to register
	 */
	void AddCollisionPair(CollisionPair aPair);
	
	/**
	 * \brief Returns if the CollisionPair is colliding. 
	 * \note It doesn't matter if the collisionPair order is reversed or not.
	 * \param aPair The CollisionPair you want to test.
	 * \return If the CollisionPair is colliding, returns true if they are colliding, false if they are not.
	 */
	bool GetIsColliding(CollisionPair aPair);
	/**
	 * \brief Returns the collisionNormal of the CollisionPair.
	 * \param aPair The CollisionPair you want to test.	
	 * \warning always check if the objects are colliding first, otherwise an invalid normal will be returned.
	 * \return The normal of the collision, only valid if the objects are actually colliding.
	 */
	b2Vec2 GetCollisionNormal(CollisionPair aPair);
	
	/**
	 * \brief Internal Box2D function to register the beginning of a collisions.
	 * \param contact Internal Box2D contact
	 */
	virtual void BeginContact(b2Contact* contact) override;
	/**
	* \brief Internal Box2D function to register the end of a collisions.
	* \param contact Internal Box2D contact
	*/
	virtual void EndContact(b2Contact* contact) override;

private:
	friend class PhysicsPlugin;
	friend class PhysicsComponent;

	///the collisionDetails of each of the registerd collision pairs
	std::map<CollisionPair, CollisionDetail> collisionDetails;
};

