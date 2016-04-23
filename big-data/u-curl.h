#include "curl/curl.h" 
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wldap32")

class curlMultiIns {
public:
	std::string content_;
	static size_t handle(char *data, size_t size, size_t nmemb, void *p);
	size_t handle_impl(char *data, size_t size, size_t nmemb);
	std::string getHC(void) { return content_; };
	curlMultiIns(void);
	CURL *curl;
	CURLcode result;
	std::string THR_link, THK_html;
};

size_t curlMultiIns::handle(char *data, size_t size, size_t nmemb, void *p) {
	return static_cast<curlMultiIns*>(p)->handle_impl(data, size, nmemb);
}

size_t curlMultiIns::handle_impl(char* data, size_t size, size_t nmemb) {
	content_.append(data, size * nmemb);
	return size * nmemb;
}

curlMultiIns::curlMultiIns(void) {
	content_ = "";
}

string DownWWW(string link, string postdata = "", string referer = "", string cookieFile = "", string userAgent = "Mozilla/5.0 (compatible msie 9.0 windows nt 6.1 wow64 trident/5.0)") {
	curlMultiIns CMI;
	CMI.curl = curl_easy_init();
	if (CMI.curl) {
		curl_easy_setopt(CMI.curl, CURLOPT_URL, link.c_str());
		curl_easy_setopt(CMI.curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(CMI.curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(CMI.curl, CURLOPT_MAXCONNECTS, 100);
		curl_easy_setopt(CMI.curl, CURLOPT_MAXREDIRS, 100);

		curl_easy_setopt(CMI.curl, CURLOPT_WRITEFUNCTION, &curlMultiIns::handle);
		curl_easy_setopt(CMI.curl, CURLOPT_WRITEDATA, &CMI);
		curl_easy_setopt(CMI.curl, CURLOPT_CONNECTTIMEOUT, 30);
		curl_easy_setopt(CMI.curl, CURLOPT_TIMEOUT, 60);
		curl_easy_setopt(CMI.curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(CMI.curl, CURLOPT_USERAGENT, userAgent.c_str());

		if (referer.length()>1) curl_easy_setopt(CMI.curl, CURLOPT_REFERER, referer.c_str()); // Referer

		// Ciasteczko
		if (cookieFile.length()>1) {
			curl_easy_setopt(CMI.curl, CURLOPT_COOKIEFILE, cookieFile.c_str());
			curl_easy_setopt(CMI.curl, CURLOPT_COOKIEJAR, cookieFile.c_str());
		}

		// Post
		if (postdata.length()>2) {
			char *postthis = new char[postdata.size() + 1];
			strcpy_s(postthis, strlen(postthis), postdata.c_str());
			curl_easy_setopt(CMI.curl, CURLOPT_POSTFIELDS, postthis);
			curl_easy_setopt(CMI.curl, CURLOPT_POSTFIELDSIZE, strlen(postthis));
		}

		CMI.result = curl_easy_perform(CMI.curl);
		curl_easy_cleanup(CMI.curl);
		if (CMI.result == CURLE_OK) return (string)CMI.content_;
		else return (string)"ERROR";
	}
	else return (string)"ERROR";
}
