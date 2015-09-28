//============================================================================
// Author      : pierre BLAVY
// Version     : 1.0
// Copyright   : 2012 LGPL 3.0 or any later version : https://www.gnu.org/licenses/lgpl-3.0-standalone.html
//============================================================================


/**
 * \file BConfig.h
 * \brief A simple configuration file library
 * 		The input file is the description of a tree composed of BConfig (i.e., leaves).
 * 		BConfig are identified by a that can map to 0,1 or more BConfig (i.e., childrens).
 * 		BConfig contains values identified by a key that can map to 0,1 or more values.
 * \author {Pierre BLAVY}
 * \version {1.0.2}
 * \date {28-september-2015}
 */

#ifndef BCONFIG_H_
#define BCONFIG_H_

//needed for header
#include <deque>
#include <string>
#include <unordered_map>

#include <exception>
#include <istream>






/**\brief Base class for BConfig errors, used for catch(Error_BConfig_base &e);.
 * Do not throw this class, use derived instead*/
struct Error_BConfig_base: std::exception{
	protected:

	/**\param msg_ const std::string&. Error message*/
	Error_BConfig_base(const std::string& msg_) throw():msg("Error_BConfig : "){msg += msg_;}

	/**\brief default constructor (do nothing)*/
	Error_BConfig_base() throw(){}

	/**\brief destructor (do nothing)*/
	virtual ~Error_BConfig_base() throw(){}

	/**\return error message*/
	virtual const char* what() const throw(){return msg.c_str();}

	std::string msg; /*!<The error message. */
};


/**\brief error thrown when the input file is invalid*/
struct Error_BConfig_parse: Error_BConfig_base{

	/** \param msg_ const std::string&. Error message
	 *  \param config_file_ const std::string&, default ="". Config file path, "" for unknown
	 *  \param line_ size_t, default = 0. Line number of the error, 0 for unknown
	 */
	Error_BConfig_parse(const std::string& msg_,const std::string &config_file_="", size_t line_=0) throw();

	/**\return error message*/
	virtual const char* what() const throw(){return msg.c_str();}

	/**\brief destructor (do nothing)*/
	virtual ~Error_BConfig_parse() throw(){}
};


/** \brief error thrown when a value cannot be get */
struct Error_BConfig_get: Error_BConfig_base{

	/**\param msg_ const std::string&. Error message
	 * \param key const std::string&. Key used for getting value*/
	Error_BConfig_get(const std::string& msg_,const std::string &key) throw(){msg="Error_BConfig_get : " + msg_ + " key=" + key;}

	/**\param msg_ const std::string&. Error message
	 * \param key const std::string&. Key used for getting value
	 * \param value const std::string&. The gotten value
	 */
	Error_BConfig_get(const std::string& msg_,const std::string &key, const std::string & value) throw(){msg="Error_BConfig_get : " + msg_ + " key=" + key + " value="+value;}
	virtual const char* what() const throw(){return msg.c_str();}/*!<\return error message*/
	virtual ~Error_BConfig_get() throw(){}/*!<\brief destructor (do nothing)*/
};


/**\brief error thrown on a bad conversion (see BConfig::convert)*/
struct Error_BConfig_convert: Error_BConfig_base{

	/** \param msg_ const std::string&. Error message
	 * \param ref_str_ const std::string&. The string used for conversion
	 */
	Error_BConfig_convert(const std::string& msg_,const std::string &ref_str_) throw(){msg="Error_BConfig_convert : " + msg_ + " string=" + ref_str_;}
	virtual const char* what() const throw(){return msg.c_str();}/*!<\return error message*/
	virtual ~Error_BConfig_convert() throw(){}/*!<\brief destructor (do nothing)*/
};





/**\brief A simple configuration file library
 * 		The input file is the description of a tree composed of BConfig (i.e., leaves).
 * 		BConfig are identified by a that can map to 0,1 or more BConfig (i.e., childrens).
 * 		BConfig contains values identified by a key that can map to 0,1 or more values.
 */
struct BConfig{

	BConfig()               =default;/*!<\brief construct an empty BConfig. use BConfig::parse to fill it.*/
	BConfig(const BConfig &)=default;/*!<\brief efault copy constructor.*/
	~BConfig()              =default;/*!<\brief default destructor*/

	/** \brief Load a file located at path, see BConfig::parse for detail
	 * \param path const std::string &. Input file path*/
	explicit BConfig(const std::string &path){parse(path);}

	/**\brief load from std::istream, see BConfig::parse for detail
	 * \param path_description const std::string &, default = "".Input file path description, used only for throwing explicit errors.
	 * \param in std::istream &. Read config from this std::istream.
	 */
	explicit BConfig(std::istream &in,const std::string &path_description="" ){parse(in,path_description);}

	/**\return true if exactly one value for key, false otherwise
	 * \param key const std::string &. The key*/
	bool has_unique_value(const std::string &key)const;

	/**\return true if one or more value for key, false if no value for key
	 * \param key const std::string &. The key*/
	bool has_value       (const std::string &key)const;



	/**
	 * \tparam return_t The type of value to get. Values are converted from std::string to return_t by BConfig::convert.
	 * \param key const std::string &. The key
	 * \throw Error_BConfig_get if no values.
	 * \return a not empty std::deque<return_t> containing the values associated to the key in the current blockk in the same order as they appear in input file
	 */
	template< typename return_t = std::string>
	const std::deque<return_t> &get_values(const std::string &key,bool do_throw=true)const;

