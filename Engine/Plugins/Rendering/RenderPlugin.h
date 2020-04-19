#pragma once
#include "PluginBase.h"
#include "ExportDetails.h"

class Window;


/**
 * \brief Implementation of PluginBase, Creates and Updates everything that has to do with rendering
 * \see PluginBase
 */
#ifdef ENGINE_PROJECT
class RenderPlugin : public PluginBase
{
public:
	RenderPlugin();
	~RenderPlugin() {}

	/**
	 * \brief Calls Start on Window
	 */
	RENDERING_API void Start() override;
	/**
	 * \brief Does nothing
	 */
	RENDERING_API void FixedUpdate() override;
	/**
	 * \brief Calls the Update function on Window
	 * \param deltatime the time between the last frame and this one
	 */
	RENDERING_API void Update(float deltatime) override;
	/**
	 * \brief Does nothing
	 */
	RENDERING_API void Delete() override;

	RENDERING_API void SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories) override;

private:
	///the window that gets created
	std::shared_ptr<Window> window;
};
#endif