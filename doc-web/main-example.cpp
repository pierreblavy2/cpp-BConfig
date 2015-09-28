//============================================================================
// Name        : cpp-BConfig.cpp
// Author      : pierre BLAVY
// Version     : 1.0
// Copyright   : 2012 LGPL 3.0 or any later version : https://www.gnu.org/licenses/lgpl-3.0-standalone.html
//============================================================================

#include <iostream>
#include "BConfig.hpp"


int main(int ,char**) {

	BConfig forest("example.conf"); //parse file
	/* example.conf
		tree{
		 name = cool tree

		 trunk{
		   type = big and ugly
		   size = 10 #I'm a commant, size is in metters
		 }

		 branch{
		   broken=yes
		 }

		 branch{
		   broken=no
		   leave{} #default color = green


		   leave{
			 color=yellow
		   }

		   leave{
			 color=yellow
		   }
		 }
		}


		tree{
		 name = dead tree
		 trunk{
		   type = small
		   size = 2
		 }
		}
	*/

	std::deque<BConfig> all_trees = forest.get_blocks("tree",false);
	//tree is the block name
	//false means do not trow exception if there is no tree (i.e., the forest can be empty)

	for(const BConfig & tree : all_trees){
		std::cout <<"---Tree : " <<tree.get_unique_value("name")<<"---\n";
		const BConfig &trunk = tree.get_unique_block("trunk");//one single trunk per tree.

		std::cout <<"trunk_size : "<<trunk.get_unique_value<size_t>("size") << "\n";
		std::cout <<"trunk_type : "<<trunk.get_unique_value("type") << "\n"; //default : get std::string

		//for each branch
		const std::deque<BConfig> & all_branch = tree.get_blocks("branch",false);
		for(const BConfig & branch : all_branch){

			//check if the branch is broken
			bool broken = branch.get_yes_no("broken",false); //false = default : not_broken
			if(broken){	std::cout << "-broken_branch-\n"; continue;}
			std::cout << "-alive_branch-\n";

			//if the branch is not broken : read leaves
			const std::deque<BConfig> & all_leaves = branch.get_blocks("leave",false);
			for(const BConfig & leave : all_leaves){
				std::cout << "leave_color : " << leave.get_unique_value<std::string>("color","green")<<"\n";//default color is green
			}
		}
	}

	/*output :
	---Tree : cool tree---
	trunk_size : 10
	trunk_type : very big
	-broken_branch-
	-alive_branch-
	leave_color : green
	leave_color : yellow
	leave_color : yellow
	---Tree : dead tree---
	trunk_size : 2
	trunk_type : small
	 */



}
