#include "WelcomePage.h"

void typingEffect();
void loadingEffect();
void doneEffect();
void closeEffect();

void printWelcomePage()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string lines[100];
	lines[0] = " O       O OOOOOO O      OOOOO  OOOOO   O   O  OOOOOO";
	lines[1] = " O       O O      O     O      O     O O O O O O";
	lines[2] = " O   O   O OOOOO  O     O      O     O O  O  O OOOOO";
	lines[3] = "  O O O O  O      O     O      O     O O     O O";
	lines[4] = "   O   O   OOOOOO OOOOOO OOOOO  OOOOO  O     O OOOOOO";
	lines[5] = "      OOOOOOOO  OOOOO";
	lines[6] = "         O     O     O";
	lines[7] = "         O     O     O";
	lines[8] = "         O     O     O";
	lines[9] = "         O      OOOOO";
	lines[10] = "           OOOOOOOO O     O OOOO  OOOOOO  O   O  O  OOOOOOO O   O";
	lines[11] = "              O      O   O  O   O O      O O O O OO    O    O   O";
	lines[12] = "              O       O O   OOOO  OOOOO  O  O  O O O   O    OOOOO";
	lines[13] = "              O        O    O     O      O     O OOOO  O    O   O";
	lines[14] = "              O       O     O     OOOOOO O     O O   O O    O   O";
	lines[15] = "                            By: B10915008_Bryan & B10915012_Willy";
	typingEffect();
	cout << endl;
	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono; // nanoseconds, system_clock, seconds

	for (int i = 0; i < 16; i++)
	{
		if (i == 0)
		{
			SetConsoleTextAttribute(hConsole, 14);
		}
		else if (i == 5)
		{
			SetConsoleTextAttribute(hConsole, 15);
		}
		else if (i == 10)
		{
			SetConsoleTextAttribute(hConsole, 14);
		}
		else if (i == 15)
		{
			SetConsoleTextAttribute(hConsole, 11);
		}
		if (i == 5 || i == 10)
		{
			cout << endl;
		}
		cout << lines[i] << endl;
		if (i == 4 || i == 9)
		{
			sleep_for(milliseconds(200));
		}
		else
		{
			sleep_for(milliseconds(50));
		}
	}
	cout << endl;
	closeEffect();
	sleep_for(milliseconds(2000));

	loadingEffect();
	for (int c = 0; c < 34; c++)
	{
		system("cls");
		cout << endl;
		for (int i = 0; i < 16; i++)
		{
			if (i == 0)
			{
				SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i == 5)
			{
				SetConsoleTextAttribute(hConsole, 15);
			}
			else if (i == 10)
			{
				SetConsoleTextAttribute(hConsole, 14);
			}
			else if (i == 15)
			{
				SetConsoleTextAttribute(hConsole, 11);
			}
			if (i == 5 || i == 10)
			{
				cout << endl;
			}
			lines[i].erase(lines[i].begin());
			lines[i].erase(lines[i].begin());
			cout << lines[i] << endl;
		}

		SetConsoleTextAttribute(hConsole, 12);
		cout << " (";
		if (c + 1 != 34)
		{
			switch (c % 4)
			{
			case 0:
				cout << "|";
				break;
			case 1:
				cout << "\\";
				break;
			case 2:
				cout << "-";
				break;
			case 3:
				cout << "/";
				break;
			}
		}
		else
		{
			cout << "100%";
		}
		cout << ") ";
		cout << "Loading. . ." << endl << "Please do not type anything!" << endl;
		sleep_for(milliseconds(45));
	}
	closeEffect();

	doneEffect();
	SetConsoleTextAttribute(hConsole, 10);
	cout << "Loading Done! ENJOY!!!" << endl;
	sleep_for(milliseconds(1000));
	closeEffect();
	system("cls");
	SetConsoleTextAttribute(hConsole, 7);
	cout << "Type your equation here:" << endl;

}


void closeEffect()
{
	mciSendString(TEXT("close mp3"), NULL, 0, NULL);
}

void typingEffect()
{
	mciSendString(TEXT("open \"typing.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL);
}

void loadingEffect()
{
	mciSendString(TEXT("open \"loading.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL);
}

void doneEffect()
{
	mciSendString(TEXT("open \"loading_done.mp3\" type mpegvideo alias mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play mp3"), NULL, 0, NULL);
}