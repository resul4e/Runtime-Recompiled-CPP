#pragma once
#include "PluginBase.h"

/**
 * \brief Interface between plugin and core.
 * This class does almost nothing. 
 * Only clears the AudioLists when Delete is called.
 */
class AudioPlugin : public PluginBase
{
public:
	AudioPlugin(){};
	~AudioPlugin() {};

	/**
	 * \brief This creates the logger for the Audio plugin.
	 */
	void Start() override;
	/**
	 * \brief This function is empty.
	 */
	void FixedUpdate() override;
	/**
	 * \brief This function is empty.
	 * \param aDeltatime the time between the previous and current frame
	 */
	void Update(float aDeltatime) override;
	/**
	 * \brief Clears the AudioLists in the AudioComponent class
	 */
	void Delete() override;

	/**
	 * \brief Set the ConfigDirectories in the AudioComponent.
	 * \param aDirectories The list important directories.
	 */
	void SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories) override;
};
