#include <iostream>
#include <fstream>
#include "SystemManager.h"
#include "Location.h"

using namespace std;

int main(void)
{	
	SystemManager m;
	cout << typeid(Location(1)).name() << endl;
		
	while(m.Menu());

	return 0;
}
