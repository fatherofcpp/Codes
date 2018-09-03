#include "Menuu.h"

class SubMenu : public Menuu  
{
public:
	SubMenu();
	SubMenu(std::string);
	virtual ~SubMenu();

	void Display();
};