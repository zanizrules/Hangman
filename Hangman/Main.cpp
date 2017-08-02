#include <Windows.h>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>
#include <iterator>
#include <cstring>

enum Colours
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGRAY = 7,
	DARKGRAY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

std::array<std::string, 8> hangmanTitle =
{
	" _                                             ",
	"| |                                            ",
	"| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  ",
	"| '_ \\ / _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\",
	"| | | | (_| | | | | (_| | | | | | | (_| | | | |",
	"|_| |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_|",
	"                    __| |                      ",
	"                   |____|                      "
};

std::array<std::string, 8> hangman =
{
	"      _______             ",
	"     |/      |",
	"     |      (_)",
	"     |      \\|/",
	"     |       |",
	"     |      / \\",
	"     |                    Incorrect Guesses: ",
	"  ___|___                     "
};

std::string secretWord = "SECRET";
bool correctLetters[6] = { false, false, false, false, false, false };

std::string getSecretWordString();
std::string guessesString = "";
std::array<char, 26> guesses;
int numberOfFailedAttempts = 0;


void set_text_colour(int foreground, int background)
{
	HANDLE console_window = GetStdHandle(STD_OUTPUT_HANDLE);
	int colour = ((background & 0x0F) << 4) | (foreground & 0x0F);
	SetConsoleTextAttribute(console_window, colour);
}

void drawTitle(std::array<std::string, 8> title, int colour)
{
	set_text_colour(colour, 0);
	for (std::string &line : title)
	{
		std::cout << line << std::endl;
	}
	set_text_colour(15, 0);
}

void drawHangman(int colour)
{
	set_text_colour(colour, 0);
	int lineCount = 0;
	for (std::string &line : hangman)
	{
		std::string lineToPrint = line;

		if (lineCount < 6 && numberOfFailedAttempts < lineCount)
		{
			lineToPrint = line.substr(0, 9);
		}

		if (lineCount == 0)
		{
			std::cout << lineToPrint << getSecretWordString() << std::endl;
		}
		else if (lineCount == 7)
		{
			std::cout << lineToPrint << guessesString << std::endl;
		}
		else
		{
			std::cout << lineToPrint << std::endl;
		}

		lineCount++;
	}
	set_text_colour(15, 0);
}

std::string getSecretWordString()
{
	std::string result = "Secret Word:  ";
	int count = 0;
	for (char &letter : secretWord)
	{
		if (correctLetters[count])
		{
			result += letter;
		}
		else
		{
			result += "_";
		}
		result += " ";
		count++;
	}
	return result;
}

bool letterAlreadyGuessed(char c)
{
	return std::find(std::begin(guesses), std::end(guesses), c) != std::end(guesses);
}

bool invalidLetter(char c)
{
	return c < 65 || c > 122 || (c > 90 && c < 97);
}

void playGame()
{
	bool playing = true;
	char userInput = '0';

	while (playing)
	{

		drawHangman(DARKGRAY);

		std::cout << std::endl << "Guess a Letter: ";
		std::cin >> userInput;

		if (userInput > 96 && userInput < 123)
		{
			userInput -= 32; // Change to Uppercase
		}

		if (invalidLetter(userInput))
		{
			std::cout << std::endl << "Invalid letter!" << std::endl;
		}
		else if (letterAlreadyGuessed(userInput))
		{
			std::cout << std::endl << "You have already guessed that letter!" << std::endl;
		}
		else
		{
			std::cout << std::endl << "You guessed the letter: " <<  userInput << std::endl;

			guesses.assign(userInput);
			bool incorrectGuess = true;
			for (int i = 0; i < 6; i++)
			{
				if (secretWord.c_str()[i] == userInput)
				{
					correctLetters[i] = true;
					incorrectGuess = false;
				}
			}

			if (incorrectGuess)
			{
				numberOfFailedAttempts++;
				if (guessesString.length() != 0)
				{
					guessesString.append(", ");
				}
				guessesString += userInput;
			}

			bool gameWon = true;
			for (int a = 0; a < 6; a++)
			{
				if (correctLetters[a] == false)
				{
					gameWon = false;
				}
			}

			if (gameWon)
			{
				drawHangman(DARKGRAY);
				std::cout << std::endl << "YOU WIN!" << std::endl;
				playing = false;
			}
			else if (numberOfFailedAttempts > 4)
			{
				drawHangman(DARKGRAY);
				std::cout << std::endl << "GAME OVER!" << std::endl;
				playing = false;
			}
		}
	}
}

int main()
{
	drawTitle(hangmanTitle, LIGHTRED);

	std::cout << std::endl;
	std::cout << std::endl;

	char userInput = '0';

	while (userInput != '2')
	{
		std::cout << "Shane's Hangman Game!" << std::endl;
		std::cout << "1: Start New Game" << std::endl;
		std::cout << "2: Quit" << std::endl;

		std::cin >> userInput;

		if (userInput == '1')
		{
			playGame();
		}
		else if (userInput != '2')
		{
			std::cout << "Error: Invalid Input!" << std::endl << std::endl;
		}

	}
}