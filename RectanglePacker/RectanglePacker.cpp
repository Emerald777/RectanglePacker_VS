// RectanglePacker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <conio.h>
#include <chrono>

#include "rectangle.h"
#include "packer.h"

#include "levelh.h"

bool applyInput(Packer &pck)
{
	using Size = std::pair<int, int>;

	Size sz;

	if (scanf_s("%d,%d", &sz.first, &sz.second) == EOF) {
		printf("Error while reading container data!\n");
		return false;
	}
	pck.setupContainer(Geometry::Rectangle(static_cast<float>(sz.first), static_cast<float>(sz.second)));

	while (scanf_s("%d,%d", &sz.first, &sz.second) != EOF)
	{
		pck.addBlock(static_cast<float>(sz.first), static_cast<float>(sz.second));
	}
	return true;
}

int main()
{
	Packer packer;
	if (!applyInput(packer))
		return -1;

#ifndef RAW_OUT_DATA
	auto t1 = std::chrono::steady_clock::now();
#endif

	packer.exec();

#ifndef RAW_OUT_DATA
	auto t2 = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	printf("\nDone in %d ms\n", static_cast<int>(elapsed_ms.count()));
#endif

	_getch();
	return 0;
}
