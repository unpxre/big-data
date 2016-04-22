#include <windows.h>

#define NORMAL_COLOR		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define ALERT_COLOR			FOREGROUND_RED | FOREGROUND_INTENSITY
#define SUCCESS_COLOR		FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define WARNING_COLOR		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define INFO_COLOR			FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define CUTE_COLOR			FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY 
#define INPUT_COLOR			BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY


namespace uConsoleMgr {

	enum colors {
		NORMAL = 0,
		ALERT = 1,
		WARNING = 2,
		SUCCESS = 3,
		INFO = 4,
		CUTE = 5, //pink
		INPUT = 6
	};

	void color(char xcolor) {
		HANDLE hOut;
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (xcolor) {
		case ALERT:
			SetConsoleTextAttribute(hOut, ALERT_COLOR);
			break;
		case WARNING:
			SetConsoleTextAttribute(hOut, WARNING_COLOR);
			break;
		case SUCCESS:
			SetConsoleTextAttribute(hOut, SUCCESS_COLOR);
			break;
		case INFO:
			SetConsoleTextAttribute(hOut, INFO_COLOR);
			break;
		case CUTE:
			SetConsoleTextAttribute(hOut, CUTE_COLOR);
			break;
		case INPUT:
			SetConsoleTextAttribute(hOut, INPUT_COLOR);
			break;

		case NORMAL:
		default:
			SetConsoleTextAttribute(hOut, NORMAL_COLOR);
		}
	}

	template<typename T>
	void echo(T data, int scolor = NORMAL) {
		color(scolor);
		if ((scolor == 'A') || (scolor == 'W')) std::cerr << data;
		else std::cout << data;
		color(NORMAL);
	}

	template<typename T>
	T ask() {
		T data;
#ifdef CMD_PROMPT
		color(INFO);
		std::cout << CMD_PROMPT;
#endif
		color(INPUT);
		std::cin >> data;
		color(NORMAL);
		return data;
	}
}