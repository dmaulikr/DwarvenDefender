

class InputClass
{
private:
	int deltaY;
	int i;
public:
	int keyStates[400];
	int YPos;
	
	int x;
	int y;
	
	InputClass()
	{
		YPos = 0;
		for(i = 0; i <	300; i++)
		{
			keyStates[i] = 0;
		}
	}
	
	void update()
	{
		deltaY = 0;
		if(keyStates[(int)'w'] == 1)
		{
			deltaY += 1;
		}
		
		if(keyStates[(int)'s'] == 1)
		{
		  deltaY += -1;
		}
		
		YPos += deltaY;
	}
};