#pragma once
#include "PluginBase.h"

/**
 * \brief Does nothing
 */
class TransformPlugin : public PluginBase
{
public:
	TransformPlugin();
	~TransformPlugin();

	/**
	 * \brief Does nothing
	 */
	void Start() override;
	/**
	* \brief Does nothing
	*/
	void FixedUpdate() override;
	/**
	* \brief Does nothing
	*/
	void Update(float aDeltatime) override;
	/**
	* \brief Does nothing
	*/
	void Delete() override;
};

