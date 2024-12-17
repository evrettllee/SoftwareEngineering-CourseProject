#include "pch.h"
#include "LA.h"

bool trueFlag = false;
bool falseFlag = false;
bool intFlag = false;
bool uintFlag = false;
bool stringFlag = false;
bool boolFlag = false;
bool declareFunctionFlag = false;
bool declareVariableFlag = false;
bool declareIfFlag = false;
bool declareWhileFlag = false;
bool declareElseFlag = false;
bool declareArrayFlag = false;
bool indexFlag = false;
bool arrayAssignment = false;


namespace LA
{
	LEX LA(Parm::PARM parm, In::IN in, Log::LOG& log)
	{
		LEX lexresult;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);
		LT::Entry currentEntryLT = LT::Entry();
		IT::Entry currentEntryIT = IT::Entry();
		char* buffer = new char[LA_MAXSIZE];
		char lexema = NULL;
		int bufferIndex = 0;
		int numberOfLiterals = 0;
		int currentLine = 1;
		int column = 0;
		int countOfCodeBlocks = 0;
		int countOfParams = 0;
		std::vector<char*> scope;
		char global[TI_ID_MAXSIZE];
		strncpy(global, "global", strlen("global"));
		global[strlen("global")] = '\0';
		scope.push_back(global);
		std::vector<char> separators = { ';' ,' ', ',', '[', ']', '(', ')', '{', '}', '+', '-', '*', '/', '%', '>', '<', '=', '!', '|' };

