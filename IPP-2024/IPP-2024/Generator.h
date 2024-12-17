#pragma once
#include "pch.h"

namespace GEN
{
	void Generation(LA::LEX lex, Out::OUT& out);
	void Head(Out::OUT& out);
	void Const(LA::LEX lex, Out::OUT& out);
	void Data(LA::LEX lex, Out::OUT& out);
	void Code(LA::LEX lex, Out::OUT& out);
	void CreateNameWithScope(char*& buffer, const IT::Entry entry);
}