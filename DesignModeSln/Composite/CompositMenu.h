#include "Menuu.h"
#include <vector>

class CompositMenu : public Menuu
{
public:
	CompositMenu();
	CompositMenu(std::string);
	virtual ~CompositMenu();

	void Add(Menuu*);
	void Remove(Menuu*);
	Menuu* GetChild(int);
	void Display();
private:
	std::vector<Menuu*> m_vMenu;
};