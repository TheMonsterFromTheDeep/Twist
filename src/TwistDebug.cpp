#include "Twist.h"

#include "Platform.h"
#ifdef WINDOWS
	#include <Windows.h>
#endif

namespace Twist {
	namespace debugging {
		debugstream& debugstream::operator<<(const char *text) {
#ifdef WINDOWS
			OutputDebugStringA(text);
#else
			std::cout << text;
#endif
			return *this;
		}

		debugstream& debugstream::operator<<(const std::string& text) {
			return operator<<(text.c_str());
		}

		debugstream& debugstream::operator<<(int i) {
			return operator<<(std::to_string(i));
		}

		debugstream& debugstream::operator<<(float f) {
			return operator<<(std::to_string(f));
		}

		debugstream& debugstream::operator<<(double d) {
			return operator<<(std::to_string(d));
		}

		debugstream& debugstream::operator<<(char c) {
			char data[2];
			data[0] = c;
			data[1] = '\0';
			return operator<<(data);
		}
	}

	debugging::debugstream debug;
}

