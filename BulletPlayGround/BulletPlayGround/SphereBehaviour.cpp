#include "SphereBehaviour.h"
//#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"
//#include "btBulletDynamicsCommon.h"

void SphereBehaviour::Init(GraphicsComponent * graphicsComponent, PhysicsComponent * physicComponent, Colour colour)
{
	m_colour = colour;
	m_colourVector = ConvertColour(m_colour);
	m_graphicsComp = graphicsComponent;
	m_physicsComp = physicComponent;

	m_graphicsComp->SetColour(m_colourVector);
	m_physicsComp->SetGravity(EVector3f(0.1f, 0.0f, 0.0f));
}


void SphereBehaviour::Update(float dt)
{
	if (m_preDrop)
	{
		EVector3f pos = m_physicsComp->GetPosition();
		pos.x += m_moveVec.x * dt;
		pos.y += m_moveVec.y * dt;
		pos.z += m_moveVec.z * dt;
		m_physicsComp->SetPosition(pos);
	}
}

void SphereBehaviour::Drop()
{
	m_preDrop = false;
	m_physicsComp->SetGravity(EVector3f(0.0f, -9.81f, 0.0f));
}

void SphereBehaviour::ApplyImpulse(EVector3f const& impulse)
{
	//if (!m_preDrop)
		m_physicsComp->ApplyImpulse(impulse);
}

void SphereBehaviour::Move(EVector3f const& move)
{
	m_moveVec = move;
}

EVector3f SphereBehaviour::ConvertColour(Colour colour)
{
	EVector3f colourVector = EVector3f(0.0f, 0.0f, 0.0f);
	switch (colour)
	{
	case Colour::RED:
		colourVector.x = 1.0f;
		break;

	case Colour::YELLOW:
		colourVector.x = 0.5f;
		colourVector.y = 0.5f;
		break;

	case Colour::GREEN:
		colourVector.y = 1.0f;
		break;

	case Colour::TEAL:
		colourVector.y = 0.5f;
		colourVector.z = 0.5f;
		break;

	case Colour::BLUE:
		colourVector.z = 1.0f;
		break;

	case Colour::MAGENTA:
		colourVector.x = 0.5f;
		colourVector.z = 0.5f;
		break;
	}

	return colourVector;
}

