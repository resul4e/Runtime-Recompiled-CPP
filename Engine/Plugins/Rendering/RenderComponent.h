#pragma once
#include "ExportDetails.h"
#include "Enums.h"
#include "Filesystem.h"
#include <unordered_map>
#include "Transform/Vector.h"


//forward declarations
struct RenderCompHandle;

namespace sf
{
	class Shape;
	class Sprite;
	class Texture;
	class Drawable;
}


/**
 * \brief Class that holds all of the free-functions to be able to render things on screen.
 * 
 * A new RenderComponent is added using the Add() function or one of the Add*() function.
 * 
 * 
 * \warning Will crash the script if RenderCompHandle points to an invalid RenderShape.
 */
class RenderComponent
{
	RenderComponent(){};
	~RenderComponent(){};
public:
	/**
	* \brief Creates a renderComponent
	* \param aRenderShape The shape of the renderComponent, supports
	* \see RenderShape
	* \see RenderCompHandle
	* \return a handle to the renderComponent
	*/
	RENDERING_API static RenderCompHandle Add(RenderShape aRenderShape);
	/**
	 * \brief Creates a renderComponent with renderShape Circle
	 * \param aRadius the radius of the created circle
	 * \see RenderShape
	 * \see Add()
	 * \return A handle to the renderComponent.
	 */
	RENDERING_API static RenderCompHandle AddCircle(float aRadius);
	/**
	 * \brief Creates a renderComponent with renderShape Box
	 * \param aHalfDimensionX the width of the box
	 * \param aHalfDimensionY the height of the box
	 * \see RenderShape
	 * \see Add()
	 * \return A handle to the renderComponent.
	 */
	RENDERING_API static RenderCompHandle AddBox(float aHalfDimensionX, float aHalfDimensionY);
	/**
	* \brief Creates a renderComponent with renderShape Box
	* \param aHalfDimension the width and height divided by two of the box
	* \see RenderShape
	* \see Add()
	* \return A handle to the renderComponent.
	*/
	RENDERING_API static RenderCompHandle AddBox(const glm::vec2& aHalfDimension);
	/**
	 * \brief Creates a renderComponent with renderShape Polygon.
	 * \see Add()
	 * \see RenderShape
	 * \warning This function expects the vertices to be sorted in a clockwise or counter-clockwise manner
	 * \return A handle to the renderComponent.
	 */
	RENDERING_API static RenderCompHandle AddPolygon(const std::vector<glm::vec2>& aVertices);
	/**
	 * \brief returns the current RenderShape of shape
	 * \param aHandle The RenderComponent you want to modify.
	 * \see RenderShape
	 * \return The renderShape
	 */
	RENDERING_API static RenderShape GetRenderShape(const RenderCompHandle& aHandle);
	/**
	 * \brief changes the RenderShape of the object. You'll have to call the shape specific function to change the default values (e.g. SetRadius())
	 * \see Add()
	 * \param aHandle the shape you want to change
	 * \param aNewShape the new RenderShape
	 */
	RENDERING_API static void ChangeRenderShape(const RenderCompHandle& aHandle, RenderShape aNewShape);
	/**
	 * \brief Sets the color of the shape.
	 * \param aHandle	The RenderComponent you want to modify.
	 * \param aR		The red channel
	 * \param aG		The green channel
	 * \param aB		the blue channel
	 * \param aA		the alpha channel
	 */
	RENDERING_API static void SetFillColor(const RenderCompHandle& aHandle, unsigned char aR, unsigned char aG, unsigned char aB, unsigned char aA);
	/**
	 * \brief Gets the color of the shape.
	 * \param aHandle The RenderComponent you want the color of.
	 * \return the color of the shape.
	 */
	RENDERING_API static glm::vec4 GetFillColor(const RenderCompHandle& aHandle);
	/**
	 * \brief Sets the color of the outline of the shape. 
	 * 
	 * The outline thickness is originally set to 0 so to see the outline color change the outline thickness
	 * 
	 * \param aHandle	The RenderComponent you want to modify.
	 * \param aR		The red channel
	 * \param aG		The green channel
	 * \param aB		the blue channel
	 * \param aA		the alpha channel
	 * \see SetOutlineThickness()
	 */
	RENDERING_API static void SetOutlineColor(const RenderCompHandle& aHandle, unsigned char aR, unsigned char aG, unsigned char aB, unsigned char aA);
	/**
	* \brief Gets the outline color of the shape.
	* \param aHandle The RenderComponent you want the outline color of.
	* \return the outline color of the shape.
	*/
	RENDERING_API static glm::vec4 GetOutlineColor(const RenderCompHandle& aHandle);
	/**
	 * \brief Sets the thickness of the outline of the shape.
	 * 
	 * Negative values will draw the outline on the inside of the shape. 
	 * Positive values will draw on the outside of the shape. So if you want to preserve
	 * the dimensions of your shape use negative values.
	 * 
	 * \param aHandle		The RenderComponent you want to modify.
	 * \param aThickness	The thickness of the outline of the shape.
	 */
	RENDERING_API static void SetOutlineThickness(const RenderCompHandle& aHandle, float aThickness);
	/**
	 * \brief Gets the outline thickness of the shape.
	 * \param aHandle The RenderComponent you want the outline thickness of.
	 * \return The outline thickness.
	 */
	RENDERING_API static float GetOutlineThickness(const RenderCompHandle& aHandle);
	/**
	* \brief Gets all of the vertices that makes up this shape.
	* \param aHandle The RenderComponent that you want the vertices from.
	* \note All shapes are made up of vertices so you can get the vertices of all of the shapes
	* \return the list of vertices from the RenderShape.
	*/
	RENDERING_API static std::vector<glm::vec2> GetVertices(const RenderCompHandle& aHandle);
	/**
	* \brief Gets a vertex at the supplied index.
	* \param aHandle The RenderComponent that you want the vertex from.
	* \param aIndex The index of the vertex you want to have.
	* \note All shapes are made up of vertices so you can get the vertices of all of the shapes
	* \return The vertex at the supplied index.
	*/
	RENDERING_API static glm::vec2 GetVertex(const RenderCompHandle& aHandle, size_t aIndex);
	/**
	 * \brief Sets the origin of the shape.
	 * \param aHandle The RenderComponent that you want to modify.
	 * \param aOrigin the new Origin of the shape.
	 */
	RENDERING_API static void SetOrigin(const RenderCompHandle& aHandle, glm::vec2 aOrigin);
	/**
	 * \brief Gets the origin of the shape.
	 * \param aHandle The RenderComponent that you want the Origin of.
	 * \return The origin of the shape.
	 */
	RENDERING_API static glm::vec2 GetOrigin(const RenderCompHandle& aHandle);
	/**
	 * \brief Sets the position of the RenderComponent.
	 * \param aHandle			The RenderComponent you want to change the position of.
	 * \param aNewPosition		The new position
	 */
	RENDERING_API static void SetPosition(const RenderCompHandle& aHandle, glm::vec2 aNewPosition);
	/**
	 * \brief Gets the position of the RenderComponent.
	 * \param aHandle The RenderComponent you want to get the position of.
	 * \return The position of the RenderComponent
	 */
	RENDERING_API static glm::vec2 GetPosition(const RenderCompHandle& aHandle);
	
