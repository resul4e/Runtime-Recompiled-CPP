#pragma once
/**
 * \brief All of the rendershapes supported by the renderingComponent
 */
enum class RenderShape
{
	BoxShape, ///< a box with width and height
	CircleShape, ///< a circle with a radius
	PolygonShape, ///< a convex polygon made using points
};