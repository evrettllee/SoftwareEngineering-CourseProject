#include "pch.h"

namespace IT
{
	Entry::Entry()
	{
		idxfirstLE = 0;
		id[0] = '\0';
		scope[0] = '\0';
		iddatatype = VOID;
	}

	IdTable IT::Create(int size)
	{
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(123);
		}

		IdTable idtable;
		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];

		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= TI_MAXSIZE)
		{
			throw ERROR_THROW(124);
		}

		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		if (n < 0 || n > TI_MAXSIZE)
		{
			throw ERROR_THROW(125);
		}

		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[TI_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	int Search(IdTable& idtable, Entry entry)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(entry.id, idtable.table[i].id) == 0 && strcmp(entry.scope, idtable.table[i].scope) == 0)
			{
				return i;
			}
		}

		return TI_NULLIDX;
	}

	void WriteTable(IdTable& idtable)
	{
		std::ofstream ITfile(TI_FILENAME);

		if (!ITfile.is_open())
		{
			throw ERROR_THROW(115);
		}

		ITfile << std::left << "¹ Id " << '|'
			<< std::setw(16) << std::left << "Identifier" << '|'
			<< std::setw(10) << std::left << "Data type" << '|'
			<< std::setw(17) << std::left << "Identifier type" << '|'
			<< std::setw(13) << std::left << "Line in text" << '|'
			<< std::setw(16) << std::left << "Scope" << '|'
			<< std::setw(30) << std::left << "Value" << '|' << "\n";
		ITfile << std::setfill('-') << std::setw(114) << '-' << std::setfill(' ') << '\n';

		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry temp = IT::GetEntry(idtable, i);

			ITfile << std::setw(5) << std::left << i << '|';
			ITfile << std::setw(16) << std::left << temp.id << '|';
			switch (temp.iddatatype)
			{
			case IT::VOID:
				ITfile << std::setw(10) << std::left << "void" << '|';
				break;
			case IT::INT:
				ITfile << std::setw(10) << std::left << "int" << '|';
				break;
			case IT::UINT:
				ITfile << std::setw(10) << std::left << "uint" << '|';
				break;
			case IT::STRING:
				ITfile << std::setw(10) << std::left << "string" << '|';
				break;
			case IT::BOOL:
				ITfile << std::setw(10) << std::left << "boolean" << '|';
				break;
			}
			switch (temp.idtype)
			{
			case IT::V:
				ITfile << std::setw(17) << std::left << "variable" << '|';
				break;
			case IT::F:
				ITfile << std::setw(17) << std::left << "function" << '|';
				break;
			case IT::P:
				ITfile << std::setw(17) << std::left << "parametr" << '|';
				break;
			case IT::L:
				ITfile << std::setw(17) << std::left << "literal" << '|';
				break;
			case IT::A:
				ITfile << std::setw(17) << std::left << "array" << '|';
				break;
			}
			ITfile << std::setw(13) << std::left << temp.idxfirstLE << '|';
			ITfile << std::setw(16) << std::left << temp.scope << '|';
			switch (temp.idtype)
			{
			case IT::V:
				switch (temp.iddatatype)
				{
				case IT::INT:
					ITfile << std::setw(30) << std::left << temp.value.vint;
					break;
				case IT::UINT:
					ITfile << std::setw(30) << std::left << temp.value.vuint;
					break;
				case IT::STRING:
					ITfile << std::left << "\"" << temp.value.vstr.str << "\"(" << temp.value.vstr.len << ")";
					break;
				case IT::BOOL:
					ITfile << std::setw(30) << std::left << std::boolalpha << temp.value.vbool;
					break;
				}
				break;
			case IT::F:
				ITfile << std::setw(30) << std::left << "null";
				break;
			case IT::P:
				ITfile << std::setw(30) << std::left << "null";
				break;
			case IT::L:
				switch (temp.iddatatype)
				{
				case IT::INT:
					ITfile << std::setw(30) << std::left << temp.value.vint;
					break;
				case IT::UINT:
					ITfile << std::setw(30) << std::left << temp.value.vuint;
					break;
				case IT::STRING:
					ITfile << std::left << "\"" << temp.value.vstr.str << "\"(" << temp.value.vstr.len << ")";
					break;
				case IT::BOOL:
					ITfile << std::setw(30) << std::left << std::boolalpha << temp.value.vbool;
					break;
				}
				break;
			case IT::A:
				ITfile << std::setw(30) << std::left << temp.value.vuint;
				break;
			}
			ITfile << "\n";
		}
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		idtable.table = nullptr;
	}
}
