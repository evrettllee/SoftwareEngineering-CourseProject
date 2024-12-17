#include "pch.h"

namespace GEN
{
	void Generation(LA::LEX lex, Out::OUT& out)
	{
		Head(out);
		Const(lex, out);
		Data(lex, out);
		Code(lex, out);
	}

	void Head(Out::OUT& out)
	{
		(*out.stream) << ".586\n";
		(*out.stream) << ".model flat, stdcall\n";
		(*out.stream) << "includelib kernel32.lib\n";
		(*out.stream) << "includelib libucrt.lib\n";
		(*out.stream) << "includelib ../Debug/IPP-2024L.lib\n";
		(*out.stream) << "includelib ../Debug/IPP-2024ASML.lib\n\n";
		(*out.stream) << "ExitProcess proto : dword\n";
		(*out.stream) << "SetConsoleTitleA proto: dword\n";
		(*out.stream) << "GetStdHandle proto: dword\n";
		(*out.stream) << "WriteConsoleA proto: dword, : dword, : dword, : dword, : dword\n";
		(*out.stream) << "SetConsoleOutputCP proto : dword\n";
		(*out.stream) << "SetConsoleCP proto : dword\n";
		(*out.stream) << "overflow_error_message proto\n";
		(*out.stream) << "division_by_zero_error_message proto\n";
		(*out.stream) << "index_error_message proto\n";
		(*out.stream) << "GetStringArrayElement proto\n";
		(*out.stream) << "GetIntArrayElement proto\n";
		(*out.stream) << "PrintUnsignedInt proto : dword\n";
		(*out.stream) << "PrintUnsignedIntNewLine proto : dword\n";
		(*out.stream) << "PrintInt proto : dword\n";
		(*out.stream) << "PrintIntNewLine proto : dword\n";
		(*out.stream) << "PrintBoolean proto : dword\n";
		(*out.stream) << "PrintBooleanNewLine proto : dword\n";
		(*out.stream) << "PrintConsole proto : dword\n";
		(*out.stream) << "PrintConsoleNewLine proto : dword\n";
		(*out.stream) << "extrn TIME : proc\n";
		(*out.stream) << "extrn DATE : proc \n\n";
		(*out.stream) << ".stack 4096\n\n";
	}

