/*
 * LinuxTerminal.cpp
 *
 *  Created on: Sep 20, 2018
 *      Author: connor
 */

#include <lclib-cxx/Text.hpp>

#include <iostream>

#define RESET "\x1b[0m"


using std::to_string;





#include <unistd.h>
#include <termios.h>

#include <cstdlib>


using std::cout;
using std::endl;

using namespace std::string_literals;


int Terminal::get()noexcept(true){
	std::lock_guard<std::recursive_mutex> sync(lock);
    unsigned char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}


Terminal& Terminal::clear()noexcept(true){
    std::lock_guard<std::recursive_mutex> sync(lock);
    cout << RESET <<endl;
    system("clear");
    return *this;
}

Terminal& Terminal::wait()noexcept(true){
	std::lock_guard<std::recursive_mutex> sync(lock);
	get();
	return *this;
}


