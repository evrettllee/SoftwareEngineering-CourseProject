#pragma once
#include "pch.h"
typedef short				GRBALPHABET;
#define GRB_ERROR_SERIES	200
#define NS(n)				GRB::Rule::Chain::N(n)
#define TS(n)				GRB::Rule::Chain::T(n)
#define ISNS(n)				GRB::Rule::Chain::isN(n)

namespace GRB
{
	struct Rule															
	{
		GRBALPHABET nn;													
		int iderror;													
		short size;		

		struct Chain													
		{
			short size;													
			GRBALPHABET* nt;

			Chain()
			{
				(*this).size = 0; 										
				(*this).nt = 0;											
			};
			Chain(short psize, GRBALPHABET s, ...);						
			char* GetCChain(char* b);									
			static GRBALPHABET T(char t)								
			{ 
				return GRBALPHABET(t); 
			};	
			static GRBALPHABET N(char n)								
			{ 
				return -GRBALPHABET(n); 
			};	
			static bool IsT(GRBALPHABET s)								
			{ 
				return s > 0; 
			};			
			static bool isN(GRBALPHABET s)								
			{ 
				return !IsT(s); 
			};			
			static char AlphabetToChar(GRBALPHABET s)					
			{
				return IsT(s) ? char(s) : char(-s);
			};
		} *chains;	

		Rule()
		{
			(*this).iderror = -1;
			(*this).nn = 0x00;
			(*this).size = 0;
			(*this).chains = nullptr;
		}
		Rule(GRBALPHABET pnn, int iderror, short psize, Chain c, ...);
		char* GetCRule(char* b, short nchain); 
		short GetNextChain(GRBALPHABET t, Rule::Chain& pchain, short j); 	
	};

	struct Greibach														
	{
		short size;														
		GRBALPHABET startN;												
		GRBALPHABET stbottomT;											
		Rule* rules;				

		Greibach() { (*this).size = 0; (*this).startN = 0; (*this).stbottomT = 0; (*this).rules = 0; };
		Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...);
		short GetRule(GRBALPHABET pnn, Rule& prule);
		Rule GetRule(short n);
	};

	Greibach GetGreibach();
}