		for (int i = 0; i < in.size; i++)
		{
			column++;
			if (std::find(separators.begin(), separators.end(), in.text[i]) == separators.end())
			{
				if (in.text[i] == '\"')
				{
					ProcessReadingStringLiteral(buffer, bufferIndex, column, i, currentLine, in);
					continue;
				}
				buffer[bufferIndex++] = in.text[i];

				if (bufferIndex > LA_MAXSIZE)
				{
					throw ERROR_THROW(126);
				}
			}
			else
			{
				buffer[bufferIndex] = '\0';
				lexema = FST(buffer);

				if (lexema == LEX_MAIN)
				{
					ProcessMainEntry(buffer, scope, idtable, currentEntryLT, currentEntryIT, currentLine, column);
				}

				if (lexema == LEX_LITERAL)
				{
					ProcessLiteralEntry(buffer, scope, lextable, idtable, currentEntryLT, currentEntryIT, in, currentLine, numberOfLiterals, i, column);
				}

				if (lexema == LEX_ID)
				{
					bool addedToITFlag = false;
					currentEntryLT.lexema = lexema;
					currentEntryIT.idtype = IT::V;
					strncpy(currentEntryIT.id, buffer, TI_ID_MAXSIZE);
					currentEntryIT.id[strlen(buffer)] = '\0';
					if (!scope.empty())
					{
						strncpy(currentEntryIT.scope, scope.back(), strlen(scope.back()));
						currentEntryIT.scope[strlen(scope.back())] = '\0';
					}
					else
					{
						currentEntryIT.scope[0] = NULL;
					}
					if (strcmp(buffer, DATE) == 0 || strcmp(buffer, TIME) == 0)
					{
						ProcessDateTimeLibraryFunctions(currentEntryLT, currentEntryIT, idtable, currentLine);
						addedToITFlag = true;
					}
					if (lextable.table[lextable.size - 1].lexema == LEX_FUNCTION && declareFunctionFlag)
					{
						ProcessFunctionDeclaration(lextable, idtable, currentEntryLT, currentEntryIT, currentLine, column);
						addedToITFlag = true;
					}
					if (lextable.table[lextable.size - 2].lexema == LA_LEFTTHESIS	 &&
						lextable.table[lextable.size - 3].lexema == LEX_ID			 &&
						lextable.table[lextable.size - 3].idxTI != TI_NULLIDX		 &&
						lextable.table[lextable.size - 3].idxTI == idtable.size - 1  &&
						idtable.table[idtable.size - 1].idtype == IT::F)
					{
						ProcessFunctionParametr(lextable, idtable, currentEntryLT, currentEntryIT, currentLine, column, countOfParams);
						addedToITFlag = true;
					}
					if (lextable.table[lextable.size - 2].lexema == LA_COMMA && idtable.table[idtable.size - 1].idtype == IT::P)
					{
						ProcessFunctionParametrs(lextable, idtable, currentEntryLT, currentEntryIT, currentLine, column, countOfParams);
						addedToITFlag = true;
					}
					if (lextable.table[lextable.size - 2].lexema == LEX_NEW && declareVariableFlag)
					{
						ProcessVariableDeclaration(lextable, idtable, currentEntryLT, currentEntryIT, currentLine, column);
						addedToITFlag = true;
					}
					if (lextable.table[lextable.size - 5].lexema == LEX_NEW &&
						lextable.table[lextable.size - 3].lexema == LEX_ARRAY &&
						lextable.table[lextable.size - 2].lexema == LEX_IND &&
						declareArrayFlag)
					{
						ProcessArrayDeclaration(lextable, idtable, currentEntryLT, currentEntryIT, currentLine, column);
						addedToITFlag = true;
					}

					if (!addedToITFlag)
					{
						SearchIdInExternalScope(idtable, scope, currentEntryLT, currentEntryIT, currentLine, column);
					}
				}

				if (lexema == '\0' && buffer[0] != '\0')
				{
					throw ERROR_THROW_IN(127, currentLine, column);
				}

				bufferIndex = 0;
			}

			if (lexema != NULL)
			{
				currentEntryLT.lexema = lexema;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				lexema = NULL;
				currentEntryLT = LT::Entry();
			}

			switch (in.text[i])
			{
			case LA_NEW_LINE:
				currentLine++;
				column = 0;
				currentEntryLT = LT::Entry();
				break;
			case LA_SEMICOLON:
				currentEntryLT.lexema = LEX_SEMICOLON;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_COMMA:
				currentEntryLT.lexema = LEX_COMMA;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_LEFTBRACE:
				currentEntryLT.lexema = LEX_LEFTBRACE;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				if (lextable.table[lextable.size - 3].lexema == LEX_ID && idtable.size != 0 && idtable.table[idtable.size - 1].idtype == IT::A)
				{
					arrayAssignment = true;
					continue;
				}
				else if (!declareIfFlag && !declareElseFlag && !declareWhileFlag)
				{
					for (int i = idtable.size - 1; i >= 0; i--)
					{
						if (idtable.table[i].idtype == IT::F)
						{
							scope.push_back(idtable.table[i].id);
							break;
						}
					}
				}
				else
				{
					char blockScope[TI_ID_MAXSIZE];
					if (declareIfFlag)
					{
						sprintf(blockScope, "if%d", countOfCodeBlocks);
						declareIfFlag = false;
					}
					if (declareElseFlag)
					{
						sprintf(blockScope, "else%d", countOfCodeBlocks);
						declareElseFlag = false;
					}
					if (declareWhileFlag)
					{
						sprintf(blockScope, "while%d", countOfCodeBlocks);
						declareWhileFlag = false;
					}
					scope.push_back(blockScope);
					countOfCodeBlocks++;
				}
				currentEntryLT = LT::Entry();
				break;
			case LA_RIGHTBRACE:
				currentEntryLT.lexema = LEX_RIGHTBRACE;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				if (!scope.empty() && !arrayAssignment)
				{
					scope.pop_back();
				}
				arrayAssignment = false;
				currentEntryLT = LT::Entry();
				break;
			case LA_LEFTTHESIS:
				currentEntryLT.lexema = LEX_LEFTTHESIS;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				if (declareFunctionFlag)
				{
					for (int i = idtable.size - 1; i >= 0; i--)
					{
						if (idtable.table[i].idtype == IT::F)
						{
							scope.push_back(idtable.table[i].id);
							break;
						}
					}
				}
				if (declareIfFlag || declareWhileFlag)
				{
					char blockScope[TI_ID_MAXSIZE];
					if (declareIfFlag)
					{
						sprintf(blockScope, "if%d", countOfCodeBlocks);
					}
					if (declareWhileFlag)
					{
						sprintf(blockScope, "while%d", countOfCodeBlocks);
					}
					scope.push_back(blockScope);
				}
				currentEntryLT = LT::Entry();
				break;
			case LA_RIGHTTHESIS:
				currentEntryLT.lexema = LEX_RIGHTTHESIS;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				if (!scope.empty() && declareFunctionFlag)
				{
					scope.pop_back();
					for (int i = idtable.size - 1; idtable.table[i + 1].idtype != IT::F; i--)
					{
						if (idtable.table[i].idtype == IT::F)
						{
							if (strcmp(idtable.table[i].id, DATE) != 0 && strcmp(idtable.table[i].id, TIME) != 0)
							{
								idtable.table[i].value.vint = countOfParams;
							}
							else
							{
								idtable.table[i].value.vint = 0;
								break;
							}
						}
					}
					countOfParams = 0;
					declareFunctionFlag = false;
				}
				if (!scope.empty() && (declareIfFlag || declareElseFlag || declareWhileFlag))
				{
					scope.pop_back();
				}
				currentEntryLT = LT::Entry();
				break;
			case LA_PLUS:
				currentEntryLT.lexema = LEX_PLUS;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_MINUS:
				currentEntryLT.lexema = LEX_MINUS;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_STAR:
				currentEntryLT.lexema = LEX_STAR;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_DIRSLASH:
				currentEntryLT.lexema = LEX_DIRSLASH;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_PERCENT:
				currentEntryLT.lexema = LEX_PERCENT;
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT.lexema = NULL;
				break;
			case LA_MORE:
				currentEntryLT.lexema = LEX_MORE;
				if (in.text[i + 1] == LA_EQUAL)
				{
					currentEntryLT.lexema = LEX_MORE_OR_EQUAL;
					i++;
				}
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_LESS:
				currentEntryLT.lexema = LEX_LESS;
				if (in.text[i + 1] == LA_EQUAL)
				{
					currentEntryLT.lexema = LEX_LESS_OR_EQUAL;
					i++;
				}
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_EQUAL:
				currentEntryLT.lexema = LEX_EQUAL_SIGN;
				if (in.text[i + 1] == LA_EQUAL)
				{
					currentEntryLT.lexema = LEX_EQUAL;
					i++;
				}
				currentEntryLT.sn = currentLine;
				LT::Add(lextable, currentEntryLT);
				currentEntryLT = LT::Entry();
				break;
			case LA_NOT:
				if (in.text[i + 1] == LA_EQUAL)
				{
					currentEntryLT.lexema = LEX_NOT_EQUAL;
					i++;
					currentEntryLT.sn = currentLine;
					LT::Add(lextable, currentEntryLT);
				}
				currentEntryLT = LT::Entry();
				break;
			case LA_LEFTIND:
				if (lextable.table[lextable.size - 1].lexema == LEX_ARRAY && !declareArrayFlag)
				{
					throw ERROR_THROW_IN(208, currentLine, column);
				}
				else
				{
					currentEntryLT.lexema = LEX_IND;
					currentEntryLT.sn = currentLine;
					LT::Add(lextable, currentEntryLT);
					indexFlag = true;
				}
				currentEntryLT = LT::Entry();
				break;
			case LA_RIGHTIND:
				if (indexFlag)
				{
					indexFlag = false;
				}
				else
				{
					throw ERROR_THROW_IN(209, currentLine, column);
				}
				currentEntryLT = LT::Entry();
				break;
			case IN_CODE_SPACE:
				column++;
				currentEntryLT = LT::Entry();
				break;
			}
		}

