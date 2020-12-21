#include <Parsers/ClickGraph/ParserCypherAliasAndLabel.h>
#include <Parsers/ClickGraph/ASTCypherAliasAndLabel.h>

namespace DB
{

bool ParserCypherAliasAndLabel::parseImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
	auto get = [&](String & alias) -> bool {
		String label;
		if(pos->type != TokenType::Colon)
		{
			label = "all_label";
		} else {
		
			++pos;

			if(pos->type != TokenType::BareWord)
			{
				expected.add(pos, " label name");
				return false;
			}

			label.append(pos->begin, pos->end);
			++pos;
		}

		auto alias_and_label = std::make_shared<ASTCypherAliasAndLabel>();
		alias_and_label->alias = std::move(alias);
		alias_and_label->label = std::move(label);
		node = alias_and_label;
		return true;
	};
	
	if(pos->type == TokenType::BareWord)
	{
		String alias(pos->begin, pos->end);
		++pos;	

		if(get(alias))
			return true;
		else
			return false;
	}

	if(pos->type == TokenType::Colon)
	{
		String alias("anon_");

		if(get(alias))
			return true;
		else
			return false;

	}

	return false;
}

};
