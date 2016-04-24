// big-data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "set"

#include <boost/thread/thread.hpp>
#include <boost/regex.hpp>

using namespace std;

#include "u-console-mgr.h"
#include "u-utils.h"
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
		typedef bool(*validateFuncType)(string);
		typedef string(*getContentType)(string);

		set<string> parsedPages;
		set<string> pagesToParse;
		string baseLink;
		unsigned int threads;
		bool work;
		validateFuncType validateFunc;
		getContentType getContentFunc;
		boost::mutex urlsMutex;

		string getUrlToParse() {
			boost::mutex::scoped_lock mylock(urlsMutex);
			if (!pagesToParse.empty()) {
				string tmpUrl;
				tmpUrl = *pagesToParse.begin();
				pagesToParse.erase(pagesToParse.begin());
				parsedPages.insert(tmpUrl);
				return tmpUrl;
			}
			return "ERR";
		}

		void addUrlIfNotContain(string url) {
			boost::mutex::scoped_lock mylock(urlsMutex);
			if ((parsedPages.find(url) == parsedPages.end()) && (pagesToParse.find(url) == pagesToParse.end())) {
				pagesToParse.insert(url);
			}
		}

		void getUrls(string data) {
			vector<string> urls;
			const char* pattern = "[h,r,e,f,H,R,E,F]{4}\\=[[:alnum:]\\~\\\"\\\'\\=\\?\\,\\.\\;\\%\\&\\/\\\\\\+\\-\\_\\(\\)\\[\\]\\|\\{\\}\\:]+";
			boost::regex ip_regex(pattern);
			boost::sregex_iterator it(data.begin(), data.end(), ip_regex);
			boost::sregex_iterator end;

			for (; it != end; ++it) {
				string url = it->str();
				if (url.length() < 7) continue;
				url = url.substr(5, (url.length() - 5)); //usuniecie hrefa
				if ((url[0] == '\"') || (url[0] == '\'')) url = url.substr(1, (url.length() - 1)); //usniecie pierwszego " or '
				if ((url[url.length() - 1] == '\"') || (url[url.length() - 1] == '\'')) url = url.substr(0, (url.length() - 1)); //usniecie ostaniego ' or "
				if (url[0] == '/') url = baseLink + url; // dodanie hosta jesli brak
				url = search_replace(url, "www.", "");
				if (url.find(baseLink) == string::npos) continue;
				if (!validateFunc(url)) continue;
				
				addUrlIfNotContain(url);
				//<div class="single-entry-text bbtext" data-st-area="linki-w-tresci">
				//std::cout << url << "\n";
			}
		}

		void parsingThread() {
			while (work) {
				string tmpUrl = getUrlToParse();
				//std::cout << "download'n: " << tmpUrl << endl;
				if (tmpUrl != "ERR") {
					//cout << ".";
					std::cout << "download'n: " << tmpUrl << endl;
					string data = DownWWW(tmpUrl);
					getUrls(data);
					string content = getContentFunc(data);
					std::cout << content << "\n---\n";
				} else {
					boost::this_thread::sleep(boost::posix_time::milliseconds(500));
				}
			}
		}

	public:
		SiteParser(string baseLink, unsigned int threads, validateFuncType validateFunc, getContentType getContentFunc) {
			work = true;
			this->baseLink = baseLink;
			this->threads = threads;
			this->validateFunc = validateFunc;
			this->getContentFunc = getContentFunc;
			pagesToParse.insert(baseLink);
			boost::thread_group group;
			for (int i = 0; i < this->threads; ++i) group.create_thread(boost::bind(&SiteParser::parsingThread, this));
			group.join_all();
		}

};

bool validateUrl_pudelek_pl(string url) {
	if (url.find("/foto_") != string::npos) return false;
	if (url.find(".png") != string::npos) return false;
	if (url.find(".jpg") != string::npos) return false;
	if (url.find(".jpeg") != string::npos) return false;
	if (url.find(".pdf") != string::npos) return false;
	return true;
}

string getArticle_pudelek_pl(string data) {
	return u_substr("data-st-area=\"linki-w-tresci\">", "<div class=\"single-entry__footer\">", data, 0, "<div class=\"single-entry-text bbtext");
}

int _tmain(int argc, _TCHAR* argv[]) {


	new SiteParser("http://pudelek.pl", 1, &validateUrl_pudelek_pl, &getArticle_pudelek_pl);

	//uConsoleMgr::echo("yolo\n", uConsoleMgr::CUTE);
	system("pause");
	return 0;
}

