/*
 * BConfig_convert.hpp
 *
 *  Created on: 8 oct. 2015
 *      Author: pierre
 */

#ifndef HELPERS_BCONFIG_CONVERT_HPP_
#define HELPERS_BCONFIG_CONVERT_HPP_


#include "BConfig_error.hpp"


//conversion stuff goes here
namespace bconfig{
	/**\brief Specialize this struct to define custom conversions from std::string to Target_tt
	 * The default implementation delegates convertion to operator<<(std::istream&, Target_tt&)
	 * \tparam Target_tt the type to convert to
	 * \throw Error_BConfig_convert when the conversion is invalid
	 * */
	template<typename Target_tt> struct Convert_t{
		static Target_tt run(const std::string &s){
			try{
			Target_tt R;
			std::stringstream strStream(s);
			strStream >> R;
			return R;
			}catch(std::exception &e){
				throw Error_BConfig_convert(std::string("Error in conversion, error=") +  e.what(), s);
			}catch(...){
				throw Error_BConfig_convert("Error in conversion", s);
			}
		}
	};

	template<> struct Convert_t<std::string>{
			static const std::string &run(const std::string &s){return s;}
	};

	/**\brief convert a std::string to Target_tt
	 * specialize Convert_t to extend this function
	 * \tparam Target_tt the type to convert to
	 *
	 */
	template<typename Target_tt> Target_tt convert(const std::string &s){return Convert_t<Target_tt>::run(s);}
}//end namespace bconfig







#endif /* HELPERS_BCONFIG_CONVERT_HPP_ */
