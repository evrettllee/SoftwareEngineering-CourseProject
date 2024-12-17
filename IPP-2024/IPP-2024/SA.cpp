#include "pch.h"

namespace SA
{
	void SA(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG& log)
	{
		bool hasMain = false;

		for (int i = 0; i < lextable.size; i++)
		{
			switch (lextable.table[i].lexema)
			{
			case LEX_IND:
				HandleArrayIndexing(i, lextable, idtable);
				break;
			case LEX_EQUAL_SIGN:
				HandleAssignment(i, lextable, idtable);
				break;
			case LEX_IF:
			case LEX_WHILE:
				CheckConditionExpression(++i, lextable, idtable);
				break;
			case LEX_FUNCTION:
				CheckFunctionReturnType(++i, lextable, idtable);
				break;
			case LEX_MAIN:
				hasMain = true;
				break;
			default:
				break;
			}
		}

		if (!hasMain)
		{
			throw ERROR_THROW(308);
		}

		(*log.stream) << "\n------------------------------------------------------------------------------------------------\n";
		(*log.stream) << "0 ‚ÒÂ„Ó ÒÚÓÍ: " << lextable.size << ", ÒÂÏ‡ÌÚË˜ÂÒÍËÈ ‡Ì‡ÎËÁ ‚˚ÔÓÎÌÂÌ ·ÂÁ Ó¯Ë·ÓÍ";
		(*log.stream) << "\n------------------------------------------------------------------------------------------------\n";
	}
        
	void HandleArrayIndexing(int& pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		if (lextable.table[pos - 1].lexema == LEX_ID && idtable.table[lextable.table[pos - 1].idxTI].idtype == IT::A)
		{
			CheckArraySize(++pos, lextable, idtable);
		}
	}

	void HandleAssignment(int& pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		bool isVariable = lextable.table[pos - 1].idxTI != TI_NULLIDX &&
						  lextable.table[pos - 1].lexema == LEX_ID    &&
						  idtable.table[lextable.table[pos - 1].idxTI].idtype == IT::V;

		bool isArray = lextable.table[pos - 1].idxTI != TI_NULLIDX                   &&
					   lextable.table[pos - 1].lexema == LEX_ID					     &&
					   idtable.table[lextable.table[pos - 1].idxTI].idtype == IT::A  &&
					   lextable.table[pos + 1].lexema == LEX_LEFTBRACE;

		if (isVariable)
		{
			CheckVariableAssignment(++pos, lextable, idtable);
		}
		else if (isArray)
		{
			CheckArrayAssignment(++pos, lextable, idtable);
		}
	}

