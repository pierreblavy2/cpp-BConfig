//============================================================================
// Author      : pierre BLAVY
// Version     : 1.0
// Copyright   : 2012 LGPL 3.0 or any later version : https://www.gnu.org/licenses/lgpl-3.0-standalone.html
//============================================================================


#include "BConfig.hpp"
#include "helpers/OpenFile.h"
#include "helpers/str_tools.h"
#include "helpers/str_convert.h"

Error_BConfig_parse::Error_BConfig_parse(
		const std::string &msg_,
		const std::string &config_file_,
		size_t line_
)throw(){
	using namespace bconfig;
	this->msg="Error_BConfig_parse";
	if(config_file_!=""){msg+=": ";msg+= config_file_;}
	if(msg_!=""){msg+= " : ";msg+= msg_;}
	if(line_!=0){msg+=" line : "; msg+= str::toString(line_);}
}





const std::deque<BConfig> BConfig::get_blocks(const std::string &key, bool throw_b)const{
	//auto f = blocks.find(key);
	std::deque<BConfig> R;

	for(const auto &p : blocks){
		if(p.first==key){
			R.push_back(p.second);
		}
	}

	if(R.size()==0 and throw_b)	{throw Error_BConfig_get("Missing block", key);}
	else return R;
}



const BConfig BConfig::get_unique_block(const std::string &key)const{
	const auto &d= get_blocks(key,true);
	if(d.size()!=1){throw Error_BConfig_get("Multiple blocks", key);}
	return d[0];
}


bool BConfig::get_yes_no(const std::string &key)const{
	std::string s=get_unique_value<std::string>(key);
	if(s=="y" or s== "yes"){return true;}
	if(s=="n" or s== "no"){return false;}
	throw Error_BConfig_get("get_yes_no", key,s);
}


bool BConfig::get_yes_no(const std::string &key,bool default_v)const{
	std::string s=get_unique_value<std::string>(key,"");
	if(s=="")return default_v;
	if(s=="y" or s== "yes"){return true;}
	if(s=="n" or s== "no"){return false;}
	throw Error_BConfig_get("get_yes_no", key,s);
}




void BConfig::parse(std::istream &in, const std::string &path){
	using namespace bconfig;
	using namespace str;

	enum struct Mode{undefined,value,open_blockk, close_blockk,comment,empty_blockk};
	Mode mode=Mode::undefined;


	std::string l;


	size_t line_num=0;
	while(getline(in,l)){
		++line_num;
		str::trim(l," \t");
		if(l=="")continue;

		mode=Mode::undefined;

		std::string current_id="";
		std::string current_value="";

		for(char c : l){
			if(c=='#'){
				if(mode==Mode::undefined){mode=Mode::comment;}
				goto do_line;
			}

			if(mode==Mode::undefined){
				if(c=='='){mode=Mode::value      ;continue;}
				if(c=='{'){mode=Mode::open_blockk ;continue;}
				if(c=='}'){mode=Mode::close_blockk;continue;}
				current_id+=c;
			}

			if(mode==Mode::value)      {current_value +=c;}

			if(mode==Mode::open_blockk) {
				if(c == ' ' or c == '\t'){ continue;}
				if(c=='}'){mode=Mode::empty_blockk;continue;}
				throw Error_BConfig_parse("open_blockk" ,path,line_num);
			}

			if(mode==Mode::close_blockk){
				if(c == ' ' or c == '\t'){ continue;}
				throw Error_BConfig_parse("close_blockk",path,line_num);
			}

			if(mode==Mode::empty_blockk){
				if(c == ' ' or c == '\t'){ continue;}
				throw Error_BConfig_parse("empty_blockk",path,line_num);
			}
		}//end for char

		do_line:

		if(mode==Mode::comment){continue;} //full line is a comment
		trim(current_value," \t");
		trim(current_id," \t");

		if(mode==Mode::value){
			values[current_id].push_back(current_value);
			continue;
		}

		if(mode==Mode::open_blockk){
			BConfig new_block;

			//blocks[current_id].push_back(new_block);
			key_block_t k;
			k.first = current_id;
			k.second.parse(in,path);
			blocks.push_back(k);
			continue;
		}

		if(mode==Mode::empty_blockk){
			key_block_t k;
			k.first = current_id;
			blocks.push_back(k);
			continue;
		}


		if(mode==Mode::close_blockk){
			return;
		}

		if(mode==Mode::undefined){
			throw Error_BConfig_parse("undefined" ,path,line_num);
		}




	}
}


void BConfig::parse(const std::string & path){
	auto in = iOpenFile(path);
	parse(*in,path);
}


void BConfig::print(std::ostream &out, size_t indent_v)const{
	for(const auto &v : values){
		indent(out,indent_v);
		//out <<v.first<<":{";
		for(const auto &vv :v.second ){out <<v.first<<"="<< vv <<"\n";}
		//out << "}\n";
	}

	for(const auto &b : blocks){
		indent(out,indent_v);
		out << b.first <<"{\n";
		b.second.print(out,indent_v+1);
		indent(out,indent_v);
		out<<"}\n";
	}
}

