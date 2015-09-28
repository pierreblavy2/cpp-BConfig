/*
 * openFile.h
 *
 *  Created on: 23 juin 2011
 *      Author: pierre
 */

#ifndef OPENFILE_H_
#define OPENFILE_H_

#include <istream>
#include <ostream>
#include <memory>


struct Error_OpenFile : std::exception{
public:

	explicit Error_OpenFile(const std::string &f):p_msg("Cannot open file : " + f){}
    virtual const char* what() const throw(){return p_msg.c_str();}
	virtual ~Error_OpenFile() throw(){}
	protected:
    std::string p_msg;
    Error_OpenFile():p_msg(){}
};

struct Error_OpenFile_gz : Error_OpenFile{
public:
	explicit Error_OpenFile_gz(const std::string &f){p_msg = "Cannot open gz file : " + f;}
	virtual ~Error_OpenFile_gz() throw(){}
};


std::unique_ptr<std::ostream> oOpenFile(const std::string &path,bool throw_errors = true);
std::unique_ptr<std::istream> iOpenFile(const std::string &path,bool throw_errors = true);






#endif /* OPENFILE_H_ */
