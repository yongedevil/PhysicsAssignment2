#include "SphereBehaviour.h"
//#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"
//#include "btBulletDynamicsCommon.h"

void SphereBehaviour::Init(GraphicsComponent * graphicsComponent, PhysicsComponent * physicComponent, Colour colour, void (*endFunc)())
{
	m_graphicsComp = graphicsComponent;
	m_physicsComp = physicComponent;
	m_endFunc = endFunc;

	SetColour(colour);

	m_graphicsComp->SetColour(Behaviour::ConvertColour(colour));
	m_physicsComp->SetActivationState(0);
	m_physicsComp->SetBehaviour(this);
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
	m_physicsComp->SetActivationState(1);
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

void SphereBehaviour::OnCollision(Behaviour * other)
{
	Colour otherColour = other->GetColour();
	Colour thisColour = GetColour();

	if (otherColour == thisColour)
	{
		if (NULL != m_endFunc)
			m_endFunc();
	}
	else
	{
		m_physicsComp->ApplyImpulse(EVector3f(0.0f, 15.0f, 0.0f));
	}
}

void SphereBehaviour::Reset()
{
	btRigidBody * rbody = m_physicsComp->GetRigidBody();
	m_physicsComp->SetVelocity(EVector3f(0.0f, 0.0f, 0.0f));
	m_physicsComp->SetPosition(EVector3f(0.0f, 15.0f, 0.0f));
	m_physicsComp->SetActivationState(0);
	m_preDrop = true;
}


