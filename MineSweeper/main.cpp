#include <iostream>		
#include <stdlib.h>		
#include <time.h>		//time func
#include <windows.h>

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

int boolInt(bool tf)	//불리언 대수에 따른 값 반환. 참일시 1, 거짓일시 0.
{
	if (tf)
	{
		return 1;
	}
	else return 0;
};

int mineLocate(Block** arXY, int i, int j)	//입력받은 좌표가 어떤 경우인지 값을 반환함. 제일 구석, 변, 내부일때 계산식이 전부 달라지므로 편의를 위해 넣음
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

int mineSum(Block** arXY, int i, int j)		//주변 지뢰의 개수를 반환
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

void gotomineLocate(int x, int y)		//gotoxy 함수를 약간 변형함.
{
	COORD pos = { (x + 1) * 2, y + 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void nearbyBlockIsMine(Block** arXY, int i, int j);		//아래의 두 함수에서 재귀함수를 사용하기 위해 미리 선언함

void nearbyBlockIsMineSunFuncZero(Block** arXY, int i, int j)		//주어진 칸에 인접한 지뢰가 0개일때 호출하는 함수
{
	gotomineLocate(i, j);
	std::cout << "□";
	nearbyBlockIsMine(arXY, i, j);
	return;
}

void nearbyBlockIsMineSunFuncNotZero(Block** arXY, int i, int j)	//주어진 칸에 인접한 지뢰가 0개가 아닐때 호출하는 함수
{
	gotomineLocate(i, j);
	std::cout << mineSum(arXY, i, j);
	return;
}

void nearbyBlockIsMineSunFunc(Block** arXY, int i, int j)			//위 두 함수를 묶는 함수. 코드의 절약을 위하여 함수로 선언함.
{
	if (mineSum(arXY, i, j) == 0)
	{
		nearbyBlockIsMineSunFuncZero(arXY, i, j);
	}

	else
	{
		nearbyBlockIsMineSunFuncNotZero(arXY, i, j);
	}

	return;
}

void nearbyBlockIsMine(Block** arXY, int i, int j)				//눌린 칸에 인접한 지뢰가 0개일때 주변 칸을 탐색하여 그 칸에 인접한 지뢰가 0개이면 자동으로 열어주는 함수.
{
	if (arXY[i][j].blockStatus == Block::clicked)		//스택 오버 플로우 방지용
	{
		return;
	}
	
	else
	{
		arXY[i][j].blockStatus = Block::clicked;		// 블록의 상태를 클릭됨으로 변경

		switch (mineLocate(arXY, i, j))
		{
		case 0:
		{

				  nearbyBlockIsMineSunFunc(arXY, 1, 0);

				  nearbyBlockIsMineSunFunc(arXY, 0, 1);

				  nearbyBlockIsMineSunFunc(arXY, 1, 1);	  

				  return;
		}

		case 1:
		{
				 
				  nearbyBlockIsMineSunFunc(arXY, 9, 0);

				  nearbyBlockIsMineSunFunc(arXY, 8, 1);

				  nearbyBlockIsMineSunFunc(arXY, 9, 1);
				 
				  return;
		}

		case 2:
		{

				  nearbyBlockIsMineSunFunc(arXY, 1, 8);

				  nearbyBlockIsMineSunFunc(arXY, 0, 9);

				  nearbyBlockIsMineSunFunc(arXY, 1, 9);
				 
				  return;
		}

		case 3:
		{

				  nearbyBlockIsMineSunFunc(arXY, 9, 8);

				  nearbyBlockIsMineSunFunc(arXY, 8, 9);

				  nearbyBlockIsMineSunFunc(arXY, 9, 9);
				 
				  return;
		}

		case 4:
		{

				  nearbyBlockIsMineSunFunc(arXY, 0, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, 0, j + 1);

				  nearbyBlockIsMineSunFunc(arXY, 1, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, 1, j);

				  nearbyBlockIsMineSunFunc(arXY, 1, j + 1);
		
				  return;
		}

		case 5:
		{

				  nearbyBlockIsMineSunFunc(arXY, 9, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, 9, j + 1);

				  nearbyBlockIsMineSunFunc(arXY, 8, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, 8, j);

				  nearbyBlockIsMineSunFunc(arXY, 8, j + 1);
			
				  return;
		}

		case 6:
		{

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 0);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 0);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 1);

				  nearbyBlockIsMineSunFunc(arXY, i, 1);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 1);
				
				  return;
		}

		case 7:
		{

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 9);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 9);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, 8);

				  nearbyBlockIsMineSunFunc(arXY, i, 8);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, 8);
			
				  return;
		}

		case 8:
		{
				
				  nearbyBlockIsMineSunFunc(arXY, i - 1, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, i, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j - 1);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, j);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j);

				  nearbyBlockIsMineSunFunc(arXY, i - 1, j + 1);

				  nearbyBlockIsMineSunFunc(arXY, i, j + 1);

				  nearbyBlockIsMineSunFunc(arXY, i + 1, j + 1);

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

