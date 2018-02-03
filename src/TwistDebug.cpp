#include "Twist.h"

#include "Platform.h"
#ifdef WINDOWS
	#include <Windows.h>
#endif

namespace Twist {
	namespace Debugging {
		DebugStream& DebugStream::operator<<(const char *text) {
#ifdef WINDOWS
			OutputDebugStringA(text);
#else
			std::cout << text;
#endif
			return *this;
		}

		DebugStream& DebugStream::operator<<(const std::string& text) {
			return operator<<(text.c_str());
		}

		DebugStream& DebugStream::operator<<(int i) {
			return operator<<(std::to_string(i));
		}

		DebugStream& DebugStream::operator<<(float f) {
			return operator<<(std::to_string(f));
		}

		DebugStream& DebugStream::operator<<(double d) {
			return operator<<(std::to_string(d));
		}

		DebugStream& DebugStream::operator<<(char c) {
			char data[2];
			data[0] = c;
			data[1] = '\0';
			return operator<<(data);
		}
	}

	Debugging::DebugStream Debug;
}

