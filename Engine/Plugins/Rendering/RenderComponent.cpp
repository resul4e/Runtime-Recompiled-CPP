#include "RenderComponent.h"

#include <cassert>
#include <memory>
#include <limits>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Handle.h"

std::vector<std::shared_ptr<sf::Drawable>> RenderComponent::shapeList;
std::vector<RenderShape> RenderComponent::renderShapeList;

extern "C" RENDERING_API RenderCompHandle RenderComponent::Add(const RenderShape aRenderShape)
{
	renderShapeList.push_back(aRenderShape);

	shapeList.push_back(std::unique_ptr<sf::Drawable>(PickRenderShape(aRenderShape)));

	const RenderCompHandle tempComp = {shapeList.size() -1};
	return tempComp;
}

extern "C" RENDERING_API RenderCompHandle RenderComponent::AddCircle(const float aRadius)
{
	const RenderCompHandle tempHandle = Add(RenderShape::CircleShape);
	SetRadius(tempHandle, aRadius);
	return tempHandle;
}

extern "C" RENDERING_API RenderCompHandle RenderComponent::AddBox(const float aHalfDimensionX, const float aHalfDimensionY)
{
	const RenderCompHandle tempHandle = Add(RenderShape::BoxShape);
	SetHalfDimension(tempHandle, aHalfDimensionX, aHalfDimensionY);
	return tempHandle;
}

extern "C" RENDERING_API RenderCompHandle RenderComponent::AddBox(const glm::vec2& aHalfDimension)
{
	return AddBox(aHalfDimension.x, aHalfDimension.y);
}

extern "C" RENDERING_API RenderCompHandle RenderComponent::AddPolygon(const std::vector<glm::vec2>& aVertices)
{
	const RenderCompHandle tempHandle = Add(RenderShape::PolygonShape);
	SetSortedVertices(tempHandle, aVertices);
	return tempHandle;
}

extern "C" RENDERING_API RenderShape RenderComponent::GetRenderShape(const RenderCompHandle& aHandle)
{
	return renderShapeList.at(aHandle.index);
}

extern "C" RENDERING_API void RenderComponent::ChangeRenderShape(const RenderCompHandle& aHandle, RenderShape aNewShape)
{
	renderShapeList.at(aHandle.index) = aNewShape;
	shapeList.at(aHandle.index) = std::shared_ptr<sf::Drawable>(PickRenderShape(aNewShape));
}

extern "C" RENDERING_API void RenderComponent::SetFillColor(const RenderCompHandle& aHandle, const unsigned char aR, 
														 const unsigned char aG, const unsigned char aB, const unsigned char aA)
{
	std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->setFillColor(sf::Color(aR, aG, aB, aA));
}

///\todo weirdest shit ever
glm::vec4 color;
extern "C" RENDERING_API glm::vec4 RenderComponent::GetFillColor(const RenderCompHandle& aHandle)
{
	sf::Color c = std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getFillColor();
	color = glm::vec4(c.r, c.g, c.b, c.a);
	return color;
}

extern "C" RENDERING_API void RenderComponent::SetOutlineColor(const RenderCompHandle& aHandle, const unsigned char aR, 
															const unsigned char aG, const unsigned char aB, const unsigned char aA)
{
	std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->setOutlineColor(sf::Color(aR, aG, aB, aA));
}

extern "C" RENDERING_API glm::vec4 RenderComponent::GetOutlineColor(const RenderCompHandle& aHandle)
{
	sf::Color c = std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getOutlineColor();
	return glm::vec4(c.r, c.g, c.b, c.a);
}

extern "C" RENDERING_API void RenderComponent::SetOutlineThickness(const RenderCompHandle& aHandle, const float aThickness)
{
	std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->setOutlineThickness(aThickness);
}

extern "C" RENDERING_API float RenderComponent::GetOutlineThickness(const RenderCompHandle& aHandle)
{
	return std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getOutlineThickness();
}

extern "C" RENDERING_API void RenderComponent::SetPosition(const RenderCompHandle& aHandle, glm::vec2 aNewPosition)
{
	std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->setPosition(aNewPosition.x, aNewPosition.y);
}

extern "C" RENDERING_API glm::vec2 RenderComponent::GetPosition(const RenderCompHandle& aHandle)
{
	const sf::Vector2f tempVec = std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getPosition();
	return glm::vec2(tempVec.x, tempVec.y);
}

extern "C" RENDERING_API void RenderComponent::SetRadius(const RenderCompHandle& aHandle, const float aRadius)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::CircleShape))
	{
		return;
	}

	std::static_pointer_cast<sf::CircleShape>(shapeList.at(aHandle.index))->setRadius(aRadius);
}

extern "C" RENDERING_API float RenderComponent::GetRadius(const RenderCompHandle& aHandle)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::CircleShape))
	{
		return std::numeric_limits<float>::infinity();
	}

	return dynamic_cast<sf::CircleShape&>(*shapeList.at(aHandle.index)).getRadius();
}

extern "C" RENDERING_API void RenderComponent::SetHalfDimension(const RenderCompHandle& aHandle, const float aHalfDimensionX, const float aHalfDimensionY)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::BoxShape))
	{
		return;
	}

	dynamic_cast<sf::RectangleShape&>(*shapeList.at(aHandle.index)).setSize(sf::Vector2f(aHalfDimensionX*2.f, aHalfDimensionY*2.f));
}

extern "C" RENDERING_API void RenderComponent::SetHalfDimension(const RenderCompHandle& aHandle, const glm::vec2& aHalfDimensions)
{
	SetHalfDimension(aHandle, aHalfDimensions.x, aHalfDimensions.y);
}

