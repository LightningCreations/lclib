#include <lclib/Menu.hpp>
#include <lclib/Random.hpp>
#include <iostream>
#include <cstdlib>

using namespace std::string_literals;



MenuItem::MenuItem(const string& name,int code):c(Color::NONE),bgColor(Color::NONE),name(name),code(code){}
MenuItem::MenuItem(const string& name,Color color,int code):c(color),bgColor(Color::NONE),name(name),code(code){}
MenuItem::MenuItem(const string& name,Color fgColor,Color bgColor,int code):c(fgColor),bgColor(bgColor),name(name),code(code){}

MenuItem::MenuItem(string&& name,int code):c(Color::NONE),bgColor(Color::NONE),name(name),code(code){}
MenuItem::MenuItem(string&& name,Color color,int code):c(color),bgColor(Color::NONE),name(name),code(code){}
MenuItem::MenuItem(string&& name,Color fgColor,Color bgColor,int code):c(fgColor),bgColor(bgColor),name(name),code(code){}

Color MenuItem::getColor()const{
	return c;
}
Color MenuItem::getBackgroundColor()const{
	return bgColor;
}
const string& MenuItem::getName()const{
	return name;
}

int MenuItem::getCode()const{
	return code;
}
void MenuItem::draw(Terminal& t)const{
	t.print(c,TextComponent(bgColor,in_background),name);
}



void Menu::drawV(Terminal& t){
	std::lock_guard<recursive_mutex> sync(lock);
	t.clear();
	t.print("\t\t"s,c,name,Color::Reset,endline);
	for(int i =0;i<menuItems.size();i++){
		t.print(i==index?"->"s:"  "s);
		menuItems[i]->draw(t);
		t.print(Color::Reset,endline);
	}
}

MenuItem& Menu::getTargetItem()const{
	return *menuItems[index];
}
int Menu::getTargetPosition()const{
	return index;
}
void Menu::increment(){
	std::lock_guard<recursive_mutex> sync(lock);
	index++;
	if(index>=menuItems.size())
		index = 0;
}
void Menu::decrement(){
	std::lock_guard<recursive_mutex> sync(lock);
	index--;
	if(index<0)
		index = menuItems.size()-1;
}


Menu::Menu():index(0){}
Menu::Menu(const string& s):index(0),name(s),c(Color::NONE){}
Menu::Menu(const string& s,Color c):index(0),name(s),c(c){}
Menu::Menu(string&& s):index(0),name(s),c(Color::NONE){}
Menu::Menu(string&& s,Color c):index(0),name(s),c(c){}




Menu& Menu::addItem(MenuItem& i){
	std::lock_guard<recursive_mutex> sync(lock);
	menuItems.push_back(&i);
	return *this;
}
int Menu::select(Terminal& t){
	std::lock_guard<recursive_mutex> sync(lock);
	do{
		int i;
		drawV(t);
		i = t.get();
		switch(i){
			case 'w':
			case 'W':
				this->decrement();
			break;
			case 's':
			case 'S':
				this->increment();
			break;
			case '\r':
			case '\n':
				t.clear();
				return this->getTargetItem().getCode();
			break;
			default:
				if(handlers.count(i)!=0){
					t.clear();
					return handlers[i]();
				}
			break;
		}
	}while(true);
}
void Menu::handleKey(int key,function<int()> handler){
	handlers[key] = handler;
}
