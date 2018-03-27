#include <iostream>
#include <fstream>
#include "SystemManager.h"
#include "Location.h"

using namespace std;

int main(void)
{

	SystemManager<Location> managerBKSH;

	while (managerBKSH.Menu());

	return 0;
}
