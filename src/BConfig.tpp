//============================================================================
// Author      : pierre BLAVY
// Version     : 1.0
// Copyright   : 2012 LGPL 3.0 or any later version : https://www.gnu.org/licenses/lgpl-3.0-standalone.html
//============================================================================


inline std::ostream & operator<< (std::ostream &out, const bconfig::BConfig & b){
	b.print(out,0);
	return out;
}




namespace bconfig{

	inline bool BConfig::has_unique_value(const std::string &key)const{
		auto f = values.find(key);
		if(f==values.end()){return false;}// key not found
		return f->second.size()==1;//value is unique
	}

	inline bool BConfig::has_values       (const std::string &key)const{
		auto   f = values.find(key);
		if(f==values.end()){return false;}// key not found
		return f->second.size()!=0;       //at least one value
	}

	inline size_t BConfig::count_values(const std::string &key)const{
		auto   f = values.find(key);
		if(f==values.end()){return 0;}// key not found -> 0
		return f->second.size();     // key found -> size
	}


	//generic get
	template< typename return_t>
	inline const std::deque<return_t> BConfig::get_values(const std::string &key, bool do_throw)const{
		std::deque<std::string> s = this->get_values<std::string>(key,do_throw);
		std::deque<return_t>    r;
		for(const auto &i:s){return_t rr = bconfig::convert<return_t>(i);r.push_back(rr);}
		return r;
	}


	//string get
	template<>
	inline const std::deque<std::string> BConfig::get_values(const std::string &key, bool do_throw)const{
		auto f = values.find(key);
		if(f==values.end()){
			if(do_throw){throw Error_BConfig_get("Missing value",key);}
			else{return empty_values();}
		}
		return f->second;
	}



	template< typename return_t>
	inline return_t bconfig::BConfig::get_unique_value(const std::string &key) const{
		std::string s = get_unique_value<std::string>(key);
		return bconfig::convert<return_t>(s);
	}

	template< typename return_t>
	inline return_t bconfig::BConfig::get_unique_value(const std::string &key, const return_t &default_v)const{
		if(! has_unique_value(key) ){return default_v;}
		std::string s = get_unique_value<std::string>(key);
		return bconfig::convert<return_t>(s);
	}

	template<>
	inline std::string bconfig::BConfig::get_unique_value(const std::string &key)const{
		auto d = get_values<std::string>(key);
		if(d.size()!=1){
			std::string vvv;
			for(const std::string & i : d){vvv +=" " +i;}
			throw Error_BConfig_get("Multiple values",key,vvv);}
		return d[0];
	}

	template<>
	inline std::string bconfig::BConfig::get_unique_value(const std::string &key, const std::string &default_v)const{
		auto f = values.find(key);
		if(f==values.end()){return default_v;}
		const auto &d = f->second;
		if(d.size()!=1){
			std::string vvv;
			for(const std::string & i : d){vvv +=" " +i;}
			throw Error_BConfig_get("Multiple values", key,vvv);
		}
		return d[0];
	}


}//end namespace bconfig