	void CheckConditionExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		if (IsBooleanExpression(pos, lextable, idtable))
		{
			CheckCodeBlock(pos + 3, lextable);
		}
		else if (IsComparisonExpression(pos, lextable, idtable))
		{
			ValidateComparison(pos, lextable, idtable);
			CheckCodeBlock(pos + 4, lextable);
		}
		else
		{
			throw ERROR_THROW_IN(320, lextable.table[pos].sn, pos);
		}
	}

	void ValidateComparison(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		if (lextable.table[pos - 1].lexema == LEX_WHILE	  &&
			lextable.table[pos + 1].lexema == LEX_LITERAL &&
			lextable.table[pos + 3].lexema == LEX_LITERAL)
		{
			throw ERROR_THROW_IN(331, lextable.table[pos].sn, pos);
		}
	}

	bool IsBooleanExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		return (lextable.table[pos + 1].lexema == LEX_ID || lextable.table[pos + 1].lexema == LEX_LITERAL) &&
			    lextable.table[pos + 1].idxTI != TI_NULLIDX												   &&
			    idtable.table[lextable.table[pos + 1].idxTI].iddatatype == IT::BOOL						   &&
			    lextable.table[pos + 2].lexema == LEX_RIGHTTHESIS;
	}

	bool IsComparisonExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		return lextable.table[pos + 1].idxTI != TI_NULLIDX	&&
			   lextable.table[pos + 3].idxTI != TI_NULLIDX	&&
			   CheckDataTypeCompatibility(
			   idtable.table[lextable.table[pos + 1].idxTI].iddatatype,
			   idtable.table[lextable.table[pos + 3].idxTI].iddatatype);
	}

	void CheckVariableAssignment(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::Entry var;

		if (lextable.table[pos - 2].lexema == LEX_ID && lextable.table[pos -3].lexema != LEX_IND)
		{
			var = idtable.table[lextable.table[pos - 2].idxTI];
		}
		else if (lextable.table[pos - 4].idxTI != TI_NULLIDX && (lextable.table[pos - 2].lexema == LEX_LITERAL || lextable.table[pos - 2].lexema == LEX_ID) &&
				 lextable.table[pos - 3].lexema == LEX_IND  && lextable.table[pos - 4].lexema == LEX_ID && idtable.table[lextable.table[pos - 4].idxTI].idtype == IT::A)
		{
			var = idtable.table[lextable.table[pos - 4].idxTI];
			CheckArraySize(pos - 2, lextable, idtable);
		}
		else
		{
			throw ERROR_THROW_IN(309, lextable.table[pos].sn, pos);
		}

		if (lextable.table[pos + 1].lexema == LEX_SEMICOLON)
		{
			if (!IsMatchingType(var.iddatatype, idtable.table[lextable.table[pos].idxTI].iddatatype))
			{
				throw ERROR_THROW_IN(329, lextable.table[pos].sn, pos);
			}
		}

		std::vector<IT::Entry> operands;
		bool hasArithmeticalOperations = false;
		bool hasLogicalOperations = false;

		for (int i = pos; lextable.table[i].lexema != LEX_SEMICOLON; i++)
		{
			if ( lextable.table[i].lexema == LEX_ID								&&
				 lextable.table[i].idxTI != TI_NULLIDX							&&
				(strcmp(idtable.table[lextable.table[i].idxTI].id, DATE) == 0   ||
				 strcmp(idtable.table[lextable.table[i].idxTI].id, TIME) == 0))
			{
				if (var.iddatatype != IT::STRING)
				{
					throw ERROR_THROW_IN(310, lextable.table[i].sn, i);
				}
				CheckFunctionArguments(i, lextable, idtable);
				operands.push_back(idtable.table[lextable.table[i].idxTI]);
				while (lextable.table[i + 1].lexema != LEX_RIGHTTHESIS)
				{
					i++;
				}
			}
			if (lextable.table[i].idxTI != TI_NULLIDX && lextable.table[i].lexema == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F)
			{
				CheckFunctionArguments(i, lextable, idtable);
				operands.push_back(idtable.table[lextable.table[i].idxTI]);
				while (lextable.table[i].lexema != LEX_RIGHTTHESIS)
				{
					i++;
				}
			}
			if (IsArithmeticalOperation(lextable.table[i].lexema))
			{
				hasArithmeticalOperations = true;
			}
			if (IsLogicalOperation(lextable.table[i].lexema))
			{
				if (lextable.table[pos - 2].idxTI != TI_NULLIDX && idtable.table[lextable.table[pos - 2].idxTI].iddatatype == IT::BOOL && !hasLogicalOperations)
				{
					if (!operands.empty() && lextable.table[i + 1].idxTI != TI_NULLIDX	&&
						!CheckDataTypeCompatibility(operands.back().iddatatype, idtable.table[lextable.table[i + 1].idxTI].iddatatype))
					{
						throw ERROR_THROW_IN(330, lextable.table[i].sn, i);
					}

					hasLogicalOperations = true;
				}
				else
				{
					throw ERROR_THROW_IN(311, lextable.table[i].sn, i);
				}
			}
			if ((lextable.table[i].lexema == LEX_LITERAL || lextable.table[i].lexema == LEX_ID) && lextable.table[i].idxTI != TI_NULLIDX)
			{
				operands.push_back(idtable.table[lextable.table[i].idxTI]);
			}
		}

		for (int i = 0; i < operands.size(); i++)
		{
			if (hasArithmeticalOperations)
			{
				if (!IsMatchingType(var.iddatatype, operands[i].iddatatype))
				{
					throw ERROR_THROW_IN(313, lextable.table[i].sn, i);
					break;
				}
			}
			else if (hasLogicalOperations)
			{
				if (operands.size() == 2)
				{
					if (!CheckDataTypeCompatibility(operands[0].iddatatype, operands[1].iddatatype))
					{
						throw ERROR_THROW_IN(311, lextable.table[i].sn, i);
						break;
					}
				}
			}
			else if (!hasArithmeticalOperations && !hasLogicalOperations)
			{
				if (!IsMatchingType(var.iddatatype, operands[i].iddatatype))
				{
					throw ERROR_THROW_IN(309, lextable.table[i].sn, i);
					break;
				}
			}
			else
			{
				throw ERROR_THROW_IN(309, lextable.table[i].sn, i);
				break;
			}
		}

		return;
	}
	
	void CheckFunctionArguments(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::Entry function = idtable.table[lextable.table[pos].idxTI];
		std::vector<IT::Entry> params;

		if (strcmp(function.id, "DATE") == 0 || strcmp(function.id, "TIME") == 0)
		{
			if (lextable.table[pos + 2].lexema != LEX_RIGHTTHESIS)
			{
				throw ERROR_THROW_IN(314, lextable.table[pos].sn, pos);
			}
		}
		else
		{
			for (int i = 0; i < lextable.size; i++)
			{
				if (lextable.table[i].idxTI != TI_NULLIDX                                             &&
					lextable.table[i].sn == function.idxfirstLE && lextable.table[i].lexema == LEX_ID &&
					strcmp(idtable.table[lextable.table[i].idxTI].id, function.id) == 0)
				{
					for (int j = i + 2; lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (lextable.table[j].idxTI != TI_NULLIDX && lextable.table[j].lexema == LEX_ID &&
							idtable.table[lextable.table[j].idxTI].idtype == IT::P)
						{
							params.push_back(idtable.table[lextable.table[j].idxTI]);
						}
					}

					break;
				}
			}

			if (params.empty())
			{
				if (lextable.table[pos + 2].lexema != LEX_RIGHTTHESIS)
				{
					throw ERROR_THROW_IN(315, lextable.table[pos].sn, pos);
				}
				return;
			}

			int paramIndex = 0;
			for (int i = pos + 2; lextable.table[i].lexema != LEX_RIGHTTHESIS; i++)
			{
				if (lextable.table[i].lexema == LEX_LITERAL || lextable.table[i].lexema == LEX_ID)
				{
					if (paramIndex >= params.size()) 
					{
						throw ERROR_THROW_IN(316, lextable.table[pos].sn, pos);
					}

					if (lextable.table[i].idxTI != TI_NULLIDX &&
						!IsMatchingType(idtable.table[lextable.table[i].idxTI].iddatatype, params[paramIndex].iddatatype) ||
						(params[paramIndex].iddatatype == IT::UINT && idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT &&
						 idtable.table[lextable.table[i].idxTI].value.vint < 0)
						)
					{
						throw ERROR_THROW_IN(317, lextable.table[pos].sn, pos);
					}

					paramIndex++; 				}
			}

			if (paramIndex < params.size())
			{
				throw ERROR_THROW_IN(318, lextable.table[pos].sn, pos);
			}
		}

		return;
	}

	void CheckIfWhileExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		if ((lextable.table[pos + 1].lexema == LEX_ID || lextable.table[pos + 1].lexema == LEX_LITERAL) &&
			lextable.table[pos + 1].idxTI != TI_NULLIDX													&&
			idtable.table[lextable.table[pos + 1].idxTI].iddatatype == IT::BOOL && lextable.table[pos + 2].lexema == LEX_RIGHTTHESIS)
		{
			CheckCodeBlock(pos + 3, lextable);
			return;
		}
		else if (—heckIfWhile—omparison(pos, lextable))
		{
			if (lextable.table[pos - 1].lexema == LEX_WHILE && lextable.table[pos + 1].lexema == LEX_LITERAL && lextable.table[pos + 3].lexema == LEX_LITERAL)
			{
				throw ERROR_THROW(331);
			}
			else if (lextable.table[pos + 1].idxTI != TI_NULLIDX && lextable.table[pos + 3].idxTI != TI_NULLIDX	&&
				     CheckDataTypeCompatibility(idtable.table[lextable.table[pos + 1].idxTI].iddatatype, idtable.table[lextable.table[pos + 3].idxTI].iddatatype))
			{
				CheckCodeBlock(pos + 4, lextable);
				return;
			}
			else
			{
				throw ERROR_THROW(319);
			}
		}
		else
		{
			throw ERROR_THROW(320);
		}

		return;
	}

	void CheckFunctionReturnType(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::Entry function = idtable.table[lextable.table[pos].idxTI];
		IT::IDDATATYPE functionReturnType = function.iddatatype;  
		bool isReturnFound = false;
		int indexOfReturn = pos;

		while (indexOfReturn < lextable.size && lextable.table[indexOfReturn].lexema != LEX_RETURN)
		{
			indexOfReturn++;
		}

		if (lextable.table[indexOfReturn].lexema == LEX_RETURN)
		{
			isReturnFound = true;
		}

		if (!isReturnFound)
		{
			throw ERROR_THROW_IN(321, lextable.table[pos].sn, pos);
		}

		int returnExpressionIndex = indexOfReturn + 1; 

		if (lextable.table[returnExpressionIndex].lexema == LEX_LITERAL || lextable.table[returnExpressionIndex].lexema == LEX_ID)
		{
			IT::IDDATATYPE returnExpressionType = idtable.table[lextable.table[returnExpressionIndex].idxTI].iddatatype;
			if (!IsMatchingType(returnExpressionType, functionReturnType) || (functionReturnType == IT::UINT && returnExpressionType == IT::INT &&
				idtable.table[lextable.table[returnExpressionIndex].idxTI].value.vint < 0))
			{
				throw ERROR_THROW_IN(322, lextable.table[returnExpressionIndex].sn, returnExpressionIndex);
			}

		}
		else
		{
			throw ERROR_THROW_IN(323, lextable.table[returnExpressionIndex].sn, returnExpressionIndex);
		}
	}

	void CheckArraySize(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::Entry arrayId = idtable.table[lextable.table[pos - 2].idxTI];

		if (lextable.table[pos - 2].lexema == LEX_ID && (lextable.table[pos].lexema == LEX_LITERAL || lextable.table[pos].lexema == LEX_ID))
		{
			if(lextable.table[pos].idxTI != TI_NULLIDX	&& idtable.table[lextable.table[pos].idxTI].iddatatype == IT::UINT)
			{
				if (idtable.table[lextable.table[pos].idxTI].value.vuint >= arrayId.value.vuint)
				{
					throw ERROR_THROW_IN(324, lextable.table[pos].sn, pos);
				}
			}
			else if(lextable.table[pos].idxTI != TI_NULLIDX						   &&
					idtable.table[lextable.table[pos].idxTI].iddatatype == IT::INT &&
				    idtable.table[lextable.table[pos].idxTI].value.vint >= 0)
			{ 
				if (idtable.table[lextable.table[pos].idxTI].value.vint >= arrayId.value.vuint)
				{
					throw ERROR_THROW_IN(324, lextable.table[pos].sn, pos);
				}
			}
			else
			{
				throw ERROR_THROW_IN(325, lextable.table[pos].sn, pos);
			}
		}
		
		return;
	}

	void CheckArrayAssignment(int pos, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::IDDATATYPE arrayType = idtable.table[lextable.table[pos - 2].idxTI].iddatatype;
		int arraySize = 0;

		for (int i = pos + 1; lextable.table[i].lexema != LEX_RIGHTBRACE; i++)
		{
			if (lextable.table[i].lexema == LEX_COMMA)
			{
				continue;
			}

			if (lextable.table[i].lexema == LEX_LITERAL)
			{
				if (!IsMatchingType(arrayType, idtable.table[lextable.table[i].idxTI].iddatatype))
				{
					throw ERROR_THROW_IN(326, lextable.table[i].sn, i);
				}

				++arraySize;
			}
			else
			{
				throw ERROR_THROW_IN(327, lextable.table[i].sn, i);
			}
		}

		if (lextable.table[pos - 2].idxTI != TI_NULLIDX && arraySize != idtable.table[lextable.table[pos - 2].idxTI].value.vuint)
		{
			throw ERROR_THROW_IN(328, lextable.table[pos].sn, pos);
		}

		return;
	}

	bool —heckIfWhile—omparison(int pos, const LT::LexTable& lextable)
	{
		return (lextable.table[pos + 1].lexema == LEX_ID || lextable.table[pos + 1].lexema == LEX_LITERAL)					&&
			   (lextable.table[pos + 3].lexema == LEX_ID || lextable.table[pos + 3].lexema == LEX_LITERAL)				    &&
			   (lextable.table[pos + 2].lexema == LEX_MORE || lextable.table[pos + 2].lexema == LEX_LESS					||
				lextable.table[pos + 2].lexema == LEX_EQUAL || lextable.table[pos + 2].lexema == LEX_NOT_EQUAL				||
				lextable.table[pos + 2].lexema == LEX_MORE_OR_EQUAL || lextable.table[pos + 2].lexema == LEX_LESS_OR_EQUAL) &&
			    lextable.table[pos + 4].lexema == LEX_RIGHTTHESIS;
	}

	bool CheckDataTypeCompatibility(int leftType, int rightType)
	{
		return (leftType == rightType && leftType != IT::STRING && rightType != IT::STRING) ||
			   (leftType == IT::UINT && rightType == IT::INT)								||
			   (leftType == IT::INT && rightType == IT::UINT);
	}

	void CheckCodeBlock(int startPos, LT::LexTable& lextable)
	{
		int braceCount = 0; 

		for (int i = startPos; i < lextable.size; ++i)
		{
			if (lextable.table[i].lexema == LEX_LEFTBRACE)
			{
				++braceCount;
			}
			else if (lextable.table[i].lexema == LEX_RIGHTBRACE)
			{
				--braceCount;
				if (braceCount == 0) 
				{
					if (i == startPos + 2) 
					{
						throw ERROR_THROW_IN(332, lextable.table[startPos].sn, startPos);
					}

					return; 
				}
			}
		}
	}

	bool IsMatchingType(int leftType, int rightType)
	{
		return leftType == rightType						 ||
			  (leftType == IT::INT && rightType == IT::UINT) ||
			  (leftType == IT::UINT && rightType == IT::INT);
	}

	bool IsArithmeticalOperation(char lexema)
	{
		return (lexema == LEX_PLUS || lexema == LEX_MINUS || lexema == LEX_STAR || lexema == LEX_DIRSLASH || lexema == LEX_PERCENT || 
			    lexema == LEX_EQUAL_SIGN);
	}

	bool IsLogicalOperation(char lexema)
	{
		return (lexema == LEX_MORE || lexema == LEX_LESS || lexema == LEX_MORE_OR_EQUAL || lexema == LEX_LESS_OR_EQUAL || lexema == LEX_EQUAL || 
			    lexema == LEX_NOT_EQUAL);
	}
}
