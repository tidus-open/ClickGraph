#include <Parsers/ClickGraph/ASTCypherMapList.h>
#include <Parsers/ASTLiteral.h>
#include <Parsers/TokenIterator.h>
#include <Parsers/IParser.h>
#include <Parsers/parseQuery.h>
#include <Core/Field.h>
#include <IO/WriteBufferFromOStream.h>

#include <Parsers/ClickGraph/ParserCypherMapList.h>
#include <Parsers/ClickGraph/ParserCypherAliasAndLabel.h>
#include <Parsers/ClickGraph/ParserCypherEntity.h>

using namespace DB;

void testASTCypherMapList()
{
	auto map_list = std::make_shared<ASTCypherMapList>();
	ASTLiteral name("delphi");
	ASTLiteral x(34);
	WriteBufferFromOStream buf(std::cerr, 4096);
	map_list->addMap("name", name);
	map_list->addMap("age", x);
	map_list->dumpTree(buf);
}


ASTPtr testParserCypher(std::string query, IParser & parser)
{
	const char * start = query.data();
	const char * end = query.data() + query.size();

	WriteBufferFromOStream buf(std::cerr, 4096);
	std::string error_msg;
	ASTPtr map_list; 
	map_list = tryParseQuery(parser, start, end, error_msg,
			true, "", true, 1000, 10);
	if(map_list)
		map_list->dumpTree(buf);
	else
		std::cout << error_msg << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	return map_list;

}

ASTPtr testParserCypherMapList(std::string query)
{
	ParserCypherMapList cypherMapListParser;
	return testParserCypher(query, cypherMapListParser);
}

void testAllParserCypherMapList()
{
	assert(testParserCypherMapList("{name: 'delphi', age: 30}"));
	assert(!testParserCypherMapList("{name: 'delphi' age: 30}"));
	assert(!testParserCypherMapList("{name: 'delphi', age:}"));
	assert(!testParserCypherMapList("{name: 'delphi', age 30}"));
	assert(!testParserCypherMapList("{name: 'delphi', }"));
	assert(testParserCypherMapList("{name: 'delphi' }"));
	assert(!testParserCypherMapList("{name: 'delphi', age :30, country: China }"));
	assert(testParserCypherMapList("{name: 'delphi', age :30, country: 'China' }"));
	assert(!testParserCypherMapList("{name: \"delphi\", age :30, country: 'China' }"));
	assert(!testParserCypherMapList("{name: 'delphi, age :30, country: 'China' }"));
	assert(!testParserCypherMapList("{: 'delphi', age :30, country: 'China' }"));
}

ASTPtr testParserCypherAliasAndLabel(std::string query)
{
	ParserCypherAliasAndLabel cypherAliasAndLabelParser;
	return testParserCypher(query, cypherAliasAndLabelParser);
}

void testAllParserCypherAliasAndLabel()
{
	assert(testParserCypherAliasAndLabel("a:movie"));
	assert(testParserCypherAliasAndLabel(":movie"));
	assert(!testParserCypherAliasAndLabel("a:"));
	assert(testParserCypherAliasAndLabel("a"));
	assert(!testParserCypherAliasAndLabel("a,"));
}

ASTPtr testParserCypherNode(std::string query)
{
	ParserCypherNode cypherNodeParser;
	return testParserCypher(query, cypherNodeParser);
}

void testAllParserCypherNode()
{
	assert(testParserCypherNode("(a:movie)"));
	assert(testParserCypherNode("(a:movie {name: 'delphi', age: 34})"));
	assert(testParserCypherNode("(a {name: 'delphi', age: 34})"));
	assert(!testParserCypherNode("(a name: 'delphi', age: 34})"));
	assert(!testParserCypherNode("(a {name: 'delphi', age: 34)"));
	assert(!testParserCypherNode("(a {name: 'delphi', age: 34"));
	assert(testParserCypherNode("({name: 'delphi', age: 34})"));
	assert(testParserCypherNode("(:movie {name: 'delphi', age: 34})"));
	assert(!testParserCypherNode("(:movie name: 'delphi', age: 34})"));
	assert(!testParserCypherNode("(a:movie: {name: 'delphi', age: 34})"));
	assert(!testParserCypherNode("(a:movie {name: 'delphi', age: 34)"));
	assert(!testParserCypherNode("(a:movie {name: 'delphi', age: 34}"));

}

ASTPtr testParserCypherEdge(std::string query)
{
	ParserCypherEdge cypherEdgeParser;
	return testParserCypher(query, cypherEdgeParser);
}

void testAllParseCypherEdge()
{
	assert(testParserCypherEdge("[a:movie]"));
	assert(testParserCypherEdge("[a:movie {name: 'delphi', age: 34}]"));
	assert(testParserCypherEdge("[a {name: 'delphi', age: 34}]"));
	assert(!testParserCypherEdge("[a name: 'delphi', age: 34}]"));
	assert(!testParserCypherEdge("[a {name: 'delphi', age: 34]"));
	assert(!testParserCypherEdge("[a {name: 'delphi', age: 34"));
	assert(testParserCypherEdge("[{name: 'delphi', age: 34}]"));
	assert(testParserCypherEdge("[:movie {name: 'delphi', age: 34}]"));
	assert(!testParserCypherEdge("[:movie name: 'delphi', age: 34}]"));
	assert(!testParserCypherEdge("[a:movie: {name: 'delphi', age: 34}]"));
	assert(!testParserCypherEdge("[a:movie {name: 'delphi', age: 34]"));
	assert(!testParserCypherEdge("[a:movie {name: 'delphi', age: 34}"));

}

int main()
{
	testAllParserCypherMapList();
	testAllParserCypherAliasAndLabel();
	testAllParserCypherNode();
	testAllParseCypherEdge();
	return 0;
}
