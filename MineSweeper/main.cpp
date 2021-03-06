#include <iostream>		
#include <stdlib.h>		
#include <time.h>		//time func
#include <windows.h>
#include <conio.h>		//kbhit func
#include <wincon.h>

#define ESC 27
#define UP (256 + 72)
#define DOWN (256 + 80)
#define LEFT (256 + 75)
#define RIGHT (256 + 77)
#define A 65
#define S 83
#define D 68
#define a 97
#define s 115
#define d 100

struct Block
{
	bool isMine = false;
	int blockStatus = 0;
	//0: 가려짐 1: 클릭됨 2: 표시됨

	enum EBlockStatus		//편의를 위한 enum 선언
	{	
		hidden,
		clicked,
		checked
	};
};

int mineLocate(Block** arXY, int i, int j, int x, int y)	//입력받은 좌표가 어떤 경우인지 값을 반환함. 제일 구석, 변, 내부일때 계산식이 전부 달라지므로 편의를 위해 넣음
{
	if (i == 0 && j == 0)
	{
		return 0;
	}

	else if (i == x - 1 && j == 0)
	{
		return 1;
	}

	else if (i == 0 && j == y - 1)
	{
		return 2;
	}

	else if (i == x - 1 && j == y - 1)
	{
		return 3;
	}

	else if (i == 0)
	{
		return 4;
	}

	else if (i == x - 1)
	{
		return 5;
	}

	else if (j == 0)
	{
		return 6;
	}

	else if (j == y - 1)
	{
		return 7;
	}
	else
	{
		return 8;
	}
}

int mineSum(Block** arXY, int i, int j, int x, int y)		//주변 지뢰의 개수를 반환
{
	switch (mineLocate(arXY, i, j, x, y))
	{
	default: return -1;

	case 0:return (arXY[1][0].isMine) + (arXY[0][1].isMine) + (arXY[1][1].isMine);

	case 1:return (arXY[x-1][0].isMine) + (arXY[x-2][1].isMine) + (arXY[x-1][1].isMine);

	case 2:return (arXY[1][y-2].isMine) + (arXY[0][y-1].isMine) + (arXY[1][y-1].isMine);

	case 3:return (arXY[x-1][y-2].isMine) + (arXY[x-2][y-1].isMine) + (arXY[x-1][y-1].isMine);

	case 4:return (arXY[0][j - 1].isMine) + (arXY[0][j + 1].isMine) + (arXY[1][j - 1].isMine) + (arXY[1][j].isMine) + (arXY[1][j + 1].isMine);

	case 5:return (arXY[x-1][j - 1].isMine) + (arXY[x-1][j + 1].isMine) + (arXY[x-2][j - 1].isMine) + (arXY[x-2][j].isMine) + (arXY[x-2][j + 1].isMine);

	case 6:return (arXY[i - 1][0].isMine) + (arXY[i + 1][0].isMine) + (arXY[i - 1][1].isMine) + (arXY[i][1].isMine) + (arXY[i + 1][1].isMine);

	case 7:return (arXY[i - 1][y-1].isMine) + (arXY[i + 1][y-1].isMine) + (arXY[i - 1][y-2].isMine) + (arXY[i][y-2].isMine) + (arXY[i + 1][y-2].isMine);

	case 8:return (arXY[i - 1][j - 1].isMine) + (arXY[i][j - 1].isMine) + (arXY[i + 1][j - 1].isMine) + (arXY[i + 1][j].isMine) + (arXY[i - 1][j + 1].isMine) + (arXY[i][j + 1].isMine) + (arXY[i + 1][j + 1].isMine) + (arXY[i - 1][j].isMine);		 
	}
}

