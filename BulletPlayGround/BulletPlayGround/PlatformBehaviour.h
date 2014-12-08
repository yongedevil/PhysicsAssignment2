#ifndef PLATFORM_BEHAVIOUR_H
#define PLATFORM_HEHAVIOUR_H

#include "Colour.h"
#include "Component.h"
#include "PlatformComponent.h"
#include "GraphicsComponent.h"
#include "PlatformComponent.h"

class PlatformBehaviour : public Component
{
private:
	static const float X_VELOCITY;

	float m_changeInterval;
	float m_lastChange;
	Colour m_curColour;
	float m_minX;
	float m_maxX;
	GraphicsComponent * m_graphicsComp;
	PlatformComponent * m_platformComp;


public:
	PlatformBehaviour() : m_graphicsComp(0), m_platformComp(0), m_changeInterval(1), m_lastChange(0), m_curColour(Colour::RED) { }
	~PlatformBehaviour() { }

	void Init(GraphicsComponent * graphicsComponent, PlatformComponent * platformComponent, float changeInterval, float minX, float maxX);
	void Shutdown() { }

	void Update(float dt);

	Colour GetColour() const { return m_curColour; }

private:
	void ChangeColour();
};


#endif