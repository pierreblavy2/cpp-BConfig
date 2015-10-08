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
 * \version {2.0.0}, changes :
 *  - everything in namespace bconfig
 *  - a more homogenous interface
 *  - user can extend the conversion interface by specializing bconfig::Convert_t
 *  - clearer errors
 * \date {28-september-2015}
 */

#ifndef BCONFIG_H_
#define BCONFIG_H_

//needed for header
#include <deque>
#include <unordered_map>
#include <istream>

#include "BConfig_error.hpp"
#include "BConfig_convert.hpp"




namespace bconfig{

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
	bool has_values       (const std::string &key)const;

	/**\return the number of values for the key
	 * \param key const std::string &. The key*/
	size_t count_values(const std::string &key)const;




	/**
	 * \tparam return_t The type of value to get. Values are converted from std::string to return_t by BConfig::convert.
	 * \param key const std::string &. The key
	 * \throw Error_BConfig_get if no values.
	 * \return a not empty std::deque<return_t> containing the values associated to the key in the current blockk in the same order as they appear in input file
	 */
	template< typename return_t = std::string>
	const std::deque<return_t> get_values(const std::string &key,bool do_throw=true)const;

	/**\brief convenience wrapper for const char* key, see BConfig::get_values<return_t>(const std::string &key)const*/
	template< typename return_t = std::string>
	const std::deque<return_t> get_values(const char *key,bool do_throw=true)const{return get_values<return_t>(std::string(key),do_throw);}


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
	 * \return the number of blocks associated to key
	 */
	size_t count_blocks(const std::string &key)const;


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


}//end namespace bconfig



//inline & template code
#include "BConfig.tpp"


//--------------------------------
//Explicit template instantiation
//--------------------------------
//template std::string BConfig::convert(const std::string &s);
//template double      BConfig::convert(const std::string &s);
//template float       BConfig::convert(const std::string &s);
//template size_t      BConfig::convert(const std::string &s);

#endif /* BCONFIG_H_ */
