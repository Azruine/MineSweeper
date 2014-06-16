#include <iostream>		//
#include <stdlib.h>		//
#include <time.h>		//time func
#include <windows.h>

struct Block
{
	bool isMine = false;
	int blockStatus = 0;
	//0: 가려짐 1: 클릭됨 2: 표시됨
};

int boolInt(bool tf)
{
	if (tf)
	{
		return 1;
	}
	else return 0;
};

int mineLocate(Block** arXY, int i, int j)
{
	if (i == 0 && j == 0)
	{
		return 0;
	}

	else if (i == 9 && j == 0)
	{
		return 1;
	}

	else if (i == 0 && j == 9)
	{
		return 2;
	}

	else if (i == 9 && j == 9)
	{
		return 3;
	}

	else if (i == 0)
	{
		return 4;
	}

	else if (i == 9)
	{
		return 5;
	}

	else if (j == 0)
	{
		return 6;
	}

	else if (j == 9)
	{
		return 7;
	}
	else
	{
		return 8;
	}
}

int mineSum(Block** arXY, int i, int j)
{
	switch (mineLocate(arXY, i, j))
	{
	default: return -1;

	case 0:return boolInt(arXY[1][0].isMine) + boolInt(arXY[0][1].isMine) + boolInt(arXY[1][1].isMine);

	case 1:return boolInt(arXY[9][0].isMine) + boolInt(arXY[8][1].isMine) + boolInt(arXY[9][1].isMine);

	case 2:return boolInt(arXY[1][8].isMine) + boolInt(arXY[0][9].isMine) + boolInt(arXY[1][9].isMine);

	case 3:return boolInt(arXY[9][8].isMine) + boolInt(arXY[8][9].isMine) + boolInt(arXY[9][9].isMine);

	case 4:return boolInt(arXY[0][j - 1].isMine) + boolInt(arXY[0][j + 1].isMine) + boolInt(arXY[1][j - 1].isMine) + boolInt(arXY[1][j].isMine) + boolInt(arXY[1][j + 1].isMine);

	case 5:return boolInt(arXY[9][j - 1].isMine) + boolInt(arXY[9][j + 1].isMine) + boolInt(arXY[8][j - 1].isMine) + boolInt(arXY[8][j].isMine) + boolInt(arXY[8][j + 1].isMine);

	case 6:return boolInt(arXY[i - 1][0].isMine) + boolInt(arXY[i + 1][0].isMine) + boolInt(arXY[i - 1][1].isMine) + boolInt(arXY[i][1].isMine) + boolInt(arXY[i + 1][1].isMine);

	case 7:return boolInt(arXY[i - 1][9].isMine) + boolInt(arXY[i + 1][9].isMine) + boolInt(arXY[i - 1][8].isMine) + boolInt(arXY[i][8].isMine) + boolInt(arXY[i + 1][8].isMine);

	case 8:return boolInt(arXY[i - 1][j - 1].isMine) + boolInt(arXY[i][j - 1].isMine) + boolInt(arXY[i + 1][j - 1].isMine) + boolInt(arXY[i + 1][j].isMine) + boolInt(arXY[i - 1][j + 1].isMine) + boolInt(arXY[i][j + 1].isMine) + boolInt(arXY[i + 1][j + 1].isMine) + boolInt(arXY[i - 1][j].isMine);		 
	}
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void gotomineLocate(int x, int y)
{
	COORD pos = { (x + 1) * 2, y + 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void nearbyBlockIsMineSunFunc(Block** arXY, int i, int j)
{
	gotomineLocate(i, j);
	std::cout << "□";
	nearbyBlockIsMine(arXY, i, j);
	return;
}

void nearbyBlockIsMine(Block** arXY, int i, int j)
{
	switch (mineLocate(arXY, i, j))
	{
	case 0:
	{
			  if (mineSum(arXY, 1, 0) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, 0);
			  }

			  if (mineSum(arXY, 0, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 0, 1);
			  }

			  if (mineSum(arXY, 1, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, 1);
			  }

			  return;
	}

	case 1:
	{
			  if (mineSum(arXY, 9, 0) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, 0);
			  }

			  if (mineSum(arXY, 8, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 8, 1);
			  }

			  if (mineSum(arXY, 9, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, 1);
			  }

			  return;
	}

	case 2:
	{
			  if (mineSum(arXY, 1, 8) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, 8);
			  }

			  if (mineSum(arXY, 0, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 0, 9);
			  }

			  if (mineSum(arXY, 1, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, 9);
			  }

			  return;
	}

	case 3:
	{
			  if (mineSum(arXY, 9, 8) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, 8);
			  }

			  if (mineSum(arXY, 8, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 8, 9);
			  }

			  if (mineSum(arXY, 9, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, 9);
			  }

			  return;
	}

	case 4:
	{
			  if (mineSum(arXY, 0, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 0, j - 1);
			  }

			  if (mineSum(arXY, 0, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 0, j + 1);
			  }

			  if (mineSum(arXY, 1, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, j - 1);
			  }

			  if (mineSum(arXY, 1, j) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, j);
			  }

			  if (mineSum(arXY, 1, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 1, j + 1);
			  }

			  return;
	}

	case 5:
	{
			  if (mineSum(arXY, 9, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, j - 1);
			  }

			  if (mineSum(arXY, 9, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 9, j + 1);
			  }

			  if (mineSum(arXY, 8, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 8, j - 1);
			  }

			  if (mineSum(arXY, 8, j) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 8, j);
			  }

			  if (mineSum(arXY, 8, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, 8, j + 1);
			  }

			  return;
	}

	case 6:
	{
			  if (mineSum(arXY, i - 1, 0) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, 0);
			  }

			  if (mineSum(arXY, i + 1, 0) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, 0);
			  }

			  if (mineSum(arXY, i - 1, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, 1);
			  }

			  if (mineSum(arXY, i, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i, 1);
			  }

			  if (mineSum(arXY, i + 1, 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, 1);
			  }

			  return;
	}

	case 7:
	{
			  if (mineSum(arXY, i + 1, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, 9);
			  }

			  if (mineSum(arXY, i - 1, 9) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, 9);
			  }

			  if (mineSum(arXY, i + 1, 8) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, 8);
			  }

			  if (mineSum(arXY, i, 8) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i, 8);
			  }

			  if (mineSum(arXY, i - 1, 8) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, 8);
			  }

			  return;
	}

	case 8:
	{
			  if (mineSum(arXY, i - 1, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, j - 1);
			  }

			  if (mineSum(arXY, i, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i, j - 1);
			  }

			  if (mineSum(arXY, i + 1, j - 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, j - 1);
			  }

			  if (mineSum(arXY, i - 1, j) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, j);
			  }

			  if (mineSum(arXY, i + 1, j) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, j);
			  }

			  if (mineSum(arXY, i - 1, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i - 1, j + 1);
			  }

			  if (mineSum(arXY, i, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i, j + 1);
			  }

			  if (mineSum(arXY, i + 1, j + 1) == 0)
			  {
				  nearbyBlockIsMineSunFunc(arXY, i + 1, j + 1);
			  }

			  return;
	}
	}

}