	/////////////////////////////////
	//Shape specific functions
	/////////////////////////////////
	/**
	 * \brief Sets the radius of the CircleShape.
	 * \param aHandle	The RenderComponent you want to modify.
	 * \param aRadius	The radius of the circle.
	 * \note will assert in debug mode if the RenderShape is not a CircleShape
	 * \note will silently return in release mode if the RenderShape is not a CircleShape
	 */
	RENDERING_API static void SetRadius(const RenderCompHandle& aHandle, float aRadius);
	/**
	 * \brief returns the radius of the RenderComponent supplied
	 * \param aHandle The RenderComponent you want the information from.
	 * \return the radius as a float
	 */
	RENDERING_API static float GetRadius(const RenderCompHandle& aHandle);
	/**
	 * \brief Sets the halfDimensions of the BoxShape
	 * \param aHandle			The RenderComponent you want to modify.
	 * \param aHalfDimensionX	Half of the full width.
	 * \param aHalfDimensionY	Half of the fill height.
	 * \note will assert in debug mode if the RenderShape is not a BoxShape
	 * \note will silently return in release mode if the RenderShape is not a BoxShape
	 */
	RENDERING_API static void SetHalfDimension(const RenderCompHandle& aHandle, float aHalfDimensionX, float aHalfDimensionY);
	/**
	 * \brief Sets the halfDimensions of the BoxShape
	 * \param aHandle			The RenderComponent you want to modify.
	 * \param aHalfDimensions	Half of both the width and height.
	 */
	RENDERING_API static void SetHalfDimension(const RenderCompHandle& aHandle, const glm::vec2& aHalfDimensions);
	/**
	 * \brief returns the HalfDimensions of the object
	 * \param aHandle The RenderComponent you want the information from.
	 * \return the HalfDimension as a vec2
	 */
	RENDERING_API static glm::vec2 GetHalfDimension(const RenderCompHandle& aHandle);
	/**
	 * \brief Gets a list of unsorted vertices and sorts them in a clockwise order.
	 * \note Vertices have to be convex.
	 * \param aHandle				The RenderComponent you want to modify.
	 * \param aUnsortedPointList	The points of the convex object in a random order.
	 */
	RENDERING_API static void SetUnsortedVertices(const RenderCompHandle& aHandle,const std::vector<glm::vec2>& aUnsortedPointList);
	/**
	 * \brief Creates a convex object with the sorted points supplied
	 * \note Vertices have to be convex.
	 * \param aHandle			The RenderComponent you want to modify.
	 * \param aSortedPointList	The points of the Convex object in a Clockwise or Counter-Clockwise manner.
	 */
	RENDERING_API static void SetSortedVertices(const RenderCompHandle& aHandle,const std::vector<glm::vec2>& aSortedPointList);
	/**
	 * \brief Adds a vertex in between the vertices at index-1 and index.
	 * \note The shape still has to be convex after insertion.
	 * \note The shape's orientation still has to be clockwise or counter-clockwise.
	 * \param aHandle The RenderComponent you want to modify.
	 * \param aVertex The coordinates of the vertex you want to add.
	 * \param aIndex The place where you want to insert your vertex.
	 */
	RENDERING_API static void AddVertex(const RenderCompHandle& aHandle, const glm::vec2& aVertex, size_t aIndex);
private:
	/**
	 * \brief Creates an sf::Shape according to the supplied RenderShape.
	 * \param aRenderShape the RenderShape you want to create.
	 * \return The sf::Shape pointer.
	 */
	static sf::Drawable* PickRenderShape(RenderShape aRenderShape);
	/**
	 * \brief Checks if the RenderShape matches the RenderComponent that is saved.
	 * \param aHandle		The RenderShape you want to check.
	 * \param aRenderShape	The RenderShape you want the saved RenderShape to be.
	 * \return true if the shape is the same, false if they are not.
	 */
	static bool IsRenderShapeCorrect(const RenderCompHandle& aHandle, RenderShape aRenderShape);

public:
private:
	friend class Window;
	/// The sf::Shapes that are stored.
	static std::vector<std::shared_ptr<sf::Drawable>> shapeList;
	/// The RenderShapes that are stored.
	static std::vector<RenderShape> renderShapeList;
};