#include <iostream>
#include <GL\include\glut.h>
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_opengl.h"
#include "PhysicsManager.h"
#include "Entity.h"
#include "Component.h"
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "Helpers.h"
#include "PlatformComponent.h"
#include "PlatformBehaviour.h"
#include "SphereBehaviour.h"

using namespace Helpers;

// Game Loop variables
bool game_running = true;

std::vector<Entity*> entities;
PhysicsManager* physicsManager = nullptr;

SphereBehaviour * sphere;
PlatformBehaviour * platform;
enum gameStage
{
	LEVEL1,
	LEVEL2,
	GAME_OVER
} curStage;

// Screen size
int screenWidth = 640;
int screenHeight = 480;

// Function prototypes
void InitGL();
void HandleEvents(SDL_Event* curEvent);
void Update(float dt);
void Render(SDL_Window* window);
PlatformBehaviour * AddPlatform();
SphereBehaviour * AddSphere();
void CreateObstical(EVector3f pos, float rotation, Colour colour);
void NextStage();
void CreateStage1();
void CreateStage2();
void CreateStageEnd();

float FRAMERATE = 1.0f/60.0f;

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Event curEvent;
	SDL_GLContext glContext;

	// Set our base SDL attributes.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create our SDL window.
	window = SDL_CreateWindow(	"Physics Playground",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!window)
	{
		std::cout << "Couldn't create window: " << SDL_GetError() << std::endl;
		return 0;
	}

	// Create our openGL context with our window.
	glContext = SDL_GL_CreateContext(window);

	if (!glContext)
	{
		std::cout << "Couldn't create context: " << SDL_GetError() << std::endl;
		return 0;
	}

	// Init openGL
	InitGL();

	physicsManager = new PhysicsManager();
	physicsManager->Init();
	//physicsManager->SetDebug(true);


	CreateStage1();


	
	DWORD prevTime = GetCurrentTime();

	// Main loop
	do
	{
		while (SDL_PollEvent(&curEvent))
		{
			HandleEvents(&curEvent);
		}
		//add a real frame rate call here
		DWORD currentTime = GetCurrentTime();
		float deltaTime = (float)currentTime - (float)prevTime;
		deltaTime *= 0.001;
		prevTime = currentTime;
		Update(deltaTime);
		
		Render(window);

	} while (game_running);


	for (std::vector<Entity*>::reverse_iterator it = entities.rbegin(); it != entities.rend(); ++it)
	{
		(*it)->Shutdown();
	}

	physicsManager->Shutdown();
	return 0;
}

void InitGL()
{
	glClearColor(1, 1, 1, 1);
	glViewport(0, 0, screenWidth, screenHeight);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

}

void HandleEvents(SDL_Event* curEvent)
{
	switch (curEvent->type)
	{
	case SDL_QUIT:
		game_running = false;
		break;
	case SDL_KEYDOWN:
		switch (curEvent->key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
			sphere->Move(EVector3f(-6.0f, 0.0f, 0.0f));
			break;

		case SDLK_d:
		case SDLK_RIGHT:
			sphere->Move(EVector3f(6.0f, 0.0f, 0.0f));
			break;

		case SDLK_s:
		case SDLK_DOWN:
			sphere->ApplyImpulse(EVector3f(0.0f, -1.0f, 0.0f));
			break;

		case SDLK_p:
			sphere->Reset();
			break;

		case SDLK_BACKSPACE:
			NextStage();
			break;

		case SDLK_SPACE:
			sphere->Drop();
			break;

		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (curEvent->key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
		case SDLK_d:
		case SDLK_RIGHT:
			sphere->Move(EVector3f(0.0f, 0.0f, 0.0f));
			break;

		case SDLK_ESCAPE:
			game_running = false;
			break;
		}
		break;
	default:
		break;
	}
}

void Update(float dt)
{
	physicsManager->Update(dt);
	for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->Update(dt);
	}
}

