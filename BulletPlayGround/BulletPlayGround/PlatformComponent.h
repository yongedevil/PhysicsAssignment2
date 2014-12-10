#ifndef PLATFORMCOMPONENT_H
#define PLATFORMCOMPONENT_H

#include "PhysicsComponent.h"

class PlatformComponent : public PhysicsComponent
{
public:
	PlatformComponent() {}
	virtual ~PlatformComponent() {}

	virtual void Update(float deltaTime);

	EVector3f GetVelocity() const {return m_Velocity;}
	void SetVelocity(EVector3f velocity); 

	EVector3f GetMaxDisplacement() const {return m_MaxDisplacement;}
	void SetMaxDisplacement(EVector3f velocity) {m_MaxDisplacement = velocity;}

protected:
	EVector3f m_Velocity;
	EVector3f m_MaxDisplacement;
	EVector3f m_CurrentDisplacement;
private:

};


#endif