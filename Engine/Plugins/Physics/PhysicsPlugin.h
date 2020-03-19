#pragma once
#include "PluginBase.h"
#include <memory>

class PairContactListener;

/**
 * \brief Interface between plugin and core.
 * 
 * Sets some of the data that should never change, e.g. gravity and physicsworld.
 * Updates the physics simulation.
 */
class PhysicsPlugin : public PluginBase
{
public:
	PhysicsPlugin(){};
	~PhysicsPlugin(){};
	/**
	 * \brief starts the physics simulation.
	 */
	void Start() override;
	/**
	 * \brief Updates the physics simulation.
	 */
	void FixedUpdate() override;
	/**
	 * \brief 
	 * \param aDeltatime This function does nothing.
	 */
	void Update(float aDeltatime) override;
	/**
	 * \brief clears the lists in the PhysicComponent.
	 */
	void Delete() override;

private:
	std::shared_ptr<b2World> world;
	std::shared_ptr<PairContactListener> contactListener;
};
