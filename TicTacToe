#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>

// Buffer size
int nScreenWidth = 80;
int nScreenHeight = 30;
wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];

// The visible field
int nFieldWidth = 35;
int nFieldHeight = 16;
unsigned char *pField = nullptr;

// an array to store 3x3 logical field 
int tile[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

int winner = -1;
int nO, nX = 0;
int nTiles = 9;


void CheckForWinner()
{

	if (nTiles == 0)
	{
		winner = 2;
		return;
	}

	//check for winner
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++)
		{
			// Check columns
			int index = i * 2 + i;
			if (tile[index + 0] == j && tile[index + 1] == j && tile[index + 2] == j) {
				winner = j;
				j == 0 ? nO++ : nX++;
				return;
			}
			// Check rows
			if (tile[i + 0] == j && tile[i + 3] == j && tile[i + 6] == j) {
				winner = j;
				j == 0 ? nO++ : nX++;
				return;
			}
		}

		// Check diagonals
		if ((tile[0] == j && tile[4] == j && tile[8] == j) ||
			(tile[2] == j && tile[4] == j && tile[6] == j)) {
			winner = j;
			j == 0 ? nO++ : nX++;
		}
	}
}

void InitializeBoard(unsigned char *field, int width, int height) {
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
		{
			// Fill array with plus or dot
			field[y*width + x] = (x < width ||
				x == width - 1 || y == height - 1) ? 2 : 0;

			// Divide field to 3x3 tiles
			if ((y == 5 || y == 10) && (x > 0 && (width > x + 1))) {
				field[y*width + x] = 1;
			}

			if (x == 11 || x == 12 || x == 22 || x == 23) {
				field[y*width + x] = 1;
			}
		}
}


int main()
{
	/************************************************************************/
	/* Setup		                                                        */
	/************************************************************************/

	std::wstring shapes[2];
	bool bFirstPlayer = true;

	// Nought					
	shapes[0].append(L".+.+.");	
	shapes[0].append(L"+...+");	
	shapes[0].append(L"+...+");	
	shapes[0].append(L".+.+.");	
								
	// Cross					
	shapes[1].append(L"++.++");	
	shapes[1].append(L".+++.");	
	shapes[1].append(L".+++.");	
	shapes[1].append(L"++.++");	

	// Create a new console handle
	HANDLE hConsole = CreateConsoleScreenBuffer (GENERIC_READ | 
		GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	// Set created handle to be new buffer
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0; // number of bytes to write
	swprintf_s(screen, 11, L"O=%2d, X=%2d", nO, nX);

	// Set default console text to whitespace
	for (int x = 0; x < nScreenHeight*nScreenWidth; x++)
		screen[x] = L' ';

	// Initialize the board for drawing
	pField = new unsigned char[nFieldWidth*nFieldHeight];
	InitializeBoard(pField, nFieldWidth, nFieldHeight);

	bool bGameOver = false;

	// Game loop
	while (!bGameOver)
	{
		/************************************************************************/
		/* Input                                                                */
		/************************************************************************/
		if (GetAsyncKeyState((unsigned short)'\x2B') & 0x8000)
			bGameOver = true;

		// using numpad keys as input
		for (int k = 0; k < 9; k++)
			if (0x8000 & GetAsyncKeyState((unsigned char)
				("\x67\x68\x69\x64\x65\x66\x61\x62\x63"[k]))) {
				if (tile[k] == -1 && bFirstPlayer) {
					tile[k] = 0;
					bFirstPlayer = !bFirstPlayer;
					nTiles--;
				}
				else if (tile[k] == -1 && !bFirstPlayer) {
					tile[k] = 1;
					bFirstPlayer = !bFirstPlayer;
					nTiles--;
				}
			}
		
		if (winner == -1) {
			/************************************************************************/
			/* Game logic                                                           */
			/************************************************************************/
			CheckForWinner();

			/************************************************************************/
			/* Render output                                                        */
			/************************************************************************/

			for (int index = 0; index < 9; index++) {
				if (tile[index] != -1)
				{
					int x = index % 3;
					int y = index / 3;

					// draw shape in corresponding tile
					for (int px = 0; px < 5; px++)
						for (int py = 0; py < 4; py++)
						{
							if (shapes[tile[index] == 0 ? 0 : 1][py * 5 + px] == L'+')
								pField[(y * 5 + (1 + py)) * nFieldWidth
								+ (x * 11 + (4 + px))] = 1;
						}
				}
			}

			for (int x = 0; x < nFieldWidth; x++)
				for (int y = 0; y < nFieldHeight; y++)
				{
					// Draw field to screen
					screen[(y + 2)*nScreenWidth + (x + 3)] =
						L" +."[pField[y*nFieldWidth + x]];
				}
		}
		else if (winner != -1)
		{
			swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 12, L"Nought =%3d", nO);
			swprintf_s(&screen[4 * nScreenWidth + nFieldWidth + 6], 12, L"Cross  =%3d", nX);

			if (GetAsyncKeyState((unsigned short)'\x60') & 0x8000) {
				winner = -1;

				for (int i = 0; i < 9; i++)
					tile[i] = -1;

				InitializeBoard(pField, nFieldWidth, nFieldHeight);
				nTiles = 9;
			}
		}

		// Display the frame
		WriteConsoleOutputCharacter(hConsole, screen,
			nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole);
	system("pause");
}
