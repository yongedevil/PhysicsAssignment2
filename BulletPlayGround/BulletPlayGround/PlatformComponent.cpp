#include "PlatformComponent.h"
#include "PhysicsManager.h"
#include "btBulletDynamicsCommon.h"

void PlatformComponent::Update(float deltaTime)
{
	m_rigidBody->setGravity(btVector3(0,0,0));
//	//check displacement, adjust velocity
	PhysicsComponent::Update(deltaTime);
	
}

void PlatformComponent::SetVelocity(EVector3f velocity)
{
	m_Velocity = velocity;
	m_rigidBody->activate(true);
	m_rigidBody->setLinearVelocity(btVector3(m_Velocity.x,m_Velocity.y,m_Velocity.z));
}