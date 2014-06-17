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
#define a 122
#define s 115
#define d 100

struct Block
{
	bool isMine = false;
	int blockStatus = 0;
	//0: ������ 1: Ŭ���� 2: ǥ�õ�

	enum EBlockStatus		//���Ǹ� ���� enum ����
	{	
		hidden,
		clicked,
		checked
	};
};

int boolInt(bool tf)	//�Ҹ��� ����� ���� �� ��ȯ. ���Ͻ� 1, �����Ͻ� 0.
{
	if (tf)
	{
		return 1;
	}
	else return 0;
};

int mineLocate(Block** arXY, int i, int j, int x, int y)	//�Է¹��� ��ǥ�� � ������� ���� ��ȯ��. ���� ����, ��, �����϶� ������ ���� �޶����Ƿ� ���Ǹ� ���� ����
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

int mineSum(Block** arXY, int i, int j, int x, int y)		//�ֺ� ������ ������ ��ȯ
{
	switch (mineLocate(arXY, i, j, x, y))
	{
	default: return -1;

	case 0:return boolInt(arXY[1][0].isMine) + boolInt(arXY[0][1].isMine) + boolInt(arXY[1][1].isMine);

	case 1:return boolInt(arXY[x-1][0].isMine) + boolInt(arXY[x-2][1].isMine) + boolInt(arXY[x-1][1].isMine);

	case 2:return boolInt(arXY[1][y-2].isMine) + boolInt(arXY[0][y-1].isMine) + boolInt(arXY[1][y-1].isMine);

	case 3:return boolInt(arXY[x-1][y-2].isMine) + boolInt(arXY[x-2][y-1].isMine) + boolInt(arXY[x-1][y-1].isMine);

	case 4:return boolInt(arXY[0][j - 1].isMine) + boolInt(arXY[0][j + 1].isMine) + boolInt(arXY[1][j - 1].isMine) + boolInt(arXY[1][j].isMine) + boolInt(arXY[1][j + 1].isMine);

	case 5:return boolInt(arXY[x-1][j - 1].isMine) + boolInt(arXY[x-1][j + 1].isMine) + boolInt(arXY[x-2][j - 1].isMine) + boolInt(arXY[x-2][j].isMine) + boolInt(arXY[x-2][j + 1].isMine);

	case 6:return boolInt(arXY[i - 1][0].isMine) + boolInt(arXY[i + 1][0].isMine) + boolInt(arXY[i - 1][1].isMine) + boolInt(arXY[i][1].isMine) + boolInt(arXY[i + 1][1].isMine);

	case 7:return boolInt(arXY[i - 1][y-1].isMine) + boolInt(arXY[i + 1][y-1].isMine) + boolInt(arXY[i - 1][y-2].isMine) + boolInt(arXY[i][y-2].isMine) + boolInt(arXY[i + 1][y-2].isMine);

	case 8:return boolInt(arXY[i - 1][j - 1].isMine) + boolInt(arXY[i][j - 1].isMine) + boolInt(arXY[i + 1][j - 1].isMine) + boolInt(arXY[i + 1][j].isMine) + boolInt(arXY[i - 1][j + 1].isMine) + boolInt(arXY[i][j + 1].isMine) + boolInt(arXY[i + 1][j + 1].isMine) + boolInt(arXY[i - 1][j].isMine);		 
	}
}

