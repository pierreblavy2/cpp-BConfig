/*
 * StringConvert.h
 *
 *  Created on: 7 mars 2012
 *      Author: pierre
 */

#ifndef STRINGCONVERT_H_
#define STRINGCONVERT_H_

#include <string>
#include <algorithm>
#include <sstream>




namespace bconfig{
namespace str{


	//from string : convert a string to the template parameter
	template<typename T>
	inline T fromString(const std::string &s){
		T R;
		std::stringstream strStream(s);
		strStream >> R;
		return R;
	}

	template<>
	inline std::string fromString(const std::string &s){return s;}


	template<>
	inline std::wstring fromString(const std::string &s){
			std::wstring temp(s.length(),L' ');
			std::copy(s.begin(), s.end(), temp.begin());
			return temp;
	}







	//to string : convert a T to a std::string
	template<typename T>
	inline std::string toString(T const &t){
		std::ostringstream oss;
		oss << t;
		return oss.str();
	}

	template<>
	inline std::string toString(const std::string &s){return s;}

	template<>
	inline std::string toString(const std::wstring& s){
		std::string temp(s.length(), ' ');
		std::copy(s.begin(), s.end(), temp.begin());
		return temp;
	}



}}//end namespace BConfig::str




#endif /* STRINGCONVERT_H_ */
