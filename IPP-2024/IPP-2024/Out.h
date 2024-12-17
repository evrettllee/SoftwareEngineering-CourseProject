#pragma once
#include "pch.h"

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		std::ofstream* stream;
	};
	static const OUT INITOUT{ L"",NULL };				
	OUT GetOut(wchar_t outfile[]);						
	void WriteOut(OUT out, In::IN in);					
	void WriteError(OUT out, Error::ERROR error);		
	void Close(OUT out);
}
