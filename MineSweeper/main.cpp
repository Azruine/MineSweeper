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

int mineSum(Block** arXY, int i, int j)
{
	if (i == 0 && j == 0)
	{
		return boolInt(arXY[1][0].isMine) + boolInt(arXY[0][1].isMine) + boolInt(arXY[1][1].isMine);
	}

	else if (i == 9 && j == 0)
	{
		return boolInt(arXY[9][0].isMine) + boolInt(arXY[8][1].isMine) + boolInt(arXY[9][1].isMine);
	}

	else if (i == 0 && j == 9)
	{
		return boolInt(arXY[1][8].isMine) + boolInt(arXY[0][9].isMine) + boolInt(arXY[1][9].isMine);
	}

	else if (i == 9 && j == 9)
	{
		return boolInt(arXY[9][8].isMine) + boolInt(arXY[8][9].isMine) + boolInt(arXY[9][9].isMine);
	}

	else if (i == 0)
	{
		return boolInt(arXY[0][j - 1].isMine) + boolInt(arXY[0][j + 1].isMine) + boolInt(arXY[1][j - 1].isMine) + boolInt(arXY[1][j].isMine) + boolInt(arXY[1][j + 1].isMine);
	}

	else if (i == 9)
	{
		return boolInt(arXY[9][j - 1].isMine) + boolInt(arXY[9][j + 1].isMine) + boolInt(arXY[8][j - 1].isMine) + boolInt(arXY[8][j].isMine) + boolInt(arXY[8][j + 1].isMine);
	}

	else if (j == 0)
	{
		return boolInt(arXY[i - 1][0].isMine) + boolInt(arXY[i + 1][0].isMine) + boolInt(arXY[i - 1][1].isMine) + boolInt(arXY[i][1].isMine) + boolInt(arXY[i + 1][1].isMine);
	}

	else if (j == 9)
	{
		return boolInt(arXY[i - 1][9].isMine) + boolInt(arXY[i + 1][9].isMine) + boolInt(arXY[i - 1][8].isMine) + boolInt(arXY[i][8].isMine) + boolInt(arXY[i + 1][8].isMine);
	}
	else
	{
		return boolInt(arXY[i - 1][j - 1].isMine) + boolInt(arXY[i][j - 1].isMine) + boolInt(arXY[i + 1][j - 1].isMine) + boolInt(arXY[i + 1][j].isMine) + boolInt(arXY[i - 1][j + 1].isMine) + boolInt(arXY[i][j + 1].isMine) + boolInt(arXY[i + 1][j + 1].isMine) + boolInt(arXY[i - 1][j].isMine);
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

int main(void)
{
	srand((unsigned int)time(NULL));	//난수

	int sizeX = 10, sizeY = 10;			//판 사이즈
	int i = 0, j = 0;
	int locat = 0;
	int sumMine = 0;
	int playX = 0, playY = 0, playMark = 0;
	int sum = 0;
	bool ifPCMine = false;
	bool ifAMCleared = false;

	Block **arXY = new Block*[10];
	for (int i = 0; i < 10; ++i) 
	{
		arXY[i] = new Block[10];
	}

	bool count = true;



	while (sumMine <= 20)		//지뢰 생성
	{
		locat = rand() % 100;

		if (arXY[locat % 10][(int)(locat / 10)].isMine)
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
		gotoxy(2, i + 1);
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

		gotoxy((playX + 1) * 2, playY + 1);

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
				std::cout << mineSum(arXY, playX, playY) << " " << std::endl;
				arXY[playX][playY].blockStatus = 1;
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

		if (sum == 80)
		{
			std::cout << "Cleared!" << std::endl;
			break;
		}

	}

	for (int i = 0; i < 10; ++i) {
		delete[] arXY[i];
	}
	delete[] arXY;

	return 0;
}