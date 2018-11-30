#include <lclib/Config.hpp>
#ifdef LIBLCCXX_OS_WIN32
#include <lclib/Text.hpp>

#include <iostream>

const char RESET[]{ "\x1b[0m" };


using std::to_string;



extern "C" {
#include <conio.h>
}


#include <cstdlib>


using std::cout;
using std::endl;

using namespace std::string_literals;


int Terminal::get()noexcept(true) {
	std::lock_guard<std::recursive_mutex> sync{lock};
	return _getch();
}


Terminal& Terminal::clear()noexcept(true) {
	std::lock_guard<std::recursive_mutex> sync{ lock };
	cout << RESET << endl;
	system("clear");
	return *this;
}

Terminal& Terminal::wait()noexcept(true) {
	std::lock_guard<std::recursive_mutex> sync{ lock };
	get();
	return *this;
}

#endif