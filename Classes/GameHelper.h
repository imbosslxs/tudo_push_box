#ifndef _H_GAMEHELPER_
#define _H_GAMEHELPER_


#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <ctime>
#include <string>
#include <sstream>
#include "cocos2d.h"

#define UINT unsigned int
#define DWORD unsigned long long

namespace tudo_push_box {
inline unsigned long GetTickCount()
{
	timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#endif


namespace tudo_push_box {
//======help function=======
inline void msg_box(std::string text, std::string cap = "You Know What", UINT type = 0) {
	cocos2d::MessageBox(text.c_str(), cap.c_str());
}
inline std::string convert_int_to_string(int x) {
	std::stringstream ss;
	ss << x;
	std::string a;
	ss >> a;
	return a;
}
//======help function=======
}


#endif