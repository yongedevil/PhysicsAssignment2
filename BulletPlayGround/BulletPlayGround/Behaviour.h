#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Colour.h"
#include "Component.h"
#include "Helpers.h"

class Behaviour : public Component
{
private:
	Colour m_colour;

public:
	Behaviour() : m_colour(Colour::RED) { }
	virtual ~Behaviour() { }

	Colour GetColour() const { return m_colour; }
	void SetColour(Colour colour) { m_colour = colour; }

	virtual void OnCollision(Behaviour * other);

	static EVector3f ConvertColour(Colour colour);
};

#endif