#pragma once
#include "pch.h"
#define PN_FILLER	'#'

namespace PN
{
	void PN(LT::LexTable& lextable, IT::IdTable& idtable);
	int GetPriority(LT::Entry entry, IT::IdTable& idtable);
	void PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable, char endLexem);
	void PrintModifiedExpressions(LT::LexTable& lextable, Log::LOG& log);
}