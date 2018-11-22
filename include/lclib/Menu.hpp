#ifndef Menu_hpp_2018_02_22_8_50
#define Menu_hpp_2018_02_22_8_50
#include <lclib/Text.hpp>
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
class LIBLCAPI MenuItem{
private:
	Color c;
	Color bgColor;
	string name;
	int code;
	MenuItem(const MenuItem&)=delete;
	MenuItem& operator=(const MenuItem&)=delete;
public:
	virtual ~MenuItem()=default;
	MenuItem(const string&,int);
	MenuItem(const string&,Color,int);
	MenuItem(string&&,int);
	MenuItem(string&&,Color,int);
	MenuItem(const string&,Color,Color,int);
	MenuItem(string&&,Color,Color,int);
	MenuItem(MenuItem&&)=default;
	MenuItem& operator=(MenuItem&&)=default;
	Color getColor()const;
	Color getBackgroundColor()const;
	const string& getName()const;
	int getCode()const;
	void draw(Terminal&)const;
};

class LIBLCAPI Menu{
private:
	string name;
	Color c;
	vector<MenuItem*> menuItems;
	int index;
	map<int,function<int()>> handlers;
	recursive_mutex lock;
	LIBLCHIDE void drawV(Terminal&);
	LIBLCHIDE MenuItem& getTargetItem()const;
	LIBLCHIDE int getTargetPosition()const;
	LIBLCHIDE void increment();
	LIBLCHIDE void decrement();
	Menu(const Menu&)=delete;
	Menu& operator=(const Menu&)=delete;
public:
	Menu();
	Menu(const string&);
	Menu(const string&,Color);
	Menu(string&&);
	Menu(string&&,Color);
	Menu(Menu&&)=default;
	Menu& operator=(Menu&&)=default;
	Menu& addItem(MenuItem&);
	int select(Terminal&);
	void handleKey(int,function<int()>);
};


#endif
