#include <stdio.h>
#include "arengine/Util.h"

using namespace arengine;

int main(int argc, char **argv)
{
	unsigned int hash=0;
	if (argc == 2)
	{
		hash = Util::getImageHash(string(argv[1]));
	}
	else
	{
		hash = Util::getImageHash();
	}
	FILE* hFile = fopen("imgid", "w");
	printf("hash = %u\n", hash);
	fprintf(hFile, "%u", hash);
	fclose(hFile);
	return 0;
}