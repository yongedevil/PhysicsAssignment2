#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "Helpers.h"
using namespace Helpers;
class Component;
class Entity 
{
public:

	Entity() {}
	~Entity() {}

	void Shutdown();

	void AddComponent(Component *component);

	void RemoveComponent(Component* component)
	{
		for(std::vector<Component*>::iterator it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if( (*it) == component)
			{
				it = m_Components.erase(it);
				break;
			}
		}
	}

	void Update(float deltaTime);
	void Render();

	EVector3f GetPosition() const {return m_Position;}
	void SetPosition(EVector3f pos) { m_Position = pos;}

private:
	std::vector<Component*> m_Components;
	EVector3f m_Position;
};


#endif