extern "C" RENDERING_API glm::vec2 RenderComponent::GetHalfDimension(const RenderCompHandle& aHandle)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::BoxShape))
	{
		return glm::vec2(std::numeric_limits<float>::infinity());
	}
	
	const sf::Vector2f tempVector = dynamic_cast<sf::RectangleShape&>(*shapeList.at(aHandle.index)).getSize();
	return glm::vec2(tempVector.x/2.f, tempVector.y/2.f);
}

extern "C" RENDERING_API void RenderComponent::SetUnsortedVertices(const RenderCompHandle& aHandle,
	const std::vector<glm::vec2>& aUnsortedPointList)
{
	glm::vec2 centerPoint;
	for (int i = 0; i < aUnsortedPointList.size(); i++)
	{
		centerPoint += aUnsortedPointList.at(i);
	}

	centerPoint /= static_cast<float>(aUnsortedPointList.size());
	struct Sorting
	{
		int index;
		float angle;
	};
	std::vector<Sorting> sortedList;

	sortedList.resize(aUnsortedPointList.size());
	for (int i = 0; i < aUnsortedPointList.size(); i++)
	{
		sortedList.at(i).index = i;
		sortedList.at(i).angle = atan2(aUnsortedPointList[i].y - centerPoint.y, aUnsortedPointList[i].x - centerPoint.x);
	}

	qsort(&sortedList.front(), sortedList.size(), sizeof(Sorting), [](const void* a, const void* b) {if (((Sorting*)a)->angle < ((Sorting*)b)->angle)return -1; if (((Sorting*)a)->angle == ((Sorting*)b)->angle)return 0; return 1; });

	std::vector<glm::vec2> sortedPointList;
	sortedPointList.resize(sortedList.size());
	for(int i =0; i < sortedList.size(); i++)
	{
		sortedPointList.at(i) = aUnsortedPointList.at(sortedList.at(i).index);
	}

	SetSortedVertices(aHandle, sortedPointList);
}

extern "C" RENDERING_API void RenderComponent::SetSortedVertices(const RenderCompHandle& aHandle, const std::vector<glm::vec2>& aSortedPointList)
{
	if(!IsRenderShapeCorrect(aHandle, RenderShape::PolygonShape))
	{
		return;
	}

	dynamic_cast<sf::ConvexShape&>(*shapeList.at(aHandle.index)).setPointCount(aSortedPointList.size());
	for(int i=0; i< aSortedPointList.size(); i++)
	{
		dynamic_cast<sf::ConvexShape&>(*shapeList.at(aHandle.index)).setPoint(i, sf::Vector2f(aSortedPointList.at(i).x, aSortedPointList.at(i).y));
	}
	
}

extern "C" RENDERING_API void RenderComponent::AddVertex(const RenderCompHandle& aHandle, const glm::vec2& aVertex, size_t aIndex)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::PolygonShape))
	{
		return;
	}

	dynamic_cast<sf::ConvexShape&>(*shapeList.at(aHandle.index)).setPointCount(dynamic_cast<sf::ConvexShape&>(*shapeList.at(aHandle.index)).getPointCount() + 1);
	dynamic_cast<sf::ConvexShape&>(*shapeList.at(aHandle.index)).setPoint(aIndex, sf::Vector2f(aVertex.x,aVertex.y));
}

extern "C" RENDERING_API std::vector<glm::vec2> RenderComponent::GetVertices(const RenderCompHandle& aHandle)
{
	std::vector<glm::vec2> allVertices;
	for(size_t i=0; i < std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getPointCount(); i++)
	{
		sf::Vector2f tempVec = std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getPoint(i);
		allVertices.push_back(glm::vec2(tempVec.x, tempVec.y));
	}
	
	return allVertices;
}

extern "C" RENDERING_API glm::vec2 RenderComponent::GetVertex(const RenderCompHandle& aHandle, size_t aIndex)
{
	if (!IsRenderShapeCorrect(aHandle, RenderShape::PolygonShape))
	{
		return glm::vec2(std::numeric_limits<float>::infinity());
	}

	std::shared_ptr<sf::ConvexShape> convexShape = std::static_pointer_cast<sf::ConvexShape>(shapeList.at(aHandle.index));
	sf::Vector2f tempVec = convexShape->getPoint(aIndex);
	return glm::vec2(tempVec.x, tempVec.y);
}

extern "C" RENDERING_API void RenderComponent::SetOrigin(const RenderCompHandle& aHandle, glm::vec2 aOrigin)
{
	std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->setOrigin(aOrigin.x, aOrigin.y);
}

extern "C" RENDERING_API glm::vec2 RenderComponent::GetOrigin(const RenderCompHandle& aHandle)
{
	sf::Vector2f origin = std::static_pointer_cast<sf::Shape>(shapeList.at(aHandle.index))->getOrigin();
	return glm::vec2(origin.x, origin.y);
}

sf::Drawable* RenderComponent::PickRenderShape(RenderShape aRenderShape)
{
	switch (aRenderShape)
	{
	case RenderShape::BoxShape:
		return new sf::RectangleShape();
		break;
	case RenderShape::CircleShape:
		return new sf::CircleShape();
		break;
	case RenderShape::PolygonShape:
		return new sf::ConvexShape();
		break;
	default:
		assert(false && "this RenderShape is not yet implemented");
		return nullptr;
	}
}

bool RenderComponent::IsRenderShapeCorrect(const RenderCompHandle& aHandle, const RenderShape aRenderShape)
{
	if (renderShapeList.at(aHandle.index) != aRenderShape)
	{
		assert(false && "this component is not the same as the renderShape supplied");
		return false;
	}

	return true;
}



