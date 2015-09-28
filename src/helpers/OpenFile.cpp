/*
 * openFile.cpp
 *
 *  Created on: 23 juin 2011
 *      Author: pierre Blavy
 */

#include "OpenFile.h"
#include <fstream>
#include <utility>
#include <memory>


#ifdef GZSTREAM_SUPPORT
#include "../lib/gzstream/gzstream.h"
#endif


namespace{
#ifdef GZSTREAM_SUPPORT
	bool openFile_hasExtention(std::string const &source, std::string const &end) {
		if(source.length() < end.length()){return false;}
		return source.substr(source.length() - end.length()) == end;
	}
#endif

	bool openFile_isValid(const std::unique_ptr<std::ostream> &R){
		return R == nullptr or (*R);
	}

	bool openFile_isValid(const std::unique_ptr<std::istream> &R){
		return R != nullptr and (*R);
	}
}


std::unique_ptr<std::ostream> oOpenFile(const std::string &path,bool throw_errors){

#ifdef GZSTREAM_SUPPORT
	//gz
	if(openFile_hasExtention(path,".gz")){
		std::unique_ptr<std::ostream> R( new ogzstream(path.c_str()));
		if( openFile_isValid(R) or !throw_errors ){return std::move(R);}
		else{throw Error_OpenFile_gz(path);}

	}
#endif

	//other
	std::unique_ptr<std::ostream> R( new std::ofstream (path.c_str()));
	if(openFile_isValid(R) or !throw_errors ){return move(R);}
	else{throw Error_OpenFile(path);}

}


std::unique_ptr<std::istream> iOpenFile(const std::string &path,bool throw_errors){
#ifdef GZSTREAM_SUPPORT
	//gz
	if(openFile_hasExtention(path,".gz")){
		std::unique_ptr<std::istream> R( new igzstream(path.c_str()));
		if(openFile_isValid(R)or !throw_errors ){return std::move(R);}
		else{throw Error_OpenFile_gz(path);}
	}
#endif

	//other
	std::unique_ptr<std::istream> R( new std::ifstream (path.c_str()));
	if(openFile_isValid(R)or !throw_errors ){return std::move(R);}
	else{throw Error_OpenFile(path);}

}




/* --- TESTING CODE ---
#include <iostream>

int main() {

	{
	std::unique_ptr<std::ostream> out=oOpenFile("out/test.gz");
	(*out) << "OpenFile TEST OK";
	}

	{
	std::unique_ptr<std::istream> in=iOpenFile("out/test.gz");
	std::string l;
	while(!in->eof()){
		getline(*in,l);
		std::cout << l <<"\n";
	}
	}

return 0;
}
*/

