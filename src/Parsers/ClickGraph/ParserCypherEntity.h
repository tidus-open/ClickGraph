#pragma once

#include <Parsers/IParserBase.h>
#include <Parsers/Lexer.h>
#include <Parsers/ClickGraph/ASTCypherEntity.h>
#include <Parsers/ClickGraph/ASTCypherNode.h>
#include <Parsers/ClickGraph/ASTCypherEdge.h>

namespace DB
{
class ParserCypherEntity : public IParserBase
{
	TokenType opening_type ;	
	TokenType closing_type ;
	ASTCypherEntityPtr cypher_entity;
public:
	ParserCypherEntity(const TokenType opening_type_, const TokenType closing_type_, ASTCypherEntityPtr cypher_entity_): 
		opening_type(opening_type_), 
		closing_type(closing_type_), 
		cypher_entity(cypher_entity_) 
		{}
	const char * getName() const override { return "CypherEntity"; }
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override;
};

class ParserCypherNode : public ParserCypherEntity
{
public:
	ParserCypherNode(): 
		ParserCypherEntity(TokenType::OpeningRoundBracket, TokenType::ClosingRoundBracket, 
		std::dynamic_pointer_cast<ASTCypherEntity>(std::make_shared<ASTCypherNode>())) 
	{
	}

	const char * getName() const override { return "CypherNode"; }
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override 
	{
		return ParserCypherEntity::parseImpl(pos, node, expected);
	}
};

class ParserCypherEdge : public ParserCypherEntity
{
public:
	ParserCypherEdge():
		ParserCypherEntity(TokenType::OpeningSquareBracket, TokenType::ClosingSquareBracket, 
		std::dynamic_pointer_cast<ASTCypherEntity>(std::make_shared<ASTCypherEdge>()))
	{}
	
	const char * getName() const override { return "CypherEdge"; }
	bool parseImpl(Pos & pos, ASTPtr & node, Expected & expected) override
	{
		return ParserCypherEntity::parseImpl(pos, node, expected);
	}

};

};
