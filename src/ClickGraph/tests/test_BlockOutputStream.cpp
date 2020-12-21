#include <DataStreams/CountingBlockOutputStream.h>
#include <DataStreams/SquashingBlockOutputStream.h>
#include <DataStreams/AddingDefaultBlockOutputStream.h>
#include <Interpreters/Context.h>
#include <DataTypes/DataTypeString.h>
#include <DataTypes/DataTypesNumber.h>
#include <Columns/ColumnsNumber.h>

namespace DB
{

class MyTestBlockOutputStream : public IBlockOutputStream
{

public:

	MyTestBlockOutputStream(const Block & header_): header(header_) {}

	Block getHeader() const override { return header;}
	void write(const Block & block) override {
		std::cout << block.dumpStructure() << std::endl;
	}

private:

	const Block header;

};

};

using namespace DB;

int main()
{
	SharedContextHolder shared_context = Context::createShared();
        Context global_context= Context::createGlobal(shared_context.get());
        global_context.makeGlobalContext();
        global_context.setApplicationType(Context::ApplicationType::LOCAL);

	ColumnPtr col1 = ColumnUInt32::create({0U, 1U });
        ColumnPtr col2 = ColumnUInt32::create({0U, 1U });
        ColumnPtr col3 = ColumnUInt32::create({0U, 1U });

        Block block1 = {ColumnWithTypeAndName(col1, std::make_shared<DataTypeUInt32>(), "a"),
                        ColumnWithTypeAndName(col2, std::make_shared<DataTypeUInt32>(), "b"),
                        ColumnWithTypeAndName(col3, std::make_shared<DataTypeUInt32>(), "c"),
                };

	ColumnPtr col4 = ColumnUInt32::create({2U, 3U });
        ColumnPtr col5 = ColumnUInt32::create({2U, 3U });
        ColumnPtr col6 = ColumnUInt32::create({2U, 3U });

        Block block2 = {ColumnWithTypeAndName(col4, std::make_shared<DataTypeUInt32>(), "a"),
                        ColumnWithTypeAndName(col5, std::make_shared<DataTypeUInt32>(), "b"),
                        ColumnWithTypeAndName(col6, std::make_shared<DataTypeUInt32>(), "c"),
                };

	auto namesAndTypesList = block1.getNamesAndTypesList();


	
	BlockOutputStreamPtr out = std::make_shared<MyTestBlockOutputStream>(block1);

	out = std::make_shared<AddingDefaultBlockOutputStream>(out, out->getHeader(), out->getHeader(), ColumnsDescription(namesAndTypesList), global_context);

	out = std::make_shared<SquashingBlockOutputStream>(out, out->getHeader(), 10, 100);

	out = std::make_shared<CountingBlockOutputStream>(out);

	out->write(block1);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	out->write(block2);
	return 0;
}
