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

#include <Parsers/ASTInsertQuery.h>
#include <Parsers/ParserInsertQuery.h>
#include <Interpreters/InterpreterInsertQuery.h>
#include <Processors/printPipeline.h>
#include <Processors/Pipe.h>
#include <Processors/QueryPipeline.h>
#include <Interpreters/Context.h>
#include <Formats/registerFormats.h>

#include <Columns/IColumn.h>
#include <Columns/ColumnsNumber.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/IDataType.h>
#include <Core/Block.h>
#include <Columns/ColumnString.h>
#include <DataTypes/DataTypeString.h>





using namespace DB;

using LogAST = DebugASTLog<false>;

int main()
{
	registerFunctions();
	registerFormats();
	SharedContextHolder shared_context = Context::createShared();
	Context global_context= Context::createGlobal(shared_context.get());
	global_context.makeGlobalContext();
	global_context.setApplicationType(Context::ApplicationType::LOCAL);

	NamesAndTypesList columns {
		{"a", std::make_shared<DataTypeUInt32>()},
		{"b", std::make_shared<DataTypeUInt32>()},
		{"c", std::make_shared<DataTypeUInt32>()},
	};

	StoragePtr storage = StorageMemory::create(StorageID("test", "table"), ColumnsDescription{columns}, ConstraintsDescription{});
	DatabasePtr database = std::make_shared<DatabaseMemory>("test", global_context);
	database->attachTable("test", storage);
	DatabaseCatalog::instance().attachDatabase("test", database);
	global_context.setCurrentDatabase("test");

	ColumnPtr col1 = ColumnUInt32::create({0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U});
	ColumnPtr col2 = ColumnUInt32::create({0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U});
	ColumnPtr col3 = ColumnUInt32::create({0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U});

	Block block = {ColumnWithTypeAndName(col1, std::make_shared<DataTypeUInt32>(), "a"),
			ColumnWithTypeAndName(col2, std::make_shared<DataTypeUInt32>(), "b"),
			ColumnWithTypeAndName(col3, std::make_shared<DataTypeUInt32>(), "c"),
		};

	

	String selectQuery = " insert into test(a,b,c) values";
	
//	ParserSelectQuery parser;
//	ASTPtr ast;
	WriteBufferFromOStream buf(std::cerr, 4096);
	

//	ast = parseQuery(parser, selectQuery.data(), selectQuery.data() +  selectQuery.size(), "", 262144, 1000);

	const char * start = selectQuery.data();
	const char * end = selectQuery.data() +  selectQuery.size();
	Tokens tokens(start, end, 1000);
	IParser::Pos token_iterator(tokens, 1000);

//	auto select_query = std::make_shared<ASTSelectQuery>();
//

	Expected expected;	

	ParserInsertQuery insertQueryParser(end);
	ASTPtr ast;
	if(!insertQueryParser.parseImpl(token_iterator, ast, expected))
		return -1;

	
	if(auto *insertAST = ast->as<ASTInsertQuery>())
		insertAST->dumpTree(buf);

	InterpreterInsertQuery interpreterInsert(ast, global_context);
	BlockIO blockIO = interpreterInsert.execute();
//	printPipeline(QueryPipeline::getPipe(std::move(blockIO.pipeline)).getProcessors(), buf);
//	Processors procs = QueryPipeline::getPipe(std::move(blockIO.pipeline)).getProcessors();
//	std::cout << procs.size() << std::endl; 
	blockIO.out->write(block);

//	ast->dumpTree(buf);
	
	
	return 0;
}
