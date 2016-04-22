// big-data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"

#include "u-console-mgr.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	uConsoleMgr::echo("yolo\n", uConsoleMgr::CUTE);
	system("pause");
	return 0;
}

