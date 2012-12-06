#ifndef DMAIN_H_INCLUDED
#define DMAIN_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/glu.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

#include "DEvent.h"
#include "Intersections.h"
#include "Input.h"
#include "TextureManagement.h"
#include "Terrain.h"
#include "Character.h"

using namespace std;

class DMain : public DEvent
{
private:
	bool running;
  SDL_Surface* surfDisplay;
	
public:
	DMain();
	int OnExecute();
	InputClass Input;
	TerrainClass Terrain;
	CharacterClass dwarf1;
	
	bool OnInit()
	{
		int Width = 1366;
		int Height = 768;
		int bpp = 0;
		int flags = 0;

		const SDL_VideoInfo* info = NULL;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			return false;
		}
        
		info = SDL_GetVideoInfo();
		bpp = info->vfmt->BitsPerPixel;

    if((surfDisplay = SDL_SetVideoMode (Width, Height, 32, SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
    {
			return false;
    }
		
    glEnable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, Width, Height, 0, 0, 1);
		glTranslatef (0.5, 0.5, 0);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
    return true;
	}

  void OnEvent(SDL_Event* Event)
  {
		DEvent::OnEvent(Event);
  }

  void OnExit()
  {
		running = false;
  }

  void OnKeyDown(SDLKey key, SDLMod mod, Uint16 unicode)
  {
		Input.keyStates[(int)key] = 1;
		
		if((int)key == 27)
    {
			running = false;
    }
  }
  
  void OnKeyUp(SDLKey key, SDLMod mod, Uint16 unicode)
	{
		Input.keyStates[(int)key] = 0;
	}
  
  void OnLButtonDown(int mX, int mY)
	{
		Input.x = mX;
		Input.y = mY;
	}

  void OnLoop()
  {
		Terrain.update(Input);
		Input.update();
		dwarf1.update();
  }

  void OnRender()
  {
		glClearColor(0, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glColor3f(0, 1, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 0);
			glVertex2f(30, 0);
			glVertex2f(30, 30);
			glVertex2f(0, 30);
		glEnd();

		glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 30);
			glVertex2f(30, 30);
			glVertex2f(30, 60);
			glVertex2f(0, 60);
		glEnd();
		
		glColor3f(1, 0.5, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 60);
			glVertex2f(30, 60);
			glVertex2f(30, 90);
			glVertex2f(0, 90);
		glEnd();
		
		glTranslatef(0, Input.YPos, 0);
		
		Terrain.render();
		dwarf1.render();
		
    SDL_GL_SwapBuffers();
	}

  void OnCleanUp()
  {
		SDL_Quit();
  }
};

#endif // DMAIN_H_INCLUDED
