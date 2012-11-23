#ifndef DMAIN_H_INCLUDED
#define DMAIN_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <math.h>

#include "DEvent.h"

using namespace std;

class DMain : public DEvent
{
private:
	struct wallPointsType
	{
		int x;
		int y;
	};
	
	struct pointType
	{
		int x;
		int y;
		vector<pointType*> children;
		vector<pointType*> parent;
		vector<wallPointsType> wallPoints;
	};
	
	struct relatedPointsType
	{
		float length;
		pointType* pointLookup;
	};
	
	void bubbleSort(vector<pointType> &vec)
	{
		int i;
		int j;
		bool swapMade = true;
		pointType temp;
		int length = vec.size();
		while(swapMade)
		{
			swapMade = false;
			for(j = 0; j < (length -1); j++)
			{
				if(vec[j+1].y > vec[j].y)
				{
					temp = vec[j];
					vec[j] = vec[j + 1];
					vec[j + 1] = temp;
					swapMade = true;
				}
			}
		}
		return;
	}
	
	void bubbleSort2(vector<relatedPointsType> &vec)
	{
		int i;
		int j;
		bool swapMade = true;
		relatedPointsType temp;
		int length = vec.size();
		while(swapMade)
		{
			swapMade = false;
			for(j = 0; j < (length -1); j++)
			{				
				if(vec[j + 1].length < vec[j].length)
				{
					temp = vec[j];
					vec[j] = vec[j + 1];
					vec[j + 1] = temp;
					swapMade = true;
				}
			}
		}
		return;
	}
	
	bool running;
	vector<pointType> point;
	vector<relatedPointsType> relatedPoints;
	bool extra;
  SDL_Surface* surfDisplay;
	
public:
	int childCount;
	DMain();
	int OnExecute();

	bool OnInit()
	{
		extra = false;
		int width = 1366;
		int height = 768;
		int bpp = 0;
		int flags = 0;

		const SDL_VideoInfo* info = NULL;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			return false;
		}
        
		info = SDL_GetVideoInfo();

		if(!info) 
		{
			fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		}

		bpp = info->vfmt->BitsPerPixel;

    if((surfDisplay = SDL_SetVideoMode (width, height, 32, SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
    {
			return false;
    }
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, 0, 1);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef (0.5, 0.5, 0);

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
		if((int)key == 27)
    {
			running = false;
    }
  }
  
  void OnLButtonDown(int mX, int mY)
	{
		int childCount = 2;
		int a = 0;
		int b = 0;
		int i = 0;
		if(mX < 30 && mY < 30)
		{
			extra = true;
			bubbleSort(point);
			
			for(a = 0; a < point.size(); a++)
			{
				childCount = 2;
				relatedPoints.clear();
				relatedPoints.resize(point.size() - (a + 1));
				
				i = 0;
				for(b = (a + 1); b < point.size(); b++)
				{
					relatedPoints[i].length = sqrt((pow(point[b].x - point[a].x, 2) + pow(point[b].y - point[a].y, 2)));
					relatedPoints[i].pointLookup = &point[b];
					i++;
				}
				
				bubbleSort2(relatedPoints);
				point[a].children.resize(childCount);
				
				for(b = 0; b < point[a].children.size(); b++)
				{
					point[a].children[b] = relatedPoints[b].pointLookup;
				}
			}
			
			for(a = 0; a < point.size(); a++)
			{
				for(b = 0; b < point[a].children.size(); b++)
				{
					point[a].children[b]->parent.push_back(&point[a]);
				}
			}
			//calculate wall points.
		}
		
		else if(mX < 30 && mY < 60 && mY > 30)
		{
			point.clear();
			extra = false;
		}
		
		else
		{
			pointType var;
			var.x = mX;
			var.y = mY;
			point.push_back(var);
		}
	}

  void OnLoop()
  {

  }

  void OnRender()
  {
		int a = 0;
		int b = 0;
		int i =0;
		glClearColor(0, 0, 1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

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
		
		glColor3f(1, 0, 0);
		glPointSize(5);
		for(i = 0; i < point.size(); i++)
		{
			glBegin(GL_POINTS);
				glVertex2f(point[i].x, point[i].y);
			glEnd();
		}
		glPointSize(1);
		
		if(extra)
		{
			for(a = 0; a < point.size(); a++)
			{
				for(b = 0; b < point[a].children.size(); b++)
				{
					glBegin(GL_LINES);
						glVertex2f(point[a].x, point[a].y);
						glVertex2f(point[a].children[b]->x, point[a].children[b]->y);
					glEnd();
				}
			}
			
			/*
			glcolor3f(0, 1, 0);
			glPointSize(2);
			for(a = 0; a < point.size(); a++)
			{
				for(b = 0; b < point[a].relatedPoints.size(); b++)
				{
					glBegin(GL_POINTS);
						glVertex2f(point[a].wallPoints[b].x, point[a].wallPoints[b].y);
					glEnd();
				}
			}
			glPointSize(1);
			*/
		}
		
    SDL_GL_SwapBuffers();
	}

  void OnCleanUp()
  {
		SDL_Quit ();
  }
};

#endif // DMAIN_H_INCLUDED
