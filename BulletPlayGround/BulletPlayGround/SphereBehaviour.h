#ifndef SPHERE_BEHAVIOUR_H
#define SPHERE_BEHAVIOUR_H

#include "Colour.h"
#include "Component.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Behaviour.h"

class SphereBehaviour : public Behaviour
{
private:
	bool m_preDrop;
	EVector3f m_moveVec;
	Colour m_colour;
	EVector3f m_colourVector;
	GraphicsComponent * m_graphicsComp;
	PhysicsComponent * m_physicsComp;

public:
	SphereBehaviour() : m_graphicsComp(0), m_physicsComp(0), m_colour(Colour::RED), m_preDrop(true), m_moveVec(0, 0, 0) { }
	~SphereBehaviour() { }

	void Init(GraphicsComponent * graphicsComponent, PhysicsComponent * physicComponent, Colour colour);
	void Shutdown() { }

	void Update(float dt);

	Colour GetColour() const { return m_colour; }

	void Drop();
	void ApplyImpulse(EVector3f const& impulse);
	void Move(EVector3f const& move);

	void OnCollision(Behaviour * other);

	void Reset();
};

#endif