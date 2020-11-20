#include <stdio.h>

#include "communication.hpp"
#include "application.hpp"

application::application() :
	_communication(0)
{

}

int application::run(void)
{
	int n;


	n = _communication->communicate();
	printf("application::run(): _communication->communicate()=%d\n", n);

	return n;
}