void gotomineLocate(int x, int y)		//gotoxy 함수를 약간 변형함.
{
	COORD pos = { (x + 1) * 2, y + 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void nearbyBlockIsMine(Block** arXY, int i, int j, int x, int y);		//아래의 두 함수에서 재귀함수를 사용하기 위해 미리 선언함

void nearbyBlockIsMineSunFuncZero(Block** arXY, int i, int j, int x, int y)		//주어진 칸에 인접한 지뢰가 0개일때 호출하는 함수
{
	gotomineLocate(i, j);
	std::cout << "□";
	nearbyBlockIsMine(arXY, i, j, x, y);
	return;
}

void nearbyBlockIsMineSunFuncNotZero(Block** arXY, int i, int j, int x, int y)	//주어진 칸에 인접한 지뢰가 0개가 아닐때 호출하는 함수
{
	gotomineLocate(i, j);
	arXY[i][j].blockStatus = Block::clicked;
	std::cout << mineSum(arXY, i, j, x, y);
	return;
}

void nearbyBlockIsMineSunFunc(Block** arXY, int i, int j, int x, int y)			//위 두 함수를 묶는 함수. 코드의 절약을 위하여 함수로 선언함.
{
	if (mineSum(arXY, i, j, x, y) == 0)
	{
		nearbyBlockIsMineSunFuncZero(arXY, i, j, x, y);
	}

	else
	{
		nearbyBlockIsMineSunFuncNotZero(arXY, i, j, x, y);
	}

	return;
}

void nearbyBlockIsMine(Block** arXY, int i, int j, int x, int y)				//눌린 칸에 인접한 지뢰가 0개일때 주변 칸을 탐색하여 그 칸에 인접한 지뢰가 0개이면 자동으로 열어주는 함수.
{
	if (arXY[i][j].blockStatus == Block::clicked)		//스택 오버 플로우 방지용
	{
		return;
	}
	
	else
	{
		arXY[i][j].blockStatus = Block::clicked;		// 블록의 상태를 클릭됨으로 변경

		switch (mineLocate(arXY, i, j, x, y))
		{
		case 0:
		{

				  nearbyBlockIsMineSunFunc(arXY, 1, 0, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 0, 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 1, 1, x, y);

				  return;
		}

		case 1:
		{
				 
				  nearbyBlockIsMineSunFunc(arXY, x-1, 0, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-2, 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-1, 1, x, y);
				 
				  return;
		}

		case 2:
		{

				  nearbyBlockIsMineSunFunc(arXY, 1, y-2, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 0, y-1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 1, y-1, x, y);
				 
				  return;
		}

		case 3:
		{

				  nearbyBlockIsMineSunFunc(arXY, x-1, y-2, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-2, y-1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-1, x-1, x, y);
				 
				  return;
		}

		case 4:
		{

				  nearbyBlockIsMineSunFunc(arXY, 0, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 0, j + 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 1, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 1, j, x, y);

				  nearbyBlockIsMineSunFunc(arXY, 1, j + 1, x, y);
		
				  return;
		}

		case 5:
		{

				  nearbyBlockIsMineSunFunc(arXY, x-1, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-1, j + 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-2, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-2, j, x, y);

				  nearbyBlockIsMineSunFunc(arXY, x-2, j + 1, x, y);
			
				  return;
		}

		case 6:
		{

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 0, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 0, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i, 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 1, x, y);
				
				  return;
		}

		case 7:
		{

				  nearbyBlockIsMineSunFunc(arXY, i + 1, y-1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, y-1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, y-2, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i, y - 2, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, y - 2, x, y);
			
				  return;
		}

		case 8:
		{
				
				  nearbyBlockIsMineSunFunc(arXY, i - 1, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j - 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, j, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, j + 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i, j + 1, x, y);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j + 1, x, y);

				  return;
		}
		}
	}

}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

int getKey(void)
{
	int keyInt = _getch();

	if (keyInt == 0 || keyInt == 224)
	{
		keyInt = 256 + _getch();
	}

	return keyInt;
}

