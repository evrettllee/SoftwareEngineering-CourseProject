#pragma once
#include "pch.h"
#define PARM_IN					L"-in:"							
#define PARM_OUT				L"-out:"						
#define PARM_LOG				L"-log:"
#define PARM_LOG_DEFAULT_NAME	L"log"
#define PARM_OUT_DEFAULT_NAME	L"out"
#define PARM_LOG_DEFAULT_EXT	L".log"
#define PARM_OUT_DEFAULT_EXT	L".asm"
#define PARM_MAX_SIZE			300										


namespace Parm										
{
	struct PARM										
	{
		wchar_t in[PARM_MAX_SIZE];					
		wchar_t out[PARM_MAX_SIZE];					
		wchar_t log[PARM_MAX_SIZE];					
	};

	PARM GetParm(int argc,wchar_t* argv[]);		
};