		lexresult.lextable = lextable;
		lexresult.idtable = idtable;

		(*log.stream) << "\n------------------------------------------------------------------------------------------------\n";
		(*log.stream) << "0 всего строк: " << lextable.size << ", лексический анализ выполнен без ошибок";
		(*log.stream) << "\n------------------------------------------------------------------------------------------------\n";
		LT::WriteTable(lextable);
		IT::WriteTable(idtable);

		return lexresult;
	}

	char FST(char* str)
	{
		FST_NEW;
		FST_INT;
		FST_UINT;
		FST_BOOL;
		FST_STRING;
		FST_FUNCTION;
		FST_RETURN;
		FST_ARRAY;
		FST_MAIN;
		FST_WRITE;
		FST_WRITELINE;
		FST_TRUE;
		FST_FALSE;
		FST_WHILE;
		FST_IF;
		FST_ELSE;
		FST_INT_BIN_LITERAL;
		FST_INT_OCT_LITERAL;
		FST_INT_DEC_LITERAL;
		FST_INT_HEX_LITERAL;
		FST_DECIMAL_LITERAL;
		FST_STRING_LITERAL;
		FST_IDENTIFIER;

		if (FST::Execute(_array))
		{
			declareArrayFlag = true;
			return LEX_ARRAY;
		}
		if (FST::Execute(_new))
		{
			declareVariableFlag = true;
			return LEX_NEW;
		}
		if (FST::Execute(_int))
		{
			intFlag = true;
			return LEX_INTEGER;
		}
		if (FST::Execute(_uint))
		{
			uintFlag = true;
			return LEX_UNSIGNED_INTEGER;
		}
		if (FST::Execute(_bool))
		{
			boolFlag = true;
			return LEX_BOOL;
		}
		if (FST::Execute(_string))
		{
			stringFlag = true;
			return LEX_STRING;
		}
		if (FST::Execute(_function))
		{
			declareFunctionFlag = true;
			return LEX_FUNCTION;
		}
		if (FST::Execute(_return))
		{
			return LEX_RETURN;
		}
		if (FST::Execute(_main))
		{
			return LEX_MAIN;
		}
		if (FST::Execute(_write))
		{
			return LEX_WRITE;
		}
		if (FST::Execute(_writeline))
		{
			return LEX_WRITELINE;
		}
		if (FST::Execute(_while))
		{
			declareWhileFlag = true;
			return LEX_WHILE;
		}
		if (FST::Execute(_if))
		{
			declareIfFlag = true;
			return LEX_IF;
		}
		if (FST::Execute(_else))
		{
			declareElseFlag = true;
			return LEX_ELSE;
		}
		if (FST::Execute(_int_bin_literal) || FST::Execute(_int_oct_literal) ||
			FST::Execute(_int_dec_literal) || FST::Execute(_int_hex_literal) ||
			FST::Execute(_string_literal))
		{
			return LEX_LITERAL;
		}
		if (FST::Execute(_true))
		{
			trueFlag = true;
			return LEX_LITERAL;
		}
		if (FST::Execute(_false))
		{
			falseFlag = true;
			return LEX_LITERAL;
		}
		if (FST::Execute(_identifier))
		{
			return LEX_ID;
		}

		return NULL;
	}

	void ProcessMainEntry(const char* buffer, const std::vector<char*>&scope,
						  IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, const int currentLine, const int column)
	{
		strncpy(currentEntryIT.id, buffer, TI_ID_MAXSIZE);
		currentEntryIT.id[strlen(buffer)] = '\0';
		currentEntryIT.idtype = IT::F;
		currentEntryIT.iddatatype = IT::VOID;
		currentEntryIT.idxfirstLE = currentLine;
		if (!scope.empty())
		{
			strncpy(currentEntryIT.scope, scope.back(), strlen(scope.back()));
			currentEntryIT.scope[strlen(scope.back())] = '\0';
		}
		else
		{
			currentEntryIT.scope[0] = NULL;
		}

		int indexIT = IT::Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(300, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
		}
	}

	void ProcessStringLiteral(IT::Entry & currentEntryIT, const char* buffer)
	{
		currentEntryIT.iddatatype = IT::STRING;
		int bufferLength = strlen(buffer) - 2;
		int length = bufferLength > TI_STR_MAXSIZE ? TI_STR_MAXSIZE : bufferLength;
		strncpy(currentEntryIT.value.vstr.str, buffer + 1, length);
		currentEntryIT.value.vstr.str[length] = '\0';
		currentEntryIT.value.vstr.len = length;
	}

	void ProcessIntegerLiteral(LT::LexTable & lextable, IT::Entry & currentEntryIT, const char* buffer, const In::IN & in, const int i, const int currentLine, 
							   const int column)
	{
		long long intLiteral;

		try
		{
			if (strcmp(buffer, "0") == 0)
			{
				intLiteral = 0;
			}
			else
			{
				switch (buffer[strlen(buffer) - 1])
				{
				case 'b':
					intLiteral = std::stoull(buffer, nullptr, 2);
					break;
				case 'o':
					intLiteral = std::stoull(buffer, nullptr, 8);
					break;
				case 'h':
					intLiteral = std::stoull(buffer, nullptr, 16);
					break;
				default:
					intLiteral = std::stoull(buffer, nullptr, 10);
					break;
				}

				if (in.text[i - strlen(buffer) - 1] == LA_MINUS					&&
					lextable.size > 2											&&
					lextable.table[lextable.size - 2].lexema != LEX_LITERAL		&&
					lextable.table[lextable.size - 2].lexema != LEX_ID			&&
					lextable.table[lextable.size - 2].lexema != LEX_RIGHTTHESIS)
				{
					intLiteral = -intLiteral;
					lextable.size--;
				}
			}

			if (intLiteral >= TI_INT_MINSIZE && intLiteral <= TI_INT_MAXSIZE)
			{
				currentEntryIT.iddatatype = IT::INT;
				currentEntryIT.value.vint = intLiteral;
			}
			else if (intLiteral >= TI_UINT_MINSIZE && intLiteral <= TI_UINT_MAXSIZE)
			{
				currentEntryIT.iddatatype = IT::UINT;
				currentEntryIT.value.vuint = intLiteral;
			}
			else
			{
				currentEntryIT.iddatatype = IT::INT;
				currentEntryIT.value.vint = TI_UINT_DEFAULT;
			}
		}
		catch (const std::invalid_argument&)
		{
			throw ERROR_THROW_IN(335, currentLine, column);
		}
		catch (const std::out_of_range&)
		{
			throw ERROR_THROW_IN(336, currentLine, column);
		}
	}

	void ProcessBooleanLiteral(IT::Entry & currentEntryIT)
	{
		currentEntryIT.iddatatype = IT::BOOL;

		if (trueFlag)
		{
			currentEntryIT.value.vbool = true;
			trueFlag = false;
		}

		if (falseFlag)
		{
			currentEntryIT.value.vbool = false;
			falseFlag = false;
		}
	}

	void ProcessLiteralEntry(const char* buffer, const std::vector<char*>&scope, LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT,
							 IT::Entry & currentEntryIT, const In::IN & in, const int currentLine, int& numberOfLiterals, const int i, const int column)
	{
		currentEntryIT.idtype = IT::L;

		if (in.text[i - strlen(buffer)] == '\"' && in.text[i - 1] == '\"')
		{
			ProcessStringLiteral(currentEntryIT, buffer);
		}
		if ((isdigit((in.text[i - strlen(buffer)])) || (in.text[i - strlen(buffer)] >= 'A' && in.text[i - strlen(buffer)] <= 'F')))
		{
			ProcessIntegerLiteral(lextable, currentEntryIT, buffer, in, i, currentLine, column);
		}
		if (trueFlag || falseFlag)
		{
			ProcessBooleanLiteral(currentEntryIT);
		}

		int indexIT = IT::Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			currentEntryLT.idxTI = indexIT;
		}
		else
		{
			sprintf(currentEntryIT.id, "L%d", numberOfLiterals);
			if (!scope.empty())
			{
				strncpy(currentEntryIT.scope, scope.back(), strlen(scope.back()));
				currentEntryIT.scope[strlen(scope.back())] = '\0';
			}
			else
			{
				currentEntryIT.scope[0] = NULL;
			}
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
			numberOfLiterals++;
		}
	}

	void ProcessDateTimeLibraryFunctions(LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, IT::IdTable & idtable, const int currentLine)
	{
		currentEntryIT.idtype = IT::F;
		currentEntryIT.iddatatype = IT::STRING;
		currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
		currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
		currentEntryLT.idxTI = idtable.size;
		currentEntryIT.idxfirstLE = currentLine;
		IT::Add(idtable, currentEntryIT);
		currentEntryIT = IT::Entry();
	}

	void ProcessFunctionDeclaration(LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, int currentLine,
									int column)
	{
		currentEntryIT.idtype = IT::F;

		if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_INTEGER && intFlag)
		{
			currentEntryIT.iddatatype = IT::INT;
			currentEntryIT.value.vint = TI_UINT_DEFAULT;
			intFlag = false;
		}
		if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_UNSIGNED_INTEGER && uintFlag)
		{
			currentEntryIT.iddatatype = IT::UINT;
			currentEntryIT.value.vuint = TI_UINT_DEFAULT;
			uintFlag = false;
		}
		if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_STRING && stringFlag)
		{
			currentEntryIT.iddatatype = IT::STRING;
			currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
			currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
			stringFlag = false;
		}
		if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexema == LEX_BOOL && boolFlag)
		{
			currentEntryIT.iddatatype = IT::BOOL;
			currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
			boolFlag = false;
		}

		int indexIT = Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(301, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
		}
	}

	void ProcessFunctionParametr(LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, int currentLine,
								 int column, int& countOfParams)
	{
		currentEntryIT.idtype = IT::P;
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_INTEGER && intFlag)
		{
			currentEntryIT.iddatatype = IT::INT;
			currentEntryIT.value.vint = TI_UINT_DEFAULT;
			intFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_UNSIGNED_INTEGER && uintFlag)
		{
			currentEntryIT.iddatatype = IT::UINT;
			currentEntryIT.value.vuint = TI_UINT_DEFAULT;
			uintFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_STRING && stringFlag)
		{
			currentEntryIT.iddatatype = IT::STRING;
			currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
			currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
			stringFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_BOOL && boolFlag)
		{
			currentEntryIT.iddatatype = IT::BOOL;
			currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
			boolFlag = false;
		}

		int indexIT = Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(302, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
			countOfParams++;
		}
	}

	void ProcessFunctionParametrs(LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, int currentLine,
								  int column, int& countOfParams)
	{
		currentEntryIT.idtype = IT::P;
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_INTEGER && intFlag)
		{
			currentEntryIT.iddatatype = IT::INT;
			currentEntryIT.value.vint = TI_UINT_DEFAULT;
			intFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_UNSIGNED_INTEGER && uintFlag)
		{
			currentEntryIT.iddatatype = IT::UINT;
			currentEntryIT.value.vuint = TI_UINT_DEFAULT;
			uintFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_STRING && stringFlag)
		{
			currentEntryIT.iddatatype = IT::STRING;
			currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
			currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
			stringFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_BOOL && boolFlag)
		{
			currentEntryIT.iddatatype = IT::BOOL;
			currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
			boolFlag = false;
		}

		int indexIT = Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(302, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
			countOfParams++;
		}
	}

	void ProcessVariableDeclaration(LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, int currentLine,
									int column)
	{
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_INTEGER && intFlag)
		{
			currentEntryIT.iddatatype = IT::INT;
			currentEntryIT.value.vint = TI_UINT_DEFAULT;
			intFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_UNSIGNED_INTEGER && uintFlag)
		{
			currentEntryIT.iddatatype = IT::UINT;
			currentEntryIT.value.vuint = TI_UINT_DEFAULT;
			uintFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_STRING && stringFlag)
		{
			currentEntryIT.iddatatype = IT::STRING;
			currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
			currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
			stringFlag = false;
		}
		if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexema == LEX_BOOL && boolFlag)
		{
			currentEntryIT.iddatatype = IT::BOOL;
			currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
			boolFlag = false;
		}

		int indexIT = Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(303, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
		}

		declareVariableFlag = false;
	}

	void ProcessArrayDeclaration(LT::LexTable & lextable, IT::IdTable & idtable, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT, int currentLine,
								 int column)
	{
		if (lextable.size >= 4 && lextable.table[lextable.size - 4].lexema == LEX_INTEGER && intFlag)
		{
			currentEntryIT.idtype = IT::A;
			currentEntryIT.iddatatype = IT::INT;
			currentEntryIT.value.vint = TI_UINT_DEFAULT;
			intFlag = false;
		}
		if (lextable.size >= 4 && lextable.table[lextable.size - 4].lexema == LEX_UNSIGNED_INTEGER && uintFlag)
		{
			currentEntryIT.idtype = IT::A;
			currentEntryIT.iddatatype = IT::UINT;
			currentEntryIT.value.vuint = TI_UINT_DEFAULT;
			uintFlag = false;
		}
		if (lextable.size >= 4 && lextable.table[lextable.size - 4].lexema == LEX_STRING && stringFlag)
		{
			currentEntryIT.idtype = IT::A;
			currentEntryIT.iddatatype = IT::STRING;
			currentEntryIT.value.vstr.str[0] = TI_STR_DEFAULT;
			currentEntryIT.value.vstr.len = strlen(currentEntryIT.value.vstr.str);
			stringFlag = false;
		}
		if (lextable.size >= 4 && lextable.table[lextable.size - 4].lexema == LEX_BOOL && boolFlag)
		{
			currentEntryIT.idtype = IT::A;
			currentEntryIT.iddatatype = IT::BOOL;
			currentEntryIT.value.vbool = TI_BOOL_DEFAULT;
			boolFlag = false;
		}

		int indexIT = Search(idtable, currentEntryIT);
		if (indexIT >= 0)
		{
			throw ERROR_THROW_IN(304, currentLine, column);
		}
		else
		{
			currentEntryLT.idxTI = idtable.size;
			currentEntryIT.idxfirstLE = currentLine;
			if (lextable.table[lextable.size - 1].lexema == LEX_LITERAL)
			{
				if (lextable.size > 1															  &&
					lextable.table[lextable.size - 1].idxTI != TI_NULLIDX                         &&
					(idtable.table[lextable.table[lextable.size - 1].idxTI].iddatatype == IT::INT &&
					 idtable.table[lextable.table[lextable.size - 1].idxTI].value.vint > 0)       ||
					 idtable.table[lextable.table[lextable.size - 1].idxTI].iddatatype == IT::UINT)
				{
					currentEntryIT.value.vuint = idtable.table[idtable.size - 1].iddatatype == IT::INT ?
						                         idtable.table[idtable.size - 1].value.vint : idtable.table[idtable.size - 1].value.vuint;
				}
				else
				{
					throw ERROR_THROW_IN(305, currentLine, column);
				}
			}
			else
			{
				throw ERROR_THROW_IN(306, currentLine, column);
			}
			IT::Add(idtable, currentEntryIT);
			currentEntryIT = IT::Entry();
		}

		declareArrayFlag = false;
	}

	void ProcessReadingStringLiteral(char* buffer, int& bufferIndex, int& column, int& i, const int line, const In::IN & in)
	{
		buffer[bufferIndex++] = in.text[i++];
		column++;
		while (in.text[i] != '\"')
		{
			buffer[bufferIndex++] = in.text[i++];
			column++;

			if (in.text[i] == LA_SEMICOLON || in.text[i] == LA_NEW_LINE)
			{
				throw ERROR_THROW_IN(128, line, column);
			}
		}
		buffer[bufferIndex++] = in.text[i];
	}

	void SearchIdInExternalScope(IT::IdTable & idtable, const std::vector<char*>&scope, LT::Entry & currentEntryLT, IT::Entry & currentEntryIT,
								 const int currentLine, const int column)
	{
		int indexIT = Search(idtable, currentEntryIT);

		if (indexIT >= 0)
		{
			currentEntryLT.idxTI = indexIT;
			currentEntryIT = IT::Entry();
		}
		else
		{
			for (int i = scope.size() - 2; i >= 0; --i)
			{
				strncpy(currentEntryIT.scope, scope[i], strlen(scope[i]));
				currentEntryIT.scope[strlen(scope[i])] = '\0';
				indexIT = Search(idtable, currentEntryIT);

				if (indexIT >= 0)
				{
					break;
				}
			}

			currentEntryLT.idxTI = indexIT;
			currentEntryIT = IT::Entry();
		}

		if (indexIT < 0)
		{
			throw ERROR_THROW_IN(307, currentLine, column);
		}
	}
}