	void Const(LA::LEX lex, Out::OUT& out)
	{
		(*out.stream) << ".const\n";
		(*out.stream) << "consoleTitle byte \"IPP-2024\", 0\n";
		(*out.stream) << "strPause byte \"pause\", 0\n";
		(*out.stream) << "emptyString byte \" \", 0\n";
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::L)
			{
				(*out.stream) << lex.idtable.table[i].id;
				switch (lex.idtable.table[i].iddatatype)
				{
				case IT::STRING:
					(*out.stream) << " byte \"" << lex.idtable.table[i].value.vstr.str << "\", 0\n";
					break;
				case IT::UINT:
					(*out.stream) << " dword " << lex.idtable.table[i].value.vuint << "\n";
					break;
				case IT::INT:
					(*out.stream) << " sdword " << lex.idtable.table[i].value.vint << "\n";
					break;
				case IT::BOOL:
					(*out.stream) << " dword " << lex.idtable.table[i].value.vbool << "\n";
					break;
				}
			}
		}
		(*out.stream) << "\n";
	}

	void Data(LA::LEX lex, Out::OUT& out)
	{
		char* scopedName = nullptr;
		(*out.stream) << ".data\n";
		for (int i = 0; i < lex.idtable.size; i++)
		{
			if (lex.idtable.table[i].idtype == IT::V)
			{
				CreateNameWithScope(scopedName, lex.idtable.table[i]);
				(*out.stream) << scopedName;
				switch (lex.idtable.table[i].iddatatype)
				{
				case IT::STRING:
					(*out.stream) << " dword offset emptyString\n";
					break;
				case IT::UINT:
					(*out.stream) << " dword " << lex.idtable.table[i].value.vuint << "\n";
					break;
				case IT::INT:
					(*out.stream) << " sdword " << lex.idtable.table[i].value.vint << "\n";
					break;
				case IT::BOOL:
					(*out.stream) << " dword " << lex.idtable.table[i].value.vbool << "\n";
					break;
				}
			}
			else if (lex.idtable.table[i].idtype == IT::A)
			{
				CreateNameWithScope(scopedName, lex.idtable.table[i]);
				(*out.stream) << scopedName;
				int arrSize = lex.idtable.table[i].value.vuint;
				int pos = 0;
				for (pos; pos < lex.lextable.size; pos++)
				{
					if (lex.lextable.table[pos].sn == lex.idtable.table[i].idxfirstLE							&&
						lex.lextable.table[pos].lexema == LEX_ID												&&
						strcmp(lex.idtable.table[lex.lextable.table[pos].idxTI].id, lex.idtable.table[i].id) == 0)
					{
						break;
					}
				}
				
				if(lex.lextable.table[pos + 1].lexema == LEX_SEMICOLON)
				{
					switch (lex.idtable.table[i].iddatatype)
					{
					case IT::STRING:
						(*out.stream) << " dword ";
						for (int i = 0; i < arrSize; i++)
						{
							(*out.stream) << "offset emptyString";
							if (i < arrSize - 1)
							{
								(*out.stream) << ", ";
							}
						}
						(*out.stream) << "\n";
						break;
					case IT::UINT:
						(*out.stream) << " dword " << arrSize << " dup (" << TI_UINT_DEFAULT << ")\n";
						break;
					case IT::INT:
						(*out.stream) << " sdword " << arrSize << " dup (" << TI_UINT_DEFAULT << ")\n";
						break;
					case IT::BOOL:
						(*out.stream) << " dword " << arrSize << " dup (" << TI_BOOL_DEFAULT << ")\n";
						break;
					}
				}
				else
				{
					switch (lex.idtable.table[i].iddatatype)
					{
					case IT::STRING:
						(*out.stream) << " dword ";
						break;
					case IT::UINT:
						(*out.stream) << " dword " << " ";
						break;
					case IT::INT:
						(*out.stream) << " sdword " << " ";
						break;
					case IT::BOOL:
						(*out.stream) << " dword " << " ";
						break;
					}

					for (int j = pos + 2; lex.lextable.table[j].lexema != LEX_SEMICOLON && lex.lextable.table[j].lexema != PN_FILLER; j++)
					{
						switch (lex.idtable.table[lex.lextable.table[j].idxTI].iddatatype)
						{
						case IT::STRING:
							(*out.stream) << "offset " << lex.idtable.table[lex.lextable.table[j].idxTI].id;
							break;
						case IT::UINT:
							(*out.stream) << lex.idtable.table[lex.lextable.table[j].idxTI].value.vuint;
							break;
						case IT::INT:
							(*out.stream) << lex.idtable.table[lex.lextable.table[j].idxTI].value.vint;
							break;
						case IT::BOOL:
							(*out.stream) << lex.idtable.table[lex.lextable.table[j].idxTI].value.vbool;
							break;
						}
						if (lex.lextable.table[j + 1].lexema != LEX_SEMICOLON && lex.lextable.table[j + 1].lexema != PN_FILLER)
						{
							(*out.stream) << ", ";
						}
					}
					(*out.stream) << "\n";
				}
			}
		}
		(*out.stream) << "\n";
	}

	void Code(LA::LEX lex, Out::OUT& out)
	{
		(*out.stream) << ".code\n";
		bool isMain = false;
		bool isFunction = false;
		bool isIf = false;
		bool isElse = false;
		bool isCycle = false;
		bool isFirstOperand = true;
		bool isSigned = false;
		char* scopedName = nullptr;
		char* currentFunctionName = nullptr;
		int resultPosition = 0;
		int returnMemorySize = 0;
		int counterIfElse = 0;
		int counterWhile = 0;
		std::stack<std::string> codeBlocks;
		std::stack<LT::Entry> params;
		std::vector<int> paramsMemorySize;

		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)
			{
			case LEX_FUNCTION:
				isFunction = true;
				CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
				(*out.stream) << scopedName << " proc";
				currentFunctionName = new char[strlen(scopedName)];
				strncpy(currentFunctionName, scopedName, strlen(scopedName));
				currentFunctionName[strlen(scopedName)] = '\0';
				scopedName = nullptr;

				if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].value.vint != 0)
				{
					(*out.stream) << ", ";
				}

				while (lex.lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					if (lex.lextable.table[i].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::P)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
						(*out.stream) << scopedName << " : ";
						switch (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype)
						{
						case IT::BOOL:
						case IT::STRING:
						case IT::UINT:
							paramsMemorySize.push_back(4);
							(*out.stream) << "dword";
							break;
						case IT::INT:
							paramsMemorySize.push_back(4);
							(*out.stream) << "sdword";
							break;
						}
					}
					if (lex.lextable.table[i].lexema == LEX_COMMA)
					{
						*(out.stream) << ", ";
					}
					i++;
				}
				(*out.stream) << "\n";
				break;
			case LEX_MAIN:
				isMain = true;
				(*out.stream) << "main proc\n";
				(*out.stream) << "\tpush 1251d\n";
				(*out.stream) << "\tcall SetConsoleOutputCP\n";
				(*out.stream) << "\tpush 1251d\n";
				(*out.stream) << "\tcall SetConsoleCP\n\n";
				(*out.stream) << "\tpush offset consoleTitle\n";
				(*out.stream) << "\tcall SetConsoleTitleA\n";
				(*out.stream) << "\tpush -11\n";
				(*out.stream) << "\tcall GetStdHandle\n\n";
				break;
			case LEX_EQUAL_SIGN:
				if (lex.lextable.table[i - 1].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i - 1].idxTI].idtype == IT::A)
				{
					break;
				}
				else if ((lex.lextable.table[i - 1].lexema == LEX_LITERAL || lex.lextable.table[i - 1].lexema == LEX_ID) &&
						  lex.lextable.table[i - 3].lexema == LEX_ID && lex.lextable.table[i - 3].idxTI != TI_NULLIDX	 &&
						  lex.idtable.table[lex.lextable.table[i - 3].idxTI].idtype == IT::A)
				{
					resultPosition = i - 3;
				}
				else
				{
					resultPosition = i - 1;
				}
				i++;

				if (lex.idtable.table[lex.lextable.table[resultPosition].idxTI].iddatatype == IT::INT)
				{
					isSigned = true;
				}

				if ( lex.lextable.table[i + 1].lexema == PN_FILLER || lex.lextable.table[i + 1].lexema == LEX_SEMICOLON ||
					(lex.lextable.table[i].lexema == LEX_ID																&& 
					(lex.lextable.table[i + 1].lexema == LEX_LITERAL || lex.lextable.table[i + 1].lexema == LEX_ID)		&&
					 lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::A									    &&
					 lex.lextable.table[i + 2].lexema == PN_FILLER || lex.lextable.table[i + 2].lexema == LEX_SEMICOLON))
				{
					(*out.stream) << "\txor eax, eax\n";
					if (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype != IT::STRING)
					{
						if (lex.lextable.table[i].lexema == LEX_LITERAL)
						{
							(*out.stream) << "\tmov eax, ";
							(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id;

							if (!isSigned)
							{
								(*out.stream) << "\n\tcmp eax, 0\n";
								(*out.stream) << "\tjl OVERFLOW_ERROR\n";
							}

							i += 1;
						}
						else
						{
							if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::A)
							{
								CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
								(*out.stream) << "\tpush lengthof " << scopedName << "\n";
								(*out.stream) << "\tpush offset " << scopedName << "\n";
								if(lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype != IT::L)
								{
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
									(*out.stream) << "\tpush " << scopedName << "\n";
								}
								else
								{
									(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
								}
								(*out.stream) << "\tcall GetIntArrayElement";

								if (!isSigned)
								{
									(*out.stream) << "\n\tmov ebx, [eax]\n";
									(*out.stream) << "\tcmp ebx, 0\n";
									(*out.stream) << "\tjl OVERFLOW_ERROR\n";
								}

								i += 2;
							}
							else
							{
								(*out.stream) << "\tmov eax, ";
								CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
								(*out.stream) << scopedName;
								if (!isSigned)
								{
									(*out.stream) << "\n\tcmp eax, 0\n";
									(*out.stream) << "\tjl OVERFLOW_ERROR\n";
								}
								i += 1;
							}
						}
					}
					else
					{
						if (lex.lextable.table[i].lexema == LEX_LITERAL)
						{
							(*out.stream) << "\tmov eax, ";
							(*out.stream) << "offset " << lex.idtable.table[lex.lextable.table[i].idxTI].id;
							i += 1;
						}
						else
						{
							if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::A)
							{
								CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
								(*out.stream) << "\tpush lengthof " << scopedName << "\n";
								(*out.stream) << "\tpush offset " << scopedName << "\n";
								if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype != IT::L)
								{
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
									(*out.stream) << "\tpush " << scopedName << "\n";
								}
								else
								{
									(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
								}
								(*out.stream) << "\tcall GetStringArrayElement\n";
								i += 2;
							}
							else
							{
								CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
								(*out.stream) << "\tmov eax, " << scopedName;
								i += 1;
							}
						}
					}
					(*out.stream) << "\n";
					scopedName = nullptr;
				}

				while (lex.lextable.table[i].lexema != PN_FILLER && lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_LITERAL:
						if (isFirstOperand)
						{
							(*out.stream) << "\txor eax, eax\n";
							if (lex.lextable.table[i].lexema == LEX_LITERAL)
							{
								(*out.stream) << "\tmov eax, ";
								(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id;
							}
							else
							{
								if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::A)
								{
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
									(*out.stream) << "\tpush lengthof " << scopedName << "\n";
									(*out.stream) << "\tpush offset " << scopedName << "\n";
									if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype != IT::L)
									{
										CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
										(*out.stream) << "\tpush " << scopedName << "\n";
									}
									else
									{
										(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
									}
									(*out.stream) << "\tcall GetIntArrayElement\n";
									(*out.stream) << "\tpush [eax]\n";
									(*out.stream) << "\tpop eax\n";
									i += 1;
								}
								else
								{
									(*out.stream) << "\tmov eax, ";
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
									(*out.stream) << scopedName;
								}
							}
							(*out.stream) << "\n";
							scopedName = nullptr;
							isFirstOperand = false;
						}
						else
						{
							(*out.stream) << "\tpush eax\n";
							if (lex.lextable.table[i].lexema == LEX_LITERAL)
							{
								(*out.stream) << "\tmov eax, ";
								(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id;
							}
							else
							{
								if (lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::A)
								{
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
									(*out.stream) << "\tpush lengthof " << scopedName << "\n";
									(*out.stream) << "\tpush offset " << scopedName << "\n";
									if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype != IT::L)
									{
										CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
										(*out.stream) << "\tpush " << scopedName << "\n";
									}
									else
									{
										(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
									}
									(*out.stream) << "\tcall GetIntArrayElement\n";
									(*out.stream) << "\tpush [eax]\n";
									(*out.stream) << "\tpop eax\n";
									i += 1;
								}
								else
								{
									(*out.stream) << "\tmov eax, ";
									CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
									(*out.stream) << scopedName;
								}
							}
							(*out.stream) << "\n";
							scopedName = nullptr;
						}
						break;
					case LEX_PARAMS:
						if(!isFirstOperand)
						{
							(*out.stream) << "\tpush eax\n";
						}
						else
						{
							(*out.stream) << "\txor eax, eax\n";
							isFirstOperand = false;
						}

						i++;
						while (lex.idtable.table[lex.lextable.table[i].idxTI].idtype != IT::F)
						{
							params.push(lex.lextable.table[i]);
							i++;
						}

						while (!params.empty())
						{
							(*out.stream) << "\tpush ";
							if(lex.idtable.table[params.top().idxTI].iddatatype != IT::STRING)
							{
								if (params.top().lexema == LEX_LITERAL)
								{
									(*out.stream) << lex.idtable.table[params.top().idxTI].id << "\n";
								}
								else
								{
									CreateNameWithScope(scopedName, lex.idtable.table[params.top().idxTI]);
									(*out.stream) << scopedName << "\n";
									scopedName = nullptr;
								}
							}
							else
							{
								if (params.top().lexema == LEX_LITERAL)
								{
									(*out.stream) << " offset " << lex.idtable.table[params.top().idxTI].id << "\n";
								}
								else
								{
									CreateNameWithScope(scopedName, lex.idtable.table[params.top().idxTI]);
									(*out.stream) << scopedName << "\n";
									scopedName = nullptr;
								}
							}
							params.pop();
						}

						if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "DATE") != 0 &&
							strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "TIME") != 0)
						{
							CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
							(*out.stream) << "\tcall " << scopedName << "\n";
							scopedName = nullptr;
						}
						else
						{
							(*out.stream) << "\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id << "\n";
						}
						break;
					case LEX_PLUS:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tadd eax, ebx\n";
						(*out.stream) << "\tjo OVERFLOW_ERROR\n";
						break;
					case LEX_MINUS:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tsub ebx, eax\n";
						(*out.stream) << "\tjo OVERFLOW_ERROR\n";
						(*out.stream) << "\tmov eax, ebx\n";
						break;
					case LEX_STAR:
						(*out.stream) << "\tpop ebx\n";
						if(isSigned)
						{
							(*out.stream) << "\timul ebx\n";
						}
						else
						{
							(*out.stream) << "\tmul ebx\n";
						}
						(*out.stream) << "\tjo OVERFLOW_ERROR\n";
						break;
					case LEX_DIRSLASH:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp eax, 0\n";
						(*out.stream) << "\tje DIVISION_BY_ZERO_ERROR\n";
						(*out.stream) << "\tpush eax\n";
						(*out.stream) << "\tmov eax, ebx\n";
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\txor edx, edx\n";
						if (isSigned)
						{
							(*out.stream) << "\tcdq\n";
							(*out.stream) << "\tidiv ebx\n";
						}
						else
						{
							(*out.stream) << "\tcmp eax, 0\n";
							(*out.stream) << "\tjl OVERFLOW_ERROR\n";
							(*out.stream) << "\tcmp ebx, 0\n";
							(*out.stream) << "\tjl OVERFLOW_ERROR\n";
							(*out.stream) << "\tdiv ebx\n";
						}
						(*out.stream) << "\tjo OVERFLOW_ERROR\n";
						break;
					case LEX_PERCENT:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp eax, 0\n";
						(*out.stream) << "\tje DIVISION_BY_ZERO_ERROR\n";
						(*out.stream) << "\tpush eax\n";
						(*out.stream) << "\tmov eax, ebx\n";
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\txor edx, edx\n";
						if (isSigned)
						{
							(*out.stream) << "\tcdq\n";
							(*out.stream) << "\tidiv ebx\n";
						}
						else
						{
							(*out.stream) << "\tcmp eax, 0\n";
							(*out.stream) << "\tjl OVERFLOW_ERROR\n";
							(*out.stream) << "\tcmp ebx, 0\n";
							(*out.stream) << "\tjl OVERFLOW_ERROR\n";
							(*out.stream) << "\tdiv ebx\n";
						}
						(*out.stream) << "\tjo OVERFLOW_ERROR\n";
						(*out.stream) << "\tmov eax, edx\n";
						break;
					case LEX_MORE:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsetg al\n";        
						(*out.stream) << "\tmovzx eax, al\n"; 
						break;
					case LEX_LESS:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsetl al\n";         
						(*out.stream) << "\tmovzx eax, al\n";  
						break;
					case LEX_MORE_OR_EQUAL:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsetge al\n";        
						(*out.stream) << "\tmovzx eax, al\n";  
						break;
					case LEX_LESS_OR_EQUAL:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsetle al\n";        
						(*out.stream) << "\tmovzx eax, al\n";  
						break;
					case LEX_EQUAL:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsete al\n";        
						(*out.stream) << "\tmovzx eax, al\n";  
						break;
					case LEX_NOT_EQUAL:
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tcmp ebx, eax\n";
						(*out.stream) << "\tsetne al\n";        
						(*out.stream) << "\tmovzx eax, al\n"; 
						break;
					}
					i++;
				}

				CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[resultPosition].idxTI]);

				if(lex.idtable.table[lex.lextable.table[resultPosition].idxTI].idtype != IT::A)
				{
					if (lex.idtable.table[lex.lextable.table[resultPosition].idxTI].iddatatype == IT::UINT)
					{
						(*out.stream) << "\tcmp eax, 0\n";
						(*out.stream) << "\tjl OVERFLOW_ERROR\n";
					}
					(*out.stream) << "\tmov " << scopedName << ", eax\n";
				}
				else
				{
					if (lex.idtable.table[lex.lextable.table[resultPosition].idxTI].iddatatype != IT::STRING)
					{
						(*out.stream) << "\tpush eax\n";
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[resultPosition].idxTI]);
						(*out.stream) << "\tpush lengthof " << scopedName << "\n";
						(*out.stream) << "\tpush offset " << scopedName << "\n";
						if (lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI].idtype != IT::L)
						{
							CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI]);
							(*out.stream) << "\tpush " << scopedName << "\n";
						}
						else
						{
							(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI].id << "\n";
						}
						(*out.stream) << "\tcall GetIntArrayElement\n";
						(*out.stream) << "\tpop ebx\n";
						if (lex.idtable.table[lex.lextable.table[resultPosition].idxTI].iddatatype == IT::UINT)
						{
							(*out.stream) << "\tcmp ebx, 0\n";
							(*out.stream) << "\tjl OVERFLOW_ERROR\n";
						}
						(*out.stream) << "\tmov [eax], ebx\n";
					}
					else
					{
						(*out.stream) << "\tpush eax\n";
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[resultPosition].idxTI]);
						(*out.stream) << "\tpush lengthof " << scopedName << "\n";
						(*out.stream) << "\tpush offset " << scopedName << "\n";
						if (lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI].idtype != IT::L)
						{
							CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI]);
							(*out.stream) << "\tpush " << scopedName << "\n";
						}
						else
						{
							(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[resultPosition + 2].idxTI].id << "\n";
						}
						(*out.stream) << "\tcall GetStringArrayElement\n";
						(*out.stream) << "\tpop ebx\n";
						(*out.stream) << "\tmov [eax], ebx\n";
					}
				}
				isSigned = false;
				isFirstOperand = true;
				scopedName = nullptr;
				break;
			case LEX_RETURN:
				if (isFunction)
				{
					(*out.stream) << "\tjmp NO_ERRORS\n\n";
					(*out.stream) << "OVERFLOW_ERROR:\n";
					(*out.stream) << "\tcall overflow_error_message\n\n";
					(*out.stream) << "DIVISION_BY_ZERO_ERROR:\n";
					(*out.stream) << "\tcall division_by_zero_error_message\n\n";
					(*out.stream) << "NO_ERRORS:\n";
				}
				(*out.stream) << "\tmov eax, ";
				i++;

				if (lex.lextable.table[i].lexema == LEX_LITERAL)
				{
					switch (lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].value.vint << "\n";
						break;
					case IT::INT:
						(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].value.vint << "\n";
						break;
					case IT::BOOL:
						(*out.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].value.vbool << "\n";
						break;
					case IT::STRING:
						(*out.stream) << "offset " << lex.idtable.table[lex.lextable.table[i].idxTI].id << "\n";
						break;
					}
				}
				else
				{
					CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i].idxTI]);
					(*out.stream) << scopedName << "\n";
					scopedName = nullptr;
				}

				for (int i = 0; i < paramsMemorySize.size(); i++)
				{
					returnMemorySize += paramsMemorySize[i];
				}


				if (!paramsMemorySize.empty())
				{
					(*out.stream) << "\tret " << returnMemorySize << "\n";
					paramsMemorySize.clear();
					returnMemorySize = 0;
				}
				else
				{
					(*out.stream) << "\tret\n";
				}
				break;
			case LEX_RIGHTBRACE:
				if (isMain && !isIf && !isElse && !isCycle)
				{
					(*out.stream) << "\tjmp NO_ERRORS\n\n";
					(*out.stream) << "\nOVERFLOW_ERROR:\n";
					(*out.stream) << "\tcall overflow_error_message\n\n";
					(*out.stream) << "DIVISION_BY_ZERO_ERROR:\n";
					(*out.stream) << "\tcall division_by_zero_error_message\n\n";
					(*out.stream) << "NO_ERRORS:\n";
					(*out.stream) << "\tpush 0\n";
					(*out.stream) << "\tcall ExitProcess\n";
					(*out.stream) << "main endp\n\n";
					(*out.stream) << "end main";
					isMain = false;
				}
				if (isFunction && !isIf && !isElse && !isCycle)
				{
					(*out.stream) << currentFunctionName << " endp\n\n";
					isFunction = false;
					currentFunctionName = nullptr;
				}
				if (isCycle && !codeBlocks.empty() && codeBlocks.top() == "while")
				{
					(*out.stream) << "\tjmp WHILE_" << counterWhile << "\n";
					(*out.stream) << "END_WHILE_" << counterWhile << ":\n";
					isCycle = false;
					counterWhile++;
				}
				if (isIf && !codeBlocks.empty() && codeBlocks.top() == "if")
				{
					(*out.stream) << "\tjmp IF_" << counterIfElse << "\n";
					isIf = false;
					codeBlocks.pop();
				}
				if (isElse && !codeBlocks.empty() && codeBlocks.top() == "else")
				{
					(*out.stream) << "IF_" << counterIfElse << ":\n";
					isElse = false;
					codeBlocks.pop();
					counterIfElse++;
				}
				break;
			case LEX_WRITE:
				if (lex.lextable.table[i + 1].lexema == LEX_LITERAL)
				{
					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintUnsignedInt\n";
						break;
					case IT::INT:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintInt\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintBoolean\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintConsole\n";
						break;
					}
				}
				else if(lex.lextable.table[i + 1].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::A)
				{
					CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
					(*out.stream) << "\tpush lengthof " << scopedName << "\n";
					(*out.stream) << "\tpush offset " << scopedName << "\n";
					if(lex.idtable.table[lex.lextable.table[i + 3].idxTI].idtype != IT::L)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tpush " << scopedName << "\n";
					}
					else
					{
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
					}

					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype != IT::STRING)
					{
						(*out.stream) << "\tcall GetIntArrayElement\n";
						(*out.stream) << "\tpush [eax]";
					}
					else
					{
						(*out.stream) << "\tcall GetStringArrayElement\n";
						(*out.stream) << "\tpush [eax]";
					}
					(*out.stream) << "\n";

					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tcall PrintUnsignedInt\n";
						break;
					case IT::INT:
						(*out.stream) << "\tcall PrintInt\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tcall PrintBoolean\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tcall PrintConsole\n";
						break;
					}

					i += 1;
				}
				else
				{
					CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintUnsignedInt\n";
						break;
					case IT::INT:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintInt\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintBoolean\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintConsole\n";
						break;
					}
				}
				break;
			case LEX_WRITELINE:
				if (lex.lextable.table[i + 1].lexema == LEX_LITERAL)
				{
					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintUnsignedIntNewLine\n";
						break;
					case IT::INT:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintIntNewLine\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintBooleanNewLine\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 1].idxTI].id << "\n";
						(*out.stream) << "\tcall PrintConsoleNewLine\n";
						break;
					}
				}
				else if (lex.lextable.table[i + 1].lexema == LEX_ID && lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::A)
				{
					CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
					(*out.stream) << "\tpush lengthof " << scopedName << "\n";
					(*out.stream) << "\tpush offset " << scopedName << "\n";
					if (lex.idtable.table[lex.lextable.table[i + 3].idxTI].idtype != IT::L)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tpush " << scopedName << "\n";
					}
					else
					{
						(*out.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
					}

					if (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype != IT::STRING)
					{
						(*out.stream) << "\tcall GetIntArrayElement\n";
						(*out.stream) << "\tpush [eax]";
					}
					else
					{
						(*out.stream) << "\tcall GetStringArrayElement\n";
						(*out.stream) << "\tpush [eax]";
					}
					(*out.stream) << "\n";

					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tcall PrintUnsignedIntNewLine\n";
						break;
					case IT::INT:
						(*out.stream) << "\tcall PrintIntNewLine\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tcall PrintBooleanNewLine\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tcall PrintConsoleNewLine\n";
						break;
					}

					i += 1;
				}
				else
				{
					CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 1].idxTI]);
					switch (lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype)
					{
					case IT::UINT:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintUnsignedIntNewLine\n";
						break;
					case IT::INT:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintIntNewLine\n";
						break;
					case IT::BOOL:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintBooleanNewLine\n";
						break;
					case IT::STRING:
						(*out.stream) << "\tpush " << scopedName << "\n";
						(*out.stream) << "\tcall PrintConsoleNewLine\n";
						break;
					}
				}
				break;
			case LEX_IF:
				isIf = true;
				codeBlocks.push("if");

				if ((lex.lextable.table[i + 2].lexema == LEX_LITERAL || lex.lextable.table[i + 2].lexema == LEX_ID) &&
					 lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype == IT::BOOL)
				{
					if(lex.lextable.table[i + 2].lexema == LEX_LITERAL)
					{
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
					}
					else
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						scopedName = nullptr;
					}

					(*out.stream) << "\tcmp eax, 1\n";
					(*out.stream) << "\tjne ELSE_" << counterIfElse << "\n";
				}
				else
				{
					if(lex.lextable.table[i + 2].lexema == LEX_LITERAL && lex.lextable.table[i + 3].lexema == LEX_LITERAL)
					{
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
						(*out.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
					}
					else if (lex.lextable.table[i + 2].lexema == LEX_LITERAL && lex.lextable.table[i + 3].lexema == LEX_ID)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
						(*out.stream) << "\tcmp eax, " << scopedName << "\n";
						scopedName = nullptr;
					}
					else if (lex.lextable.table[i + 2].lexema == LEX_ID && lex.lextable.table[i + 3].lexema == LEX_LITERAL)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						(*out.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
						scopedName = nullptr;
					}
					else
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						scopedName = nullptr;
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tcmp eax, " << scopedName << "\n";
						scopedName = nullptr;
					}

					switch (lex.lextable.table[i + 4].lexema)
					{
					case LEX_MORE:
						(*out.stream) << "\tjle ELSE_" << counterIfElse << "\n";
						break;
					case LEX_LESS:
						(*out.stream) << "\tjge ELSE_" << counterIfElse << "\n";
						break;
					case LEX_MORE_OR_EQUAL:
						(*out.stream) << "\tjl ELSE_" << counterIfElse << "\n";
						break;
					case LEX_LESS_OR_EQUAL:
						(*out.stream) << "\tjg ELSE_" << counterIfElse << "\n";
						break;
					case LEX_EQUAL:
						(*out.stream) << "\tjne ELSE_" << counterIfElse << "\n";
						break;
					case LEX_NOT_EQUAL:
						(*out.stream) << "\tje ELSE_" << counterIfElse << "\n";
						break;
					}
				}
				break;
			case LEX_ELSE:
				isElse = true;
				codeBlocks.push("else");
				(*out.stream) << "ELSE_" << counterIfElse << ":\n";
				break;
			case LEX_WHILE:
				codeBlocks.push("while");
				isCycle = true;
				(*out.stream) << "WHILE_" << counterWhile << ":\n";

				if ((lex.lextable.table[i + 2].lexema == LEX_LITERAL || lex.lextable.table[i + 2].lexema == LEX_ID) &&
					lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype == IT::BOOL)
				{
					if (lex.lextable.table[i + 2].lexema == LEX_LITERAL)
					{
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
					}
					else
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						scopedName = nullptr;
					}

					(*out.stream) << "\tcmp eax, 1\n";
					(*out.stream) << "\tjne END_WHILE_" << counterWhile << "\n";
				}
				else
				{
					if (lex.lextable.table[i + 2].lexema == LEX_LITERAL && lex.lextable.table[i + 3].lexema == LEX_LITERAL)
					{
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
						(*out.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
					}
					else if (lex.lextable.table[i + 2].lexema == LEX_LITERAL && lex.lextable.table[i + 3].lexema == LEX_ID)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tmov eax, " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n";
						(*out.stream) << "\tcmp eax, " << scopedName << "\n";
						scopedName = nullptr;
					}
					else if (lex.lextable.table[i + 2].lexema == LEX_ID && lex.lextable.table[i + 3].lexema == LEX_LITERAL)
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						(*out.stream) << "\tcmp eax, " << lex.idtable.table[lex.lextable.table[i + 3].idxTI].id << "\n";
						scopedName = nullptr;
					}
					else
					{
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 2].idxTI]);
						(*out.stream) << "\tmov eax, " << scopedName << "\n";
						scopedName = nullptr;
						CreateNameWithScope(scopedName, lex.idtable.table[lex.lextable.table[i + 3].idxTI]);
						(*out.stream) << "\tcmp eax, " << scopedName << "\n";
						scopedName = nullptr;
					}

					switch (lex.lextable.table[i + 4].lexema)
					{
					case LEX_MORE:
						(*out.stream) << "\tjle END_WHILE_" << counterWhile << "\n";
						break;
					case LEX_LESS:
						(*out.stream) << "\tjge END_WHILE_" << counterWhile << "\n";
						break;
					case LEX_MORE_OR_EQUAL:
						(*out.stream) << "\tjl END_WHILE_" << counterWhile << "\n";
						break;
					case LEX_LESS_OR_EQUAL:
						(*out.stream) << "\tjg END_WHILE_" << counterWhile << "\n";
						break;
					case LEX_EQUAL:
						(*out.stream) << "\tjne END_WHILE_" << counterWhile << "\n";
						break;
					case LEX_NOT_EQUAL:
						(*out.stream) << "\tje END_WHILE_" << counterWhile << "\n";
						break;
					}
				}
				break;
			default:
				continue;
			}
		}

		delete[] scopedName;
	}

	void CreateNameWithScope(char*& scopedName, const IT::Entry entry)
	{
		int idNameSize = strlen(entry.id);
		int scopeNameSize = strlen(entry.scope);
		scopedName = new char[idNameSize + scopeNameSize + 2];
		scopedName[0] = '\0';
		strcat(scopedName, entry.id);
		strcat(scopedName, "_");
		strcat(scopedName, entry.scope);
	}
}