int main(void)
{
	srand((unsigned int)time(NULL));	//난수

	int sizeX = 10, sizeY = 10;			//판 사이즈
	int i = 0, j = 0;
	int locat = 0;
	int sumMine = 0;
	int playX = 0, playY = 0, playMark = 0;
	int sum = 0;
	int mineCount = 1;
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
			gotoxy(0, 0);
			std::cout << "				" << std::endl;
			std::cout << "				" << std::endl;
			std::cout << "				" << std::endl;
			std::cout << "				" << std::endl;
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

		if (arXY[locat % sizeX][locat / sizeY].isMine)
		{
			continue;
		}

		arXY[locat % sizeX][locat / sizeY].isMine = true;

		sumMine += 1;
	}

	setcolor(8, 0);

	std::cout << "┌";
	for (i = 0; i < sizeY; i++)
	{
		std::cout << "─";
	}
	std::cout << "┐"<<std::endl;

	for (j = 0; j < sizeX; j++)
	{
		std::cout << "│";
		for (i = 0; i < sizeY; i++)
		{
			std::cout << "  ";
		}
		std::cout << "│" << std::endl;
	}

	std::cout << "└";
	for (i = 0; i < sizeY; i++)
	{
		std::cout << "─";
	}
	std::cout << "┘" << std::endl;



	for (i = 0; i < sizeX; i++)
	{
		gotomineLocate(0, i);
		for (j = 0; j < sizeY; j++)
		{
			std::cout << "■";
		}
	}

	gotoxy(0, sizeY+3);

	std::cout << "X 좌표(0부터 시작) Y 좌표(0부터 시작) 마크 종류(0~2)" << std::endl;
	std::cout << "0: 블록 클릭 / 1: 블록 표시 / 2: 블록 표시 제거" << std::endl;

	while (true)
	{
		std::cin >> playX;
		std::cin >> playY;
		std::cin >> playMark;

		if (playX > sizeY - 1 || playX < 0 || playY > sizeX - 1 || playY < 0 || playMark > 2 || playMark < 0)
		{
			gotoxy(0, sizeY + 5);
			std::cout << "		";
			gotoxy(0, sizeY + 5);
			continue;
		}

		if (arXY[playX][playY].blockStatus == Block::clicked)
		{
			gotoxy(0, sizeY + 5);
			std::cout << "		";
			gotoxy(0, sizeY + 5);
			continue;
		}

		gotomineLocate(playX, playY);

		if (playMark == 0)
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
				if (mineSum(arXY, playX, playY) > 0)
				{
					std::cout << mineSum(arXY, playX, playY) << " " << std::endl;
					arXY[playX][playY].blockStatus = Block::clicked;
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
		gotoxy(0, sizeY + 5);
		std::cout << "		";
		gotoxy(0, sizeY + 5);

		for (i = 0; i < sizeX; i++)
		{
			for (j = 0; j < sizeY; j++)
			{
				if (!arXY[i][j].isMine && arXY[i][j].blockStatus == 1)
				{
					sum += arXY[i][j].blockStatus;
				}
			}
		}

		if (sum == (sizeX * sizeY - mineCount))
		{
			std::cout << "Cleared!" << std::endl;
			break;
		}

		else
		{
			sum = 0;
		}

	}

	for (i = 0; i < sizeY; ++i)	//메모리 반환
	{
		delete[] arXY[i];
	}
	delete[] arXY;

	return 0;
}