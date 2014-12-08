#include "PlatformBehaviour.h"
#include "Entity.h"

const float PlatformBehaviour::X_VELOCITY = 5.0f;

void PlatformBehaviour::Init(GraphicsComponent * graphicsComponent, PlatformComponent * platformComponent, float changeInterval, float minX, float maxX)
{
	m_graphicsComp = graphicsComponent;
	m_platformComp = platformComponent;

	m_changeInterval = changeInterval;
	m_lastChange = 0;

	m_minX = minX;
	m_maxX = maxX;

	EVector3f velocity = EVector3f(5.0f, 0.0f, 0.0f);
	m_platformComp->SetVelocity(velocity);

	m_curColour = Colour::RED;
	ChangeColour();
}

void PlatformBehaviour::Update(float dt)
{
	if (m_lastChange > m_changeInterval)
	{
		m_lastChange = 0;
		ChangeColour();
	}

	EVector3f pos = m_Owner->GetPosition();
	if (pos.x < m_minX && m_platformComp->GetVelocity().x < 0)
	{
		m_platformComp->SetVelocity(EVector3f(X_VELOCITY, 0.0f, 0.0f));
	}

	else if (pos.x > m_maxX && m_platformComp->GetVelocity().x > 0)
	{
		m_platformComp->SetVelocity(EVector3f(-X_VELOCITY, 0.0f, 0.0f));
	}

	m_lastChange += dt;
}

void PlatformBehaviour::ChangeColour()
{
	EVector3f colour = EVector3f(0.0f, 0.0f, 0.0f);

	m_curColour = (Colour)((short)m_curColour + 1);
	if (m_curColour >= Colour::NUM_COLOURS)
		m_curColour = Colour::RED;

	switch (m_curColour)
	{
	case Colour::RED:
		colour.x = 1.0f;
		break;

	case Colour::YELLOW:
		colour.x = 0.5f;
		colour.y = 0.5f;
		break;

	case Colour::GREEN:
		colour.y = 1.0f;
		break;

	case Colour::TEAL:
		colour.y = 0.5f;
		colour.z = 0.5f;
		break;

	case Colour::BLUE:
		colour.z = 1.0f;
		break;

	case Colour::MAGENTA:
		colour.x = 0.5f;
		colour.z = 0.5f;
		break;
	}

	m_graphicsComp->SetColour(colour);
}




