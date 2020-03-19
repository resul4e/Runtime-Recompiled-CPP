#pragma once
#include "PluginBase.h"

/**
 * \brief Interface between plugin and core.
 * This class does almost nothing. 
 * It only sets the RenderWindow in the InputComponent.
 */
class InputPlugin :  public PluginBase
{
public:

	InputPlugin();
	~InputPlugin();

	/**
	 * \brief Sets the RenderWindow in the InputComponent.
	 */
	void Start() override;
	/**
	 * \brief This function does nothing.
	 */
	void FixedUpdate() override;
	/**
	 * \brief This function does nothing.
	 * \param aDeltatime The time between this frame and the last.
	 */
	void Update(float aDeltatime) override;
	/**
	 * \brief This function does nothing.
	 */
	void Delete() override;
};