void gotomineLocate(int x, int y)		//gotoxy �Լ��� �ణ ������.
{
	COORD pos = { (x + 1) * 2, y + 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void nearbyBlockIsMine(Block** arXY, int i, int j, int x, int y);		//�Ʒ��� �� �Լ����� ����Լ��� ����ϱ� ���� �̸� ������

void nearbyBlockIsMineSunFuncZero(Block** arXY, int i, int j, int x, int y)		//�־��� ĭ�� ������ ���ڰ� 0���϶� ȣ���ϴ� �Լ�
{
	gotomineLocate(i, j);
	std::cout << "��";
	nearbyBlockIsMine(arXY, i, j, x, y);
	return;
}

void nearbyBlockIsMineSunFuncNotZero(Block** arXY, int i, int j, int x, int y)	//�־��� ĭ�� ������ ���ڰ� 0���� �ƴҶ� ȣ���ϴ� �Լ�
{
	gotomineLocate(i, j);
	std::cout << mineSum(arXY, i, j, x, y);
	return;
}

void nearbyBlockIsMineSunFunc(Block** arXY, int i, int j, int x, int y)			//�� �� �Լ��� ���� �Լ�. �ڵ��� ������ ���Ͽ� �Լ��� ������.
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

void nearbyBlockIsMine(Block** arXY, int i, int j, int x, int y)				//���� ĭ�� ������ ���ڰ� 0���϶� �ֺ� ĭ�� Ž���Ͽ� �� ĭ�� ������ ���ڰ� 0���̸� �ڵ����� �����ִ� �Լ�.
{
	if (arXY[i][j].blockStatus == Block::clicked)		//���� ���� �÷ο� ������
	{
		return;
	}
	
	else
	{
		arXY[i][j].blockStatus = Block::clicked;		// ����� ���¸� Ŭ�������� ����

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
	srand((unsigned int)time(NULL));	//����

	HANDLE hOut = NULL;

	CONSOLE_SCREEN_BUFFER_INFO SBInfo;

	GetConsoleScreenBufferInfo(hOut, &SBInfo);

	int sizeX = 10, sizeY = 10;			//�� ������
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
		std::cout << "���� �ʺ� �Է��ϼ���: ";
		std::cin >> sizeX;

		std::cout << "���� �ʺ� �Է��ϼ���: ";
		std::cin >> sizeY;

		std::cout << "���� ������ �Է��ϼ���: ";
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
			std::cout << "���� ������ �� ĭ�� �������� ����� �մϴ�.";
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



	while (sumMine < mineCount)		//���� ����
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

	std::cout << "��";
	for (i = 0; i < sizeX; i++)
	{
		std::cout << "��";
	}
	std::cout << "��"<<std::endl;

	for (j = 0; j < sizeY; j++)
	{
		std::cout << "��";
		for (i = 0; i < sizeX; i++)
		{
			std::cout << "  ";
		}
		std::cout << "��" << std::endl;
	}

	std::cout << "��";
	for (i = 0; i < sizeX; i++)
	{
		std::cout << "��";
	}
	std::cout << "��" << std::endl;



	for (i = 0; i < sizeY; i++)
	{
		gotomineLocate(0, i);
		for (j = 0; j < sizeX; j++)
		{
			std::cout << "��";
		}
	}

	gotoxy(0, sizeY+3);

	//std::cout << "X ��ǥ(0���� ����) Y ��ǥ(0���� ����) ��ũ ����(0~2)" << std::endl;
	//std::cout << "0: ��� Ŭ�� / 1: ��� ǥ�� / 2: ��� ǥ�� ����" << std::endl;
	std::cout << "ȭ��ǥ�� �̵� / A : ��� Ŭ�� / S : ��� ǥ�� / D : ��� ǥ�� ����" << std::endl;

	gotomineLocate(0, 0);

	while ((kb = getKey()) != ESC)
	{
		//std::cin >> playX;
		//std::cin >> playY;
		//std::cin >> playMark;

		kb = getKey();

		playX = SBInfo.dwCursorPosition.X / 2 - 1;
		playY = SBInfo.dwCursorPosition.Y - 1;

		if (kb == UP || kb == DOWN || kb == LEFT || kb == RIGHT)
		{
			if (playX > 0 || playX < sizeX - 1 || playY > 0 || playY < sizeY - 1)
			{
				switch (kb)
				{
				case UP: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y - 1);

				case DOWN: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y + 1);

				case LEFT: gotoxy(SBInfo.dwCursorPosition.X - 2, SBInfo.dwCursorPosition.Y);

				case RIGHT: gotoxy(SBInfo.dwCursorPosition.X + 2, SBInfo.dwCursorPosition.Y);
				}
			}

			else if (SBInfo.dwCursorPosition.X = 2)
			{
				switch (kb)
				{
				case UP: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y - 1);

				case DOWN: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y + 1);

				case LEFT: gotoxy((sizeX * 2), SBInfo.dwCursorPosition.Y);

				case RIGHT: gotoxy(SBInfo.dwCursorPosition.X + 2, SBInfo.dwCursorPosition.Y);
				}
			}

			else if (SBInfo.dwCursorPosition.X = (sizeX * 2))
			{
				switch (kb)
				{
				case UP: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y - 1);

				case DOWN: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y + 1);

				case LEFT: gotoxy(SBInfo.dwCursorPosition.X - 2, SBInfo.dwCursorPosition.Y);

				case RIGHT: gotoxy(2, SBInfo.dwCursorPosition.Y);
				}
			}

			else if (SBInfo.dwCursorPosition.Y = 1)
			{
				switch (kb)
				{
				case UP: gotoxy(SBInfo.dwCursorPosition.X, sizeY);

				case DOWN: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y + 1);

				case LEFT: gotoxy((sizeX * 2), SBInfo.dwCursorPosition.Y);

				case RIGHT: gotoxy(SBInfo.dwCursorPosition.X + 2, SBInfo.dwCursorPosition.Y);
				}
			}

			else if (SBInfo.dwCursorPosition.Y = sizeY)
			{
				switch (kb)
				{
				case UP: gotoxy(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y - 1);

				case DOWN: gotoxy(SBInfo.dwCursorPosition.X, 1);

				case LEFT: gotoxy((sizeX * 2), SBInfo.dwCursorPosition.Y);

				case RIGHT: gotoxy(SBInfo.dwCursorPosition.X + 2, SBInfo.dwCursorPosition.Y);
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
					std::cout << "��" << std::endl;
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
					}

					else
					{
						std::cout << "��" << std::endl;
						nearbyBlockIsMine(arXY, playX, playY, sizeX, sizeY);
					}
				}
			}
		}

		else if (kb == S || kb == s)
		{
			std::cout << "��" << std::endl;
			arXY[playX][playY].blockStatus = 2;
		}

		else if (kb == D || kb ==d)
		{
			std::cout << "��" << std::endl;
			arXY[playX][playY].blockStatus = 0;
		}
	
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

	for (i = 0; i < sizeX; ++i)	//�޸� ��ȯ
	{
		delete[] arXY[i];
	}
	delete[] arXY;

	return 0;
}