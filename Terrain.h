#ifndef TERRAIN_H_INCLUDED
#define TERRIAN_H_INCLUDED

using namespace std;
 
class TerrainClass
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
	
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
	unsigned int i;
	
public:
	
	vector<pointType> point;
	vector<relatedPointsType> relatedPoints;
	bool extra;
	
	TerrainClass()
	{
		
	}
	
	void update(InputClass Input)
	{
		int childCount = 2;
		if(Input.x < 30 && Input.y < 30)
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
		}
		
		else if(Input.x < 30 && Input.y < 60 && Input.y > 30)
		{
			point.clear();
			extra = false;
		}
		
		else if(Input.x < 30 && Input.y < 90 && Input.y > 60)
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
									continue;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			pointType var;
			var.x = Input.x;
			var.y = Input.y - Input.YPos;
			point.push_back(var);
		}
	}
	
	void render()
	{
		glColor3f(1, 0, 0);
		glPointSize(5);
		for(a = 0; a < point.size(); a++)
		{
			glBegin(GL_POINTS);
				glVertex2f(point[a].x, point[a].y);
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
		}
	}
};

#endif