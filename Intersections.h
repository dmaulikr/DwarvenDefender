float checkParallel(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4)
{
	return (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
}

void bubbleSortInter(int (&cords)[2])
{
	int j;
	bool swapMade = true;
	int temp;
	
	while(swapMade)
	{
		swapMade = false;
		for(j = 0; j < 3; j++)
		{
			if(cords[j + 1] > cords[j])
			{
				temp = cords[j];
				cords[j] = cords[j + 1];
				cords[j + 1] = temp;
				swapMade = true;
			}
		}
	}
	return;
}

bool checkIntersection(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4)
{
	float intersectX;
	float intersectY;
	
	int xCords1[2];
	xCords1[0] = x1;
	xCords1[1] = x2;
	bubbleSortInter(xCords1);
	
	int xCords2[2];
	xCords2[0] = x3;
	xCords2[1] = x4;
	bubbleSortInter(xCords2);
	
	int yCords1[2];
	yCords1[0] = y1;
	yCords1[1] = y2;
	bubbleSortInter(yCords1);
	
	int yCords2[2];
	yCords2[0] = y3;
	yCords2[1] = y4;
	bubbleSortInter(yCords2);
	
	float denom = checkParallel(x1, x2, x3, x4, y1, y2, y3, y4);
	
	if(denom != 0)
	{
		intersectX = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))/denom;
		intersectY = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4))/denom;
	}
	else
	{
		return false;
	}
	
	if((intersectX < xCords1[0] && intersectX > xCords1[1] && intersectY < yCords1[0] && intersectY > yCords1[1]) && (intersectX < xCords2[0] && intersectX > xCords2[1] && intersectY < yCords2[0] && intersectY > yCords2[1]))
	{
		return true;
	}
	else
	{
		return false;
	}
}