void Render(SDL_Window *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, screenWidth / screenHeight, 0.1f, 250);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Should be change to camera class or similar.
	gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);
   
	for(std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
        glPushMatrix();
		(*it)->Render();
        glPopMatrix();
	}

	SDL_GL_SwapWindow(window);
}


PlatformBehaviour * AddPlatform()
{
	Entity * ent = new Entity;
	EVector3f pos;
	pos.x = 0.0f;
	pos.y = 1.0f;
	pos.z = 0.0f;
	ent->SetPosition(pos);

	//platform componet
	PlatformComponent* pc = new PlatformComponent();
	pc->SetOwner(ent);
	pc->Init(PhysicsComponent::RBST_Plane);
	ent->AddComponent(pc);

	//graphics component
	GraphicsComponent* gc = new GraphicsComponent();
	gc->SetOwner(ent);
	gc->Init(GraphicsComponent::GST_Plane);
	ent->AddComponent(gc);

	//platform behaviour component
	PlatformBehaviour * pbc = new PlatformBehaviour();
	pbc->SetOwner(ent);
	pbc->Init(gc, pc, 2.0f, -15.0f, 15.0f);
	ent->AddComponent(pbc);


	entities.push_back(ent);
	return pbc;
}

SphereBehaviour * AddSphere()
{
	Entity * ent = new Entity();
	EVector3f pos;
	pos.x = 0.0f;
	pos.y = 15.0f;
	pos.z = 0.0f;
	ent->SetPosition(pos);

	//physics Component
	PhysicsComponent* pc = new PhysicsComponent();
	pc->SetOwner(ent);
	pc->Init(PhysicsComponent::RBST_Sphere);
	ent->AddComponent(pc);

	//graphics Component
	GraphicsComponent* gc = new GraphicsComponent();
	gc->SetOwner(ent);
	gc->Init(GraphicsComponent::GST_Sphere);
	gc->SetRadius(0.5f);
	ent->AddComponent(gc);

	//sphere behaviour component
	SphereBehaviour * sbc = new SphereBehaviour();
	sbc->SetOwner(ent);
	sbc->Init(gc, pc, Colour::BLUE, NextStage);
	ent->AddComponent(sbc);


	entities.push_back(ent);
	return sbc;
}

void CreateObstical(EVector3f pos, float rotation, Colour colour)
{
	Entity * ent = new Entity();
	ent->SetPosition(pos);

	//platform componet
	PlatformComponent* pc = new PlatformComponent();
	pc->SetOwner(ent);
	pc->Init(PhysicsComponent::RBST_Plane);
	pc->SetInverseMass(0);
	pc->SetRotation(rotation);
	ent->AddComponent(pc);

	//graphics component
	GraphicsComponent* gc = new GraphicsComponent();
	gc->SetOwner(ent);
	gc->Init(GraphicsComponent::GST_Plane);
	gc->SetColour(Behaviour::ConvertColour(colour));
	gc->SetRotation(rotation);
	ent->AddComponent(gc);

	entities.push_back(ent);
}

void NextStage()
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		(*it)->Shutdown();
	}
	entities.clear();

	switch (curStage)
	{
	case gameStage::LEVEL1:
		CreateStage2();
		break;

	case gameStage::LEVEL2:
		CreateStageEnd();
		break;

	case gameStage::GAME_OVER:
		CreateStage1();
		break;
	}
}

void CreateStage1()
{
	platform = AddPlatform();
	sphere = AddSphere();
	curStage = gameStage::LEVEL1;
}

void CreateStage2()
{
	platform = AddPlatform();
	sphere = AddSphere();

	CreateObstical(EVector3f(1, 5, 0), 45, Colour::GREEN);
	CreateObstical(EVector3f(-4, 10, 0), 6, Colour::RED);
	CreateObstical(EVector3f(10, 8, 0), 55, Colour::TEAL);

	curStage = gameStage::LEVEL2;
}

void CreateStageEnd()
{
	curStage = gameStage::GAME_OVER;
}
