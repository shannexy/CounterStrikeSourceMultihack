#include "netvar.hpp"

#include "console.hpp"
#include "../core/interfaces.hpp"

using namespace std;

void netvars::SetupNetvars() {
	for (auto clientClass = interfaces::client->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext)
		if (clientClass->m_pRecvTable)
			Dump(clientClass->m_pNetworkName, clientClass->m_pRecvTable);
}

void netvars::DumpClasses()
{
	ofstream output_file("classids.txt");
	if (output_file.is_open()) {
		output_file << "enum e_ClassDefinitions {" << std::endl;
		for (auto clientClass = interfaces::client->GetAllClasses(); clientClass; clientClass = clientClass->m_pNext)
		{
			output_file << std::format("\t{} = {},", clientClass->m_pNetworkName, clientClass->m_ClassID) << std::endl;
		}
		output_file << "}" << std::endl;
		output_file.close();
	}
}

const char* netvars::szConvertType(SendPropType type)
{
	switch (type)
	{
		case SendPropType::INT:
			return "int";
		case SendPropType::FLOAT:
			return "float";
		case SendPropType::VECTOR:
			return "vector";
		case SendPropType::STRING:
			return "string";
		case SendPropType::ARRAY:
			return "array";
		case SendPropType::DATATABLE:
			return "datatable";
		case SendPropType::INT64:
			return "int64";
	}
	return "Unknown";
}

void netvars::Dump(const char* baseClass, RecvTable* table, std::uint32_t offset)
{
	for (auto i = 0; i < table->propsCount; i++) {
		const auto prop = &table->props[i];

		if (!prop || isdigit(prop->varName[0]))
			continue;

		if (fnv::RunTime(prop->varName) == fnv::CompileTime("baseclass"))
			continue;

		if (prop->recvType == SendPropType::DATATABLE &&
			prop->dataTable &&
			prop->dataTable->tableName[0] == 'D')
		{
			Dump(baseClass, prop->dataTable, offset + prop->offset);
		}

		const auto netvarName = std::format("{}->{}", baseClass, prop->varName);

		data[fnv::RunTime(std::format("{}->{}", baseClass, prop->varName).c_str())] = offset + prop->offset;


		const auto propType = szConvertType(prop->recvType);

#ifdef _DEBUG
		console::print("Netvar: ", netvarName, "(", propType, ") = ", std::hex, data[fnv::RunTime(netvarName.c_str())]);
#endif
	}
}

std::uint32_t netvars::GetOffset(const char* baseclass, const char* propName)
{
	return data[fnv::RunTime(std::format("{}->{}", baseclass, propName).c_str())];
}
