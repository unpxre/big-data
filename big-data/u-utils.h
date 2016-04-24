

string search_replace(string String, string searchString, string replaceString, string::size_type pos = 0) {
	while ((pos = String.find(searchString, pos)) != string::npos) {
		String.replace(pos, searchString.size(), replaceString);
		pos += replaceString.size();
	}
	return String;
}

inline string u_trim(string v) {
	int cmd_str = v.find_first_not_of(" \t\f\v\n\r");
	v = v.substr(cmd_str, (v.length() - cmd_str));
	cmd_str = v.find_last_not_of(" \t\f\v\n\r");
	return v.erase(cmd_str + 1);
}

string u_substr(string start_cut, string finish_cut, string data, int jmod = 0, string jumpto = "") { //u_substr("<a href=\"", "\">", data, 10, "<div id=menu>");

	if (data.length()<3) return "ERR: len";
	if (jumpto != "") jmod += data.find(jumpto);
	if ((data.find(start_cut, jmod) == string::npos) || (data.find(finish_cut, jmod) == string::npos)) return "ERR: not found strat or end cut";
	unsigned int scut = data.find(start_cut, jmod) + start_cut.length();
	unsigned int fcut = data.find(finish_cut, jmod);
	jmod = fcut + finish_cut.length();
	if ((scut >= fcut) || (fcut >= data.length()))  return "ERR: bad order";
	return u_trim(data.substr(scut, (fcut - scut)));
}
