

#include <iostream>
#include "App.h"


int main(int argc, char** argv[])
{	
	App app;
	while (!app.quit){
		app.update();
	};
	return 0;
}