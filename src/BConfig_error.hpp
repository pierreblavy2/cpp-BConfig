/*
 * BConfig_error.hpp
 *
 *  Created on: 8 oct. 2015
 *      Author: pierre
 */

#ifndef BCONFIG_ERROR_HPP_
#define BCONFIG_ERROR_HPP_

#include <exception>
#include <string>


namespace bconfig{
/**\brief Base class for BConfig errors, used for catch(Error_BConfig_base &e);.
 * Do not throw this class, use derived instead*/
struct Error_BConfig_base: std::exception{
	protected:


	/**\param msg_  Error message
	 *  */

	Error_BConfig_base(
			const std::string& msg_
	):msg(msg_){}

	/**\brief destructor (do nothing)*/
	virtual ~Error_BConfig_base() throw(){}

	/**\return error message*/
	virtual const char* what() const throw(){
		std::string R=msg;
		return R.c_str();
	}

	std::string msg;    /*!<The error message. */
	std::string file="";/*!<Optional : the file */
	size_t line  =0;    /*!<Optional : line number, 0=unknown*/
	size_t column=0;    /*!<Optional : column number, 0=unknown*/
};


/**\brief error thrown when the input file is invalid*/
struct Error_BConfig_parse: Error_BConfig_base{
	/**\param msg_  Error message
	 * \param file_ Parsed file path,   ""=unknown
	 * \param line_ Parsed file line,   0=unknown
	 * \param column_ Parsed file column, 0=unknown
	 *  */
	explicit Error_BConfig_parse(
			const std::string& msg_     ,
			const std::string& file_ ="",
			const size_t line_       =0,
			const size_t column_     =0
	):Error_BConfig_base(msg_),file(file_),line(line_),column(column_){}

	virtual const char* what() const throw(){
		std::string R=Error_BConfig_base::what();
		if(file  !=""){R +=", file="  +file;}
		if(line  !=0 ){R +=", line="  +std::to_string(line);}
		if(column!=0 ){R +=", column="+std::to_string(column);}
		return R.c_str();
	}

	std::string file  ="";
	size_t      line  =0;
	size_t      column=0;
};


/** \brief error thrown when a value cannot be get */
struct Error_BConfig_get: Error_BConfig_base{

	std::string key;
	std::string value;

	/**\param msg_  Error message
	 * \param key_   Key used for getting value
	 * \param value_  Retrieved value,  ""=unknown or empty
	*/
	Error_BConfig_get(
			const std::string &msg_,
			const std::string &key_,
			const std::string &value_=""
	) throw():Error_BConfig_base(msg_),key(key_),value(value_){}

	/**\param msg_ const std::string&. Error message
	 * \param key const std::string&. Key used for getting value
	 * \param value const std::string&. The gotten value
	 */
	virtual const char* what() const throw(){
		std::string R = Error_BConfig_base::what();
		if(key!=""){R+=", key="+key;}
		if(value!=""){R+=", value="+value;}
		return R.c_str();
	}/*!<\return error message*/
};


/**\brief error thrown on a bad conversion (see BConfig::convert)*/
struct Error_BConfig_convert: Error_BConfig_base{
	std::string from;

	/**\param msg_  Error message
	 * \param from_ The string used for conversion
	 */
	Error_BConfig_convert(
			const std::string &msg_,
			const std::string &from_
	):Error_BConfig_base(msg_),from(from_){};

	virtual const char* what() const throw(){
		std::string R = Error_BConfig_base::what();
		R+=", from="+from;
		return R.c_str();
	}

};

}//end namespace bconfig








#endif /* BCONFIG_ERROR_HPP_ */
