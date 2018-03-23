#include <iostream>
#include <fstream>
#include "SystemManager.h"

using namespace std;

int main(void)
{

	SystemManager managerBKSH;

	managerBKSH.selectGraph();

	managerBKSH.loadFiles();
	

	return 0;
}
 