#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Component.h"
#include "../BulletCollision/CollisionDispatch/btCollisionWorld.h"

class btRigidBody;

class PhysicsComponent : public Component
{
public:

	enum RigidBodyShapeTypes { RBST_Plane = 0, RBST_Sphere };

	PhysicsComponent() : m_shapeType(RBST_Plane) {}
	virtual ~PhysicsComponent() {}

	void Init(RigidBodyShapeTypes type);
	void Shutdown();

	virtual void Update(float deltaTime);

	virtual void OnContactProcCallback(btManifoldPoint& cp, PhysicsComponent* collider);
	virtual void OnAddSingleResult(btManifoldPoint& cp,int partId0,int index0,const btCollisionObjectWrapper* collidedObjWrap,int collidedObjPartId,int collidedObjIndex);

	void DebugPrint();

	btRigidBody* GetRigidBody() const {return m_rigidBody;}

	void SetShapeType(RigidBodyShapeTypes type) { m_shapeType = type;}
	RigidBodyShapeTypes GetShapeType() const { return m_shapeType; }

	void SetGravity(EVector3f const& gravity);
	EVector3f GetGravity() const;

	void SetPosition(EVector3f const& pos);
	EVector3f GetPosition() const;

	void ApplyImpulse(EVector3f const& impulse);

private:
	void CreatePlane();
	void CreateSphere();
	void CreatePlatform();
protected:
	//physics variables
	btRigidBody* m_rigidBody;
	RigidBodyShapeTypes m_shapeType;
};

#endif