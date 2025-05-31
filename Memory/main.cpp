#include "memory/memory.h"

int main()
{
	Memory memory(L"cs2.exe");

	int value = memory.Read<int>(1337);
	memory.Write<int>(1337, 1337);

	return 0;
}