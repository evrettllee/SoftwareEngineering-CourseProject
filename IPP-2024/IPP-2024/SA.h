#pragma once

namespace SA
{
	void SA(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG& log);
	void HandleArrayIndexing(int& pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void HandleAssignment(int& pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckConditionExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void ValidateComparison(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	bool IsBooleanExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	bool IsComparisonExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckVariableAssignment(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckFunctionArguments(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckIfWhileExpression(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckFunctionReturnType(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckArraySize(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckArrayAssignment(int pos, LT::LexTable& lextable, IT::IdTable& idtable);
	bool —heckIfWhile—omparison(int pos, const LT::LexTable& lextable);
	bool CheckDataTypeCompatibility(int leftType, int rightType);
	void CheckCodeBlock(int startPos, LT::LexTable& lextable);
	bool IsMatchingType(int leftType, int rightType);
	bool IsArithmeticalOperation(char lexema);
	bool IsLogicalOperation(char lexema);
}
