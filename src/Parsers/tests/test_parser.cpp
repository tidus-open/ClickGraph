#include <Parsers/ExpressionListParsers.h>
#include <Parsers/TokenIterator.h>
#include <Parsers/IParser.h>
#include <IO/WriteBufferFromOStream.h>
#include <Interpreters/TreeRewriter.h>
#include <Interpreters/ExpressionAnalyzer.h>
#include <Functions/registerFunctions.h>
#include <Interpreters/Context.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/DataTypeString.h>
#include <Interpreters/ExpressionActions.h>
#include <Processors/Chunk.h>
#include <Processors/Transforms/FilterTransform.h>
#include <Core/Block.h>
#include <Columns/ColumnsNumber.h>
#include <Interpreters/InterpreterSelectQuery.h>
#include <Parsers/ParserSelectQuery.h>
#include <Databases/DatabaseMemory.h>
#include <Storages/StorageMemory.h>
#include <Interpreters/ActionsVisitor.h>
#include <Parsers/DumpASTNode.h>
#include <DataStreams/SizeLimits.h>

#include <Parsers/IParserBase.h>
#include <Parsers/CommonParsers.h>
#include <Parsers/ExpressionElementParsers.h>
#include <Parsers/ExpressionListParsers.h>




using namespace DB;

using LogAST = DebugASTLog<false>;

int main()
{
	registerFunctions();
	SharedContextHolder shared_context = Context::createShared();
	Context global_context= Context::createGlobal(shared_context.get());
	global_context.makeGlobalContext();

	NamesAndTypesList columns {
		{"id", std::make_shared<DataTypeUInt64>()},
		{"name", std::make_shared<DataTypeString>()},
		{"age", std::make_shared<DataTypeUInt8>()},
	};

	StoragePtr storage = StorageMemory::create(StorageID("test", "table"), ColumnsDescription{columns}, ConstraintsDescription{});
	DatabasePtr database = std::make_shared<DatabaseMemory>("test", global_context);
	database->attachTable("test", storage);
	DatabaseCatalog::instance().attachDatabase("test", database);
	global_context.setCurrentDatabase("test");

	String selectQuery = " (id + 10) * 2";
	
//	ParserSelectQuery parser;
//	ASTPtr ast;
	WriteBufferFromOStream buf(std::cerr, 4096);
	

//	ast = parseQuery(parser, selectQuery.data(), selectQuery.data() +  selectQuery.size(), "", 262144, 1000);

	Tokens tokens(selectQuery.data(), selectQuery.data() +  selectQuery.size(), 1000);
	IParser::Pos token_iterator(tokens, 1000);

//	auto select_query = std::make_shared<ASTSelectQuery>();
//

	Expected expected;	

	ASTPtr select_expression_list;
	
	ParserKeyword s_select("SELECT");
	
	ParserNotEmptyExpressionList exp_list_for_select_clause(true);
	
//	if(!s_select.ignore(token_iterator, expected))
//		return 0;

	if(!exp_list_for_select_clause.parse(token_iterator, select_expression_list, expected))
		return 0;

	select_expression_list->dumpTree(buf);

//	ast->dumpTree(buf);
	
	
	return 0;
}