int main(void)
{
	srand((unsigned int)time(NULL));	//난수

	HANDLE hOut = NULL;

	CONSOLE_SCREEN_BUFFER_INFO SBInfo;

	GetConsoleScreenBufferInfo(hOut, &SBInfo);

	int sizeX = 10, sizeY = 10;			//판 사이즈
	int i = 0, j = 0;
	int locat = 0;
	int sumMine = 0;
	int playX = 0, playY = 0, playMark = 0;
	int sum = 0;
	int mineCount = 1;
	int kb = 0;
	bool ifPCMine = false;
	bool ifAMCleared = false;

	while (true)
	{
		std::cout << "가로 너비를 입력하세요: ";
		std::cin >> sizeX;

		std::cout << "세로 너비를 입력하세요: ";
		std::cin >> sizeY;

		std::cout << "지뢰 개수를 입력하세요: ";
		std::cin >> mineCount;

		if (mineCount < sizeX*sizeY)
		{
			system("cls");
			gotoxy(0, 0);
			break;
		}

		else
		{
			std::cout << "지뢰 개수는 총 칸의 개수보다 적어야 합니다.";
			gotoxy(0, 0);
			std::cout << "				" << std::endl;
			std::cout << "				" << std::endl;
			std::cout << "				" << std::endl;
			gotoxy(0, 0);
		}
	}

	Block **arXY = new Block*[sizeY];
	for (i = 0; i < sizeX; ++i) 
	{
		arXY[i] = new Block[sizeY];
	}

	bool count = true;

	while (sumMine < mineCount)		//지뢰 생성
	{
		locat = rand() % (sizeX*sizeY);

		if (arXY[(locat % sizeX)][locat / sizeX].isMine)
		{
			continue;
		}
		arXY[(locat % sizeX)][locat / sizeX].isMine = true;

		sumMine += 1;
	}

	setcolor(8, 0);

	std::cout << "┌";
	for (i = 0; i < sizeX; i++)
	{
		std::cout << "─";
	}
	std::cout << "┐"<<std::endl;

	for (j = 0; j < sizeY; j++)
	{
		std::cout << "│";
		for (i = 0; i < sizeX; i++)
		{
			std::cout << "  ";
		}
		std::cout << "│" << std::endl;
	}

	std::cout << "└";
	for (i = 0; i < sizeX; i++)
	{
		std::cout << "─";
	}
	std::cout << "┘" << std::endl;



	for (i = 0; i < sizeY; i++)
	{
		gotomineLocate(0, i);
		for (j = 0; j < sizeX; j++)
		{
			std::cout << "■";
		}
	}

	gotoxy(0, sizeY+3);

	std::cout << "화살표로 이동 / A : 블록 클릭 / S : 블록 표시 / D : 블록 표시 제거" << std::endl;

	gotomineLocate(0, 0);

	playX = 0;
	playY = 0;

	while (true)
	{
		kb = getKey();

		if (kb == UP || kb == DOWN || kb == LEFT || kb == RIGHT)
		{
			switch (mineLocate(arXY, playX, playY, sizeX, sizeY))
			{
			case 0:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, sizeY - 1);
						  playY = sizeY - 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(sizeX - 1, playY);
						  playX = sizeX - 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}

			case 1:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, sizeY - 1);
						  playY = sizeY - 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(0, playY);
						  playX = 0;
						  break;
					  }
					  break;
			}

			case 2:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, 0);
						  playY = 0;
						  break;

					  case LEFT:
						  gotomineLocate(sizeX - 1, playY);
						  playX = sizeX - 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}

			case 3:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, 0);
						  playY = 0;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(0, playY);
						  playX = 0;
						  break;
					  }
					  break;
			}

			case 4:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(sizeX - 1, playY);
						  playX = sizeX - 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}

			case 5:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(0, playY);
						  playX = 0;
						  break;
					  }
					  break;
			}

			case 6:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, sizeY - 1);
						  playY = sizeY - 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}

			case 7:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, 0);
						  playY = 0;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}

			case 8:
			{
					  switch (kb)
					  {
					  case UP:
						  gotomineLocate(playX, playY - 1);
						  playY -= 1;
						  break;

					  case DOWN:
						  gotomineLocate(playX, playY + 1);
						  playY += 1;
						  break;

					  case LEFT:
						  gotomineLocate(playX - 1, playY);
						  playX -= 1;
						  break;

					  case RIGHT:
						  gotomineLocate(playX + 1, playY);
						  playX += 1;
						  break;
					  }
					  break;
			}
			}
		}

		else if (kb == A || kb == a)
		{
			if (arXY[playX][playY].blockStatus == Block::clicked)
				continue;
			else
			{
				if (arXY[playX][playY].isMine)
				{
					std::cout << "♂" << std::endl;
					gotoxy(0, sizeY + 5);
					std::cout << "FAILED" << std::endl;
					break;
				}
				else
				{
					if (mineSum(arXY, playX, playY, sizeX, sizeY) > 0)
					{
						std::cout << mineSum(arXY, playX, playY, sizeX, sizeY) << " " << std::endl;
						arXY[playX][playY].blockStatus = Block::clicked;
						gotomineLocate(playX, playY);
					}

					else
					{
						std::cout << "□" << std::endl;
						nearbyBlockIsMine(arXY, playX, playY, sizeX, sizeY);
						gotomineLocate(playX, playY);
					}
				}
			}
		}

		else if (kb == S || kb == s)
		{
			std::cout << "★" << std::endl;
			arXY[playX][playY].blockStatus = 2;
			gotomineLocate(playX, playY);
		}

		else if (kb == D || kb ==d)
		{
			std::cout << "■" << std::endl;
			arXY[playX][playY].blockStatus = 0;
			gotomineLocate(playX, playY);
		}

		else if (kb == ESC)
		{
			break;
		}

	
		for (i = 0; i < sizeX; i++)			//클릭한 블록수 점검
		{
			for (j = 0; j < sizeY; j++)
			{
				if (!arXY[i][j].isMine && arXY[i][j].blockStatus == 1)
				{
					sum += arXY[i][j].blockStatus;
				}
			}
		}

		if (sum == (sizeX * sizeY - mineCount))		//클리어
		{
			gotoxy(0, sizeY + 5);
			std::cout << "Cleared!" << std::endl;
			break;
		}

		else
		{
			sum = 0;
		}

	}

	for (i = 0; i < sizeX; ++i)	//메모리 반환
	{
		delete[] arXY[i];
	}
	delete[] arXY;

	return 0;
}