#include <iostream>
#include "skel.h"

using namespace std;



void read_skel(const char *filename)
{
	Skel skel(filename);

	skel.showHeadInfo();

}
 
int main(int argc, char* argv[])
{
	
	read_skel("res/skel/6779ED9F.skel");
	
	return 0;
}
