// big-data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "set"

#include <boost/thread/thread.hpp>

using namespace std;

#include "u-console-mgr.h"
#include "u-curl.h"

/*
	TODO:
	[] pobrac linki
	[] zwalidowac linki funckaj z paramtru
	[] pobrac tresc funkcja z parametru
	[] wyslac tersc do nowej klasy jaka sobie cos z tym zrobi...
	[] nowa klasa z tym cos zrobi :)
*/

class SiteParser {
	private:
		set<std::string> parsedPages;
		set<std::string> pagesToParse;
		string baseLink;
		unsigned int threads;
		bool work;
		boost::mutex pagesToParseMx;

		string getUrlToParse() {
			boost::mutex::scoped_lock mylock(pagesToParseMx);
			if (!pagesToParse.empty()) {
				string tmpUrl;
				tmpUrl = *pagesToParse.begin();
				pagesToParse.erase(pagesToParse.begin());
				return tmpUrl;
			}
			return "ERR";
		}

		void parsingThread() {
			while (work) {
				string tmpUrl = getUrlToParse();
				if (tmpUrl != "ERR") {
					string data = DownWWW(tmpUrl);
					std::cout << data;
				}
			}
		}

	public:
		SiteParser(string baseLink, unsigned int threads) {
			work = true;
			this->baseLink = baseLink;
			this->threads = threads;
			pagesToParse.insert(baseLink);
			boost::thread_group group;
			for (int i = 0; i < this->threads; ++i) group.create_thread(boost::bind(&SiteParser::parsingThread, this));
			group.join_all();
		}

};


int _tmain(int argc, _TCHAR* argv[]) {


	new SiteParser("http://pudelek.pl", 30);
	//cout << DownWWW("http://pudelek.pl");

	//uConsoleMgr::echo("yolo\n", uConsoleMgr::CUTE);
	system("pause");
	return 0;
}

