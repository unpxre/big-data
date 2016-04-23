// big-data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"

using namespace std;

#include "u-console-mgr.h"
#include "u-curl.h"



int _tmain(int argc, _TCHAR* argv[]) {

	cout << DownWWW("http://pudelek.pl");

	uConsoleMgr::echo("yolo\n", uConsoleMgr::CUTE);
	system("pause");
	return 0;
}