	/**\brief convenience wrapper for const char* key, see BConfig::get_values<return_t>(const std::string &key)const*/
	template< typename return_t = std::string>
	const std::deque<return_t> &get_values(const char *key,bool do_throw=true)const{return get_values<return_t>(std::string(key),do_throw);}


	/**
	 * \tparam return_t The type of value to get. Values are converted from std::string to return_t by BConfig::convert.
	 * \param key const std::string &. The key.
	 * \throw Error_BConfig_get if not exactly one value.
	 * \throw Error_BConfig_convert if conversion fails (see BConfig::convert).
	 * \return the unique value associated to key in the current blockk
	 */
	template< typename return_t = std::string>
	return_t get_unique_value(const std::string &key) const;

	/**\brief convenience wrapper for const char* key, see BConfig::get_unique_value<return_t>(const std::string &key)const*/
	template< typename return_t = std::string>
	return_t get_unique_value(const char* &key) const{return get_unique_value<return_t>(std::string(key));}



	/**
	 * \tparam return_t The type of value to get. Values are converted from std::string to return_t by BConfig::convert.
	 * \param key const std::string &. The key
	 * \param default_v const return_t &, the default value to return if the key is missing.
	 * \throw Error_BConfig_get if more than one value.
	 * \throw Error_BConfig_convert if conversion fails (see BConfig::convert).
	 * \return default_v if key is missing, or a unique value otherwise.
	 */
	template< typename return_t = std::string>
	return_t get_unique_value(const std::string &key, const return_t &default_v)const;

	/**\brief convenience wrapper for const char* key, see BConfig::get_unique_value<return_t>(const std::string &key,return_t & default_v)const*/
	template< typename return_t = std::string>
	return_t get_unique_value(const char* key, const return_t &default_v)const{return get_unique_value<return_t>(std::string(key),default_v);}


	/**
	 * \param key const std::string &. The key
	 * \param throw_b bool. If true throw a Error_BConfig_get error if 0 sub-BConfig are found.
	 * \throw Error_BConfig_get if throw_b==true and 0  sub-BConfig are found.
	 * \return std::deque<BConfig> containing the sub-BConfig in the same order as they appear in input file
	 */
	const std::deque<BConfig> get_blocks (const std::string &key, bool throw_b=true)const;

	/**
	 * \param key const std::string &. The key
	 * \throw Error_BConfig_get if not exactly one sub-BConfig are found.
	 * \return the unique sub-BConfig associated to the key.
	 */
	const BConfig get_unique_block(const std::string &key)const;


	/**
	 * \param key const std::string &. The key
	 * \throw Error_BConfig_get if if key is missing
	 * \throw Error_BConfig_get if the associated value is neither "yes","y","no" nor "n".
	 * \throw Error_BConfig_get if there is more than one value for the key
	 * \return true for "yes" or "y" , false for "no" or "n"
	 */
	bool get_yes_no(const std::string &key)const;


	/**
	 * \param key const std::string &. The key.
	 * \param default_v bool. The default value to return if key is missing or if value is an empty string.
	 * \throw Error_BConfig_get if the associated value is neither "yes","y","no" nor "n".
	 * \throw Error_BConfig_get if there is more than one value for the key
	 * \return true for "yes" or "y" , false for "no" or "n", or default_v if key is missing or value is an empty string
	 */
	bool get_yes_no(const std::string &key,bool default_v)const;





	/**
	 * \brief load a file.
	 * \param path const std::string &. Filepath to config file
	 * \throw Error_BConfig_parse if file is invalid
	 */
	void parse(const std::string & path);

	/**
	 * \brief load a file.
	 * \param in std::istream &. Read config file from this flux.
	 * \param path const std::string &. Filepath to config file, path is used as optional parameter to throw readable errors.
	 * \throw Error_BConfig_parse if file is invalid
	 */
	void parse(std::istream &in, const std::string &path="");


	/**
	 * \brief Convert a std::string to return_t. Specialize this function to handle your own types. If conversion fails : throw Error_BConfig_convert
	 * \tparam return_t. std::string are converted to this type.
	 * \param s const std::string &. The source string for conversion
	 * \throw Error_BConfig_parse if file is invalid
	 */
	template< typename return_t> static return_t    convert(const std::string &s);

	/**
	 * \param out std::ostream &. Where to print, used for debug
	 * \param indent_v std::ostream &. Indentation level
	 * \brief write the parsed filed in a valid format, usefull for debug
	 */
	void print(std::ostream &out, size_t indent_v=0)const;


private:
	static const std::deque<BConfig>     &empty_blocks(){static std::deque<BConfig> i;return i;}
	static const std::deque<std::string> &empty_values(){static std::deque<std::string> i;return i;}


	static void indent(std::ostream &out, size_t s){for(size_t i = 0;i<s;++i){out << "  ";}}

	typedef std::pair<std::string, BConfig > key_block_t;
	std::unordered_map<std::string, std::deque<std::string> > values; //key_values : values in a blockk are unordered
	std::deque< key_block_t >     blocks ; //blockks are ordered


};



//inline & template code
#include "BConfig.tpp"


//--------------------------------
//Explicit template instantiation
//--------------------------------
template std::string BConfig::convert(const std::string &s);
template double      BConfig::convert(const std::string &s);
template float       BConfig::convert(const std::string &s);
template size_t      BConfig::convert(const std::string &s);

#endif /* BCONFIG_H_ */
