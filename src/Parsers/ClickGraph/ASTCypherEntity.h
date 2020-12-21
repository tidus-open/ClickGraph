#pragma once

#include <Parsers/IAST.h>
#include <Parsers/ClickGraph/ASTCypherMapList.h>
#include <Parsers/ClickGraph/ASTCypherAliasAndLabel.h>

namespace DB
{

class ASTCypherEntity : public IAST
{
public:
	void addAliasAndLabel(const ASTPtr & alias_and_lable_) 
	{
		children.emplace_back(alias_and_lable_);
	}

	void addMapList(const ASTPtr & map_list_)
	{
		children.emplace_back(map_list_);	
	}

	void addAnonAliasAndAllLabel()
	{
		auto alias_and_label = std::make_shared<ASTCypherAliasAndLabel>();
		alias_and_label->alias = "anon_";
		alias_and_label->label = "all_label";
		children.emplace_back(alias_and_label);
	}

};

using ASTCypherEntityPtr = std::shared_ptr<ASTCypherEntity>;

};



