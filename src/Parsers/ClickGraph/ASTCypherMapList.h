#pragma once

#include <Parsers/IAST.h>
#include <Parsers/ASTLiteral.h>
#include <Parsers/ClickGraph/ASTCypherMap.h>
#include <Core/Field.h>
#include <vector>

namespace DB
{

class ASTCypherMapList : public IAST
{
public:

	ASTCypherMapList() = default;
	
	void addMap(const String & prop_name, const ASTLiteral & value) 
	{
		auto map = std::make_shared<ASTCypherMap>(prop_name, value);
		children.emplace_back(map);	
	}

	String getID(char delimiter) const override { return "ASTCypherMapList" + (delimiter + String(""));}
	
	ASTPtr clone() const override {return std::make_shared<ASTCypherMapList>(*this);}
};

};
