#pragma once 

#include <Parsers/IAST.h>

namespace DB
{

class ASTCypherMap : public IAST
{
public:
	String prop_name;
	//ASTLiteral value;

	explicit ASTCypherMap(const String & prop_name_, const ASTLiteral & value_): prop_name(std::move(prop_name_))
	{
		auto value = std::make_shared<ASTLiteral>(value_);
		children.emplace_back(value);
	} 	

	String getID(char delim) const override { return "CypherMap" + (delim + prop_name); }

	ASTPtr clone() const override { return std::make_shared<ASTCypherMap>(*this); }

};

};