void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

int main(void)
{
	srand((unsigned int)time(NULL));	//난수

	int sizeX = 10, sizeY = 10;			//판 사이즈
	int i = 0, j = 0;
	int locat = 0;
	int sumMine = 0;
	int playX = 0, playY = 0, playMark = 0;
	int sum = 0;
	int mineCount = 20;
	bool ifPCMine = false;
	bool ifAMCleared = false;

	Block **arXY = new Block*[10];
	for (int i = 0; i < 10; ++i) 
	{
		arXY[i] = new Block[10];
	}

	bool count = true;



	while (sumMine <= mineCount)		//지뢰 생성
	{
		locat = rand() % 100;

		if (arXY[locat % 10][locat / 10].isMine)
		{
			continue;
		}

		arXY[locat % 10][(int)(locat / 10)].isMine = true;

		sumMine += 1;
	}

	setcolor(8, 0);

	std::cout << "┌──────────┐" << std::endl;
	for (int i = 0; i < 10; i++)
	{
		std::cout << "│   		      │" << std::endl;
	}
	std::cout << "└──────────┘" << std::endl;



	for (i = 0; i < 10; i++)
	{
		gotomineLocate(0, i);
		std::cout << "■■■■■■■■■■";

	}

	gotoxy(0, 13);

	std::cout << "X 좌표(0~9) Y 좌표(0~9) 마크 종류(0~2)" << std::endl;
	std::cout << "0: 블록 클릭 / 1: 블록 표시 / 2: 블록 표시 제거" << std::endl;
	//std::cout << "Help 입력시 자동 풀이" << std::endl;

	while (true)
	{
		std::cin >> playX;
		std::cin >> playY;
		std::cin >> playMark;

		if (playX > 9 || playX < 0 || playY > 9 || playY < 0 || playMark > 2 || playMark < 0)
		{
			gotoxy(0, 15);
			std::cout << "		";
			gotoxy(0, 15);
			continue;
		}

		if (arXY[playX][playY].blockStatus == 1)
		{
			gotoxy(0, 15);
			std::cout << "		";
			gotoxy(0, 15);
			continue;
		}

		gotomineLocate(playX, playY);

		if (playMark == 0)
		{

			if (arXY[playX][playY].isMine)
			{
				std::cout << "♂" << std::endl;
				gotoxy(0, 15);
				std::cout << "FAILED" << std::endl;
				break;
			}
			else
			{
				if (mineSum(arXY, playX, playY) != 0)
				{
					std::cout << mineSum(arXY, playX, playY) << " " << std::endl;
					arXY[playX][playY].blockStatus = 1;
				}

				else
				{
					std::cout << "□" << std::endl;
					nearbyBlockIsMine(arXY, playX, playY);
				}
			}
		}

		else if (playMark == 1)
		{
			std::cout << "★" << std::endl;
			arXY[playX][playY].blockStatus = 2;
		}

		else if (playMark == 2)
		{
			std::cout << "■" << std::endl;
			arXY[playX][playY].blockStatus = 0;
		}
		gotoxy(0, 15);
		std::cout << "		";
		gotoxy(0, 15);

		for (i = 0; i < 10; i++)
		{
			for (j = 0; j < 10; j++)
			{
				if (!arXY[i][j].isMine && arXY[i][j].blockStatus == 1)
				{
					sum += arXY[i][j].blockStatus;
				}
			}
		}

		if (sum == (100 - mineCount))
		{
			std::cout << "Cleared!" << std::endl;
			break;
		}

		else
		{
			sum = 0;
		}

	}

	for (int i = 0; i < 10; ++i)	//메모리 반환
	{
		delete[] arXY[i];
	}
	delete[] arXY;

	return 0;
}