#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include "Component.h"

class GraphicsComponent : public Component
{
public:
	enum GraphicShapeTypes { GST_Plane = 0, GST_Sphere, GST_Cube };

	GraphicsComponent() : m_ShapeType(GST_Plane), m_rotation(0) {}
	virtual ~GraphicsComponent() {}
	virtual void Init(GraphicShapeTypes shapeType) { SetShapeType(shapeType); }
	virtual void Shutdown() {}

	virtual void Update(float deltaTime);
	virtual void Render();

	GraphicShapeTypes GetShapeType() const { return m_ShapeType; }
	void SetShapeType(GraphicShapeTypes shapeType) { m_ShapeType = shapeType; }

	double GetRadius() const { return m_Radius;}
	void SetRadius(double radius) {m_Radius = radius;}

	float GetHeight() const { return m_Height;}
	void SetHeight(float height) {m_Height = height;}

	float GetWidth() const { return m_Width;}
	void SetWidth(float width) {m_Width = width;}

	void DrawPlane();
	void DrawSphere();
	void DrawCube();

	EVector3f GetColour() const {return m_Colour; }
	void SetColour(EVector3f colour) {m_Colour = colour;}

	float GetRotation() const { return m_rotation; }
	void SetRotation(float rotation) { m_rotation = rotation; }

private:
	double m_Radius;
	float m_Height;
	float m_Width;
	GraphicShapeTypes m_ShapeType;
	EVector3f m_Colour;
	float m_rotation;
	
protected:
};

#endif