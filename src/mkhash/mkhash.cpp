#include <stdio.h>
#include "arengine/Util.h"

using namespace arengine;

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage:mkhash <ARApp Path>");
	}
	else
	{
		int hash;
		hash = Util::getImageHash(string(argv[1]));
		FILE *hFile = fopen("imgid", "w");
		printf("hash = %d", hash);
		fprintf(hFile, "%d", hash);
		fclose(hFile);
	}
	return 0;
}