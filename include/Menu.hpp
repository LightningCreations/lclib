#ifndef Menu_hpp_2018_02_22_8_50
#define Menu_hpp_2018_02_22_8_50
#include <Text.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <mutex>
#include <initializer_list>

using std::function;
using std::vector;
using std::map;
using std::recursive_mutex;

/**
 * Defines an Entry in a menu.
 * The Entry has a code, a Color (Which applies in the foreground),
 * and a name.
 */
class MenuItem{
private:
	Color c;
	string name;
	int code;
	MenuItem(const MenuItem&)=delete;
	MenuItem& operator=(const MenuItem&)=delete;
public:
	virtual ~MenuItem()=default;
	LIBLCFUNC MenuItem(const string&,int);
	LIBLCFUNC MenuItem(const string&,Color,int);
	LIBLCFUNC MenuItem(string&&,int);
	LIBLCFUNC MenuItem(string&&,Color,int);
	MenuItem(MenuItem&&)=default;
	MenuItem& operator=(MenuItem&&)=default;
	LIBLCFUNC Color getColor()const;
	LIBLCFUNC const string& getName()const;
	LIBLCFUNC int getCode()const;
};

class Menu{
private:
	string name;
	Color c;
	vector<MenuItem*> menuItems;
	int index;
	map<int,function<int()>> handlers;
	recursive_mutex lock;
	void drawV(Terminal&);
	MenuItem& getTargetItem()const;
	int getTargetPosition()const;
	void increment();
	void decrement();
	Menu(const Menu&)=delete;
	Menu& operator=(const Menu&)=delete;
public:
	LIBLCFUNC Menu();
	LIBLCFUNC Menu(const string&);
	LIBLCFUNC Menu(const string&,Color);
	LIBLCFUNC Menu(string&&);
	LIBLCFUNC Menu(string&&,Color);
	Menu(Menu&&)=default;
	Menu& operator=(Menu&&)=default;
	LIBLCFUNC Menu& addItem(MenuItem&);
	LIBLCFUNC int select(Terminal&);
	LIBLCFUNC void handleKey(int,function<int()>);
};


#endif
