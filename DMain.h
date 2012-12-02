#ifndef DMAIN_H_INCLUDED
#define DMAIN_H_INCLUDED

#define PI 3.14159265

#include <SDL/SDL.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <math.h>

#include "DEvent.h"
#include "Intersections.h"

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

  	//Uses a floor point and the data to another floor point to find an angle.
	float ConvertToAngle (pointType* center, int space, bool listP)
	{
	    float x, y;

	    //True means it is comparing to a parent, false to a child.
	    if (listP)
	    {
	        x = center->parent[space]->x - center->x;
	        y = center->parent[space]->y - center->y;
	    }
	    else
	    {
	        x = center->children[space]->x - center->x;
	        y = center->children[space]->y - center->y;
	    }

	    float angle = atan2 (x, y);

	    //Moves the angle from [-PI,+PI] to [0,2PI). I think the notation is right.
	    //if (angle < 0) angle = angle + 2*PI;

	    return angle = 0;
	}

	//Moves a wall point based on an angle and distance.
	void ConvertToCoordinates (wallPointsType* wall, float angle, float distance)
	{
	    //cos & x
	    wall->x = wall->x + (cos(angle) * distance);
	    //sin & y
	    wall->y = wall->y + (sin(angle) * distance);
	}

	//Finds all the wall points around a floor point.
	void IntersectionWalls (pointType* center)
	{
        //First the parent and child points are sorted by angle.
        vector<relatedPointsType> relatedWalls;
        relatedWalls.clear();
        relatedWalls.resize(center->children.size() + center->parent.size());

        for (unsigned int n = 0 ; n < center->parent.size() ; n++)
        {
            relatedWalls[n].pointLookup = center->parent[n];
            relatedWalls[n].length = ConvertToAngle(center, n, true);
        }

        for (unsigned int n = 0 ; n < center->children.size() ; n++)
        {
            relatedWalls[n + center->parent.size()].pointLookup = center->children[n];
            relatedWalls[n + center->parent.size()].length = ConvertToAngle(center, n, false);
        }

        bubbleSort2(relatedWalls);

        int first = relatedWalls.size();
        int second = 0;
        //0 first close, 1 second close, 2 first far, 3 second far.
        wallPointsType tempWallPoints [4];
        float firstAngle;
        float secondAngle;

        //Now it finds the wall points, going counderclockwise.
        for (unsigned int n = 0 ; n < relatedWalls.size() ; n++)
        {
            //The four tempWallPoints are set (placed on a floor point then sifted).
            tempWallPoints[0].x = tempWallPoints[1].x = center->x;
            tempWallPoints[0].y = tempWallPoints[1].y = center->y;
            tempWallPoints[2].x = relatedWalls[first].pointLookup->x;
            tempWallPoints[2].y = relatedWalls[first].pointLookup->y;
            tempWallPoints[3].x = relatedWalls[second].pointLookup->x;
            tempWallPoints[3].y = relatedWalls[second].pointLookup->y;

            firstAngle = relatedWalls[first].length + PI/2;
            secondAngle = relatedWalls[second].length - PI/2;

            if (firstAngle > PI) firstAngle = firstAngle - 2*PI;
            if (firstAngle < -PI) firstAngle = firstAngle + 2*PI;
            if (secondAngle > PI) secondAngle = secondAngle - 2*PI;
            if (secondAngle < -PI) secondAngle = secondAngle + 2*PI;

            //The third value in ConvertToCoordinates {20} is the intersection's radius.
            ConvertToCoordinates (&tempWallPoints[0], firstAngle, 20);
            ConvertToCoordinates (&tempWallPoints[1], secondAngle, 20);
            ConvertToCoordinates (&tempWallPoints[2], firstAngle, 20);
            ConvertToCoordinates (&tempWallPoints[3], secondAngle, 20);

            if (checkIntersection(tempWallPoints[0].x, tempWallPoints[1].x,
                                   tempWallPoints[2].x, tempWallPoints[3].x,
                                    tempWallPoints[0].y, tempWallPoints[1].y,
                                     tempWallPoints[2].y, tempWallPoints[3].y))
            {
                //If there is an intersection it adds the intersection as a wall point.
                //Note to self: make a FindIntersection() in Intersection.h.
            }
            else
            {
                //If there is no intersection it adds the two close wall points.
                center->wallPoints.push_back (tempWallPoints[0]);
                center->wallPoints.push_back (tempWallPoints[1]);
            }

            first = n;
            second = n + 1;
        }
	}

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
		int c = 0;
		int d = 0;
		int i = 0;
		bool hit = false;
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

			/*//Calculate wall points.
            for (unsigned int n = 0 ; n < point.size() ; n++)
            {
                //A crash guard here.
                if (point[n].parent.size() > 0 && point[n].children.size() > 0)
                    IntersectionWalls(&point[n]);
            }
            */
		}

		else if(mX < 30 && mY < 60 && mY > 30)
		{
			point.clear();
			extra = false;
		}

		else if(mX < 30 && mY < 90 && mY > 60)
		{
			for(a = 0; a < point.size(); a++)
			{
				for(b = 0; b < point[a].children.size(); b++)
				{
					for(c = 0; c < point.size(); c++)
					{
						if(c == a)
						{
							continue;
						}

						else
						{
							for(d = 0; d < point[c].children.size(); d++)
							{
								if(checkIntersection(point[a].x, point[a].children[b]->x, point[c].x, point[c].children[d]->x, point[a].y, point[a].children[b]->y, point[c].y, point[c].children[d]->y))
								{
									point[c].children.erase(point[c].children.begin() + d);
									hit = true;
									continue;
								}
							}
						}
						if(hit)
						{
							hit = false;
							continue;
						}
					}
				}
			}
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

		glColor3f(1, 0.5, 0);
		glBegin(GL_QUADS);
			glVertex2f(0, 60);
			glVertex2f(30, 60);
			glVertex2f(30, 90);
			glVertex2f(0, 90);
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

			glColor3f(0, 1, 0);
			glPointSize(2);
			for(a = 0; a < point.size(); a++)
			{
				if (point[a].wallPoints.size() != 0)
				{
                    for(b = 0; b < point[a].wallPoints.size(); b++)
                    {
                        glBegin(GL_POINTS);
                            glVertex2f(point[a].wallPoints[b].x, point[a].wallPoints[b].y);
                        glEnd();
                    }
				}
			}

			glPointSize(1);
		}

    SDL_GL_SwapBuffers();
	}

  void OnCleanUp()
  {
		SDL_Quit ();
  }
};

#endif // DMAIN_H_INCLUDED
