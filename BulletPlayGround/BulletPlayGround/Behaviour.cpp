#include "Behaviour.h"

EVector3f Behaviour::ConvertColour(Colour colour)
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

void Behaviour::OnCollision(Behaviour * other)
{

}