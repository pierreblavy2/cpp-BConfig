/*
 * stringtools.h
 *
 *  Created on: 6 mai 2010
 *      Author: pierre Blavy
 *      GPL 3.0
 */

#ifndef STRINGTOOLS_H_
#define STRINGTOOLS_H_

#include <string>
#include <algorithm>
#include <sstream>
#include <deque>






namespace bconfig{
namespace str{

//--- begins end with ---
inline bool beginWith(std::string const &source, std::string const &end) {
	if(source.length() < end.length()){return false;}
	return source.substr(0, end.length() ) == end;
}

inline bool endWith(std::string const &source, std::string const &end) {
	if(source.length() < end.length()){return false;}
	return source.substr(source.length() - end.length()) == end;
}


//--- remove fist, last  ---

inline void removeLast(std::string &s,char c){
	if(s[s.length()-1] == c){
		s=s.substr(0,s.length()-1);
	}
}


inline void removeFirst(std::string &s,char c){
	if(s.length()<1){return;}
	if(s[0] == c){
		s=s.substr(1);
	}
}


//---Trim---

inline void trim_right(std::string &source, const std::string &t= " ") {
	source.erase(source.find_last_not_of(t)+1);
}

inline void trim_left(std::string &source, const std::string &t = " ") {
	source.erase(0, source.find_first_not_of(t));
}

inline void trim(std::string &source, const std::string &t = " ") {
	trim_right(source,t);
	trim_left(source,t);
}



//--- casse ---

inline void toUpper(std::string &s){
	std::transform(s.begin(), s.end(),
	s.begin(), ::toupper);
}


inline void toLower(std::string &s){
	std::transform(s.begin(), s.end(),
	s.begin(), ::tolower);
}



//--- replace ---


inline void replace(std::string &s, char oldone, char newone)
{
	for(size_t i=0;i<s.length();++i){
		if(s[i]==oldone){s[i] = newone;}
	}

}

//Source : http://stackoverflow.com/questions/1452501/string-replace-in-c
inline void replace(std::string& value, std::string const& search,std::string const& replace){
	using namespace std;

    std::string::size_type  next;

    for(next = value.find(search);        // Try and find the first match
        next != std::string::npos;        // next is npos if nothing was found
        next = value.find(search,next)    // search for the next match starting after
                                          // the last match that was found.
       )
    {
        // Inside the loop. So we found a match.
        value.replace(next,search.length(),replace);   // Do the replacement.
        next += replace.length();                      // Move to just after the replace
                                                       // This is the point were we start
                                                       // the next search from.
    }

}




///--- tokenize ---


inline std::deque<std::string> tokenize(const std::string &s ,char sep){
	std::string token;
	std::deque<std::string> R;
	std::istringstream iss(s);
	while ( getline(iss, token, sep) )
	{
	R.push_back(token);
	}
	return R;
}



inline std::deque<std::string> tokenize(const std::string &s ,const std::string & sep){
	std::deque<std::string> R;
	if(sep==""){R.push_back(s); return R;}

	std::string token;
	for(size_t i=0; i< s.length() - sep.size() +1 ;++i){
		char c = s[i];
		if(s.substr(i,sep.size()) == sep  ){
			R.push_back(token); token="";
		}
		token+=c;
	}

	if(token !="") R.push_back(token);


	return R;
}



inline std::deque<std::string> tokenizeMultipleSep(std::string const & s, std::string const & seps){
	using namespace std;
	deque<string> R;

	string token;
	for(size_t i = 0; i<s.length();++i){
		char c=s[i];
		if(seps.find(c)!=string::npos){//separator
			if(token!=""){ R.push_back(token);}
			token="";
			continue;
		}

		token+=c;

	}
	if(token!=""){ R.push_back(token);}
	return R;
}





//--- find all ---

inline std::deque<size_t> findAll(const std::string &s,const std::string &toFind){
	using namespace std;
	std::deque<size_t> R;
	size_t pos= s.find(toFind);
	while (pos != string::npos ){
		R.push_back(pos);
		pos = s.find(toFind,pos+1);
	}
	return R;
}




//--- delChar ---
inline void delChar(std::string &s ,char sep){
	//TODO make a more efficient code without tokenize dependency
	std::deque<std::string> d = tokenize(s,sep);
	s="";
	for(size_t i=0;i<d.size();++i){
	s+=d[i];
	}
}



//--- getBetween ---
inline std::string getBetween(std::string const &s, size_t begin, size_t end){
	return s.substr(begin+1, end-begin);
}


//--- comments ---
inline void cutComment(std::string&s, char c = '#'){s=tokenize(s,c)[0];trim(s);}



}}//end namespace BConfig::str

#endif /* STRINGTOOLS_H_ */
