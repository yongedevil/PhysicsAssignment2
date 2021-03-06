#include "PhysicsComponent.h"
#include "PhysicsManager.h"
#include "btBulletDynamicsCommon.h"
#include <iostream>
#include "Entity.h"
#include "Behaviour.h"

#include <cmath>

using namespace std;
using namespace Helpers;

void PhysicsComponent::Init(RigidBodyShapeTypes type)
{
	Component::Init();
	m_shapeType = type;
	switch (type)
	{
	case PhysicsComponent::RBST_Plane:
		CreatePlatform();
		break;
	case PhysicsComponent::RBST_Sphere:
		CreateSphere();
		break;
	default:
		break;
	}
}

void PhysicsComponent::CreateSphere()
{
	btCollisionShape* sphereShape = new btSphereShape(1.0f);
	
	Entity* entOwner = GetOwner();
	EVector3f pos = entOwner->GetPosition();

	btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));

	btScalar mass = 1;
        btVector3 fallInertia(0, 0, 0);
        sphereShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, sphereShape, fallInertia);
        m_rigidBody = new btRigidBody(fallRigidBodyCI);

		m_rigidBody->setUserPointer(this);
		//add physicsbody to world
		PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::CreatePlane()
{
	btCollisionShape* planeShape = new btStaticPlaneShape(btVector3(0,1,0),0.0f);
	

	btDefaultMotionState* planeMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	

	btRigidBody::btRigidBodyConstructionInfo
                planeRigidBodyCI(0, planeMotionState, planeShape);

    btRigidBody* planeRigidBody = new btRigidBody(planeRigidBodyCI);

	m_rigidBody = planeRigidBody;

	m_rigidBody->setUserPointer(this);
	//add physicsbody to world
		PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::CreatePlatform()
{
	btCollisionShape* boxShape = new btBoxShape(btVector3(0.5,0.5,0.5));
	
	Entity* entOwner = GetOwner();
	EVector3f pos = entOwner->GetPosition();

	btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));

	
	btScalar mass = 1;
        btVector3 fallInertia(0, 0, 0);
        boxShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo
                boxRigidBodyCI(mass, fallMotionState, boxShape,fallInertia);

    btRigidBody* boxRigidBody = new btRigidBody(boxRigidBodyCI);

	m_rigidBody = boxRigidBody;
	m_rigidBody->setUserPointer(this);
	//add physicsbody to world
		PhysicsManager::GetInstance()->AddPhysicsComponent(this);
}

void PhysicsComponent::Shutdown()
{
	//remove rigidBody from physics world
	PhysicsManager::GetInstance()->RemovePhysicsComponent(this);
}

void PhysicsComponent::Update(float DeltaTime)
{
	btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();
	EVector3f pos;
	pos.x = worldPos.getX();
	pos.y = worldPos.getY();
	pos.z = worldPos.getZ();

	m_Owner->SetPosition(pos);

	if(PhysicsManager::GetInstance()->IsDebugOn())
	{
		DebugPrint();
	}
}

void PhysicsComponent::OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider)
{
	Entity* ent = collider->GetOwner();
	if(ent)
	{
		btRigidBody* rigidBody = collider->GetRigidBody();
		float mass = rigidBody->getInvMass();

		if(m_behaviour && collider->GetBehaviour())
		{
			m_behaviour->OnCollision(collider->GetBehaviour());
		}
	}
}

void PhysicsComponent::OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex)
{
	const btCollisionObject* colObj = collidedObjWrap->getCollisionObject();
	PhysicsComponent* physComp = static_cast<PhysicsComponent*>(colObj->getUserPointer());
	if(physComp)
	{
		Entity* entOwner = physComp->GetOwner();
		//entOwner->OnHit();
	}
}

void PhysicsComponent::DebugPrint()
{
	if(m_rigidBody && m_shapeType == RBST_Sphere)
	{
		btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();
		btVector3 gravity = m_rigidBody->getGravity();
		cout << "Rigid Body Position: (";
		cout << worldPos.getX() << ", ";
		cout << worldPos.getY() << ", ";
		cout << worldPos.getZ() << ") | ";

		cout << "Gravity: (";
		cout << gravity.getX() << ", ";
		cout << gravity.getY() << ", ";
		cout << gravity.getZ() << ")" << endl;

		if (std::abs(worldPos.getX() - 0.205028f) < 0.000001f)
		{
			cout << " END" << endl;
		}
	}
}

void PhysicsComponent::SetGravity(EVector3f const& gravity)
{
	btVector3 btGravity = btVector3(gravity.x, gravity.y, gravity.z);
	m_rigidBody->setGravity(btGravity);
}

EVector3f PhysicsComponent::GetGravity() const
{
	btVector3 btGravity = m_rigidBody->getGravity();
	return EVector3f(btGravity.x(), btGravity.y(), btGravity.z());
}

void PhysicsComponent::SetPosition(EVector3f const& pos)
{
	btTransform trans = m_rigidBody->getWorldTransform();
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	m_rigidBody->setWorldTransform(trans);
}

EVector3f PhysicsComponent::GetPosition() const
{
	btVector3 btPosition = m_rigidBody->getWorldTransform().getOrigin();
	return EVector3f(btPosition.x(), btPosition.y(), btPosition.z());
}
void PhysicsComponent::SetInverseMass(float inverseMass)
{
	float mass;
	if (0 != inverseMass)
		mass = 1 / inverseMass;
	else
		mass = 0;
	m_rigidBody->setMassProps(mass, btVector3(0, 0, 0));
}

float PhysicsComponent::GetInverseMass() const
{
	return m_rigidBody->getInvMass();
}

void PhysicsComponent::SetVelocity(EVector3f const& vel)
{
	m_rigidBody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}
EVector3f PhysicsComponent::GetVelocity() const
{
	btVector3 btVelocity = m_rigidBody->getLinearVelocity();
	return EVector3f(btVelocity.getX(), btVelocity.getY(), btVelocity.getZ());
}

void PhysicsComponent::ApplyImpulse(EVector3f const& impulse)
{
	m_rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
}

void PhysicsComponent::SetActivationState(int state)
{
	m_rigidBody->setActivationState(state);
}

int PhysicsComponent::GetActivationState() const
{
	return m_rigidBody->getActivationState();
}

void PhysicsComponent::SetBehaviour(Behaviour * behaviour)
{
	m_behaviour = behaviour;
}

Behaviour * PhysicsComponent::GetBehaviour() const
{
	return m_behaviour;
}
void PhysicsComponent::SetRotation(float rotation)
{
	btTransform trans = m_rigidBody->getWorldTransform();
	trans.setRotation(btQuaternion(btVector3(0, 0, 1), rotation * 3.14159265359 / 180.0));
	m_rigidBody->setWorldTransform(trans);
}

float PhysicsComponent::GetRotation() const
{
	btQuaternion btRotation = m_rigidBody->getWorldTransform().getRotation();
	return btRotation.getAngle() * 180.0 / 3.14159265359;
}