#include "PlatformBehaviour.h"
#include "Entity.h"

const float PlatformBehaviour::X_VELOCITY = 4.0f;

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
	m_platformComp->SetBehaviour(this);

	SetColour(Colour::RED);
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
	Colour curColour = (Colour)((short)GetColour() + 1);
	if (curColour >= Colour::NUM_COLOURS)
		curColour = Colour::RED;

	SetColour(curColour);
	m_graphicsComp->SetColour(Behaviour::ConvertColour(curColour));
}




