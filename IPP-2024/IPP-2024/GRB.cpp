#include "pch.h"

namespace GRB
{
	Greibach greibach(
		NS('S'),
		TS('$'),
		7,
		Rule(
			NS('S'),
			GRB_ERROR_SERIES + 0,
			39,
			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS(';')),
			Rule::Chain(11, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS('='), TS('{'), NS('A'), TS('}'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(6, TS('i'), TS('@'), TS('l'), TS('='), NS('E'), TS(';')),
			Rule::Chain(6, TS('i'), TS('@'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('w'), NS('E'), TS(';')),
			Rule::Chain(3, TS('x'), NS('E'), TS(';')),
			Rule::Chain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(6, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(9, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('S')),
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('S')),
			Rule::Chain(8, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS(';'), NS('S')),
			Rule::Chain(12, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS('='), TS('{'), NS('A'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('S')),
			Rule::Chain(7, TS('i'), TS('@'), TS('l'), TS('='), NS('E'), TS(';'), NS('S')),
			Rule::Chain(7, TS('i'), TS('@'), TS('i'), TS('='), NS('E'), TS(';'), NS('S')),
			Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('S')),
			Rule::Chain(4, TS('x'), NS('E'), TS(';'), NS('S')),
			Rule::Chain(8, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(12, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('S')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}'), NS('S')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}'), NS('S'))
			),
		Rule(
			NS('N'),
			GRB_ERROR_SERIES + 1,
			30,
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(6, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS(';')),
			Rule::Chain(11, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS('='), TS('{'), NS('A'), TS('}'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(6, TS('i'), TS('@'), TS('l'), TS('='), NS('E'), TS(';')),
			Rule::Chain(6, TS('i'), TS('@'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(3, TS('w'), NS('E'), TS(';')),
			Rule::Chain(3, TS('x'), NS('E'), TS(';')),
			Rule::Chain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(6, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), TS('}')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(9, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}')),
			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(7, TS('n'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(8, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(12, TS('n'), TS('t'), TS('a'), TS('@'), TS('l'), TS('i'), TS('='), TS('{'), NS('A'), TS('}'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(7, TS('i'), TS('@'), TS('l'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(7, TS('i'), TS('@'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('w'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('x'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(8, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(7, TS('h'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(12, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), NS('N'), TS('}'), NS('N')),
			Rule::Chain(11, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS('e'), TS('{'), TS('}'), NS('N')),
			Rule::Chain(10, TS('z'), TS('('), NS('E'), TS(')'), TS('{'), TS('}'), TS('e'), TS('{'), TS('}'), NS('N'))
		),
		Rule(
			NS('E'),
			GRB_ERROR_SERIES + 2,
			14,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS('@'), TS('l')),
			Rule::Chain(3, TS('i'), TS('@'), TS('i')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('i'), TS('@'), TS('l'), NS('M')),
			Rule::Chain(4, TS('i'), TS('@'), TS('i'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		Rule(
			NS('M'),
			GRB_ERROR_SERIES + 3,
			16,
			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(2, TS('/'), NS('E')),
			Rule::Chain(2, TS('%'), NS('E')),
			Rule::Chain(2, TS('>'), NS('E')),
			Rule::Chain(2, TS('<'), NS('E')),
			Rule::Chain(2, TS('&'), NS('E')),
			Rule::Chain(2, TS('j'), NS('E')),
			Rule::Chain(2, TS('p'), NS('E')),
			Rule::Chain(2, TS('k'), NS('E')),
			Rule::Chain(3, TS('+'), NS('E'), NS('M')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),
			Rule::Chain(3, TS('*'), NS('E'), NS('M')),
			Rule::Chain(3, TS('/'), NS('E'), NS('M')),
			Rule::Chain(3, TS('%'), NS('E'), NS('M'))
		),
		Rule(
			NS('F'),
			GRB_ERROR_SERIES + 4,
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'),
			GRB_ERROR_SERIES + 5,
			4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		Rule(
			NS('A'),
			GRB_ERROR_SERIES + 6,
			2,
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('l'), TS(','), NS('A'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		(*this).nt = new GRBALPHABET[(*this).size = psize];
		int* ptr = (int*)&s;
		for (int i = 0; i < psize; i++)
		{
			(*this).nt[i] = (short)ptr[i];
		}
	}

	Rule::Rule(GRBALPHABET pnn, int iderroe, short psize, Chain c, ...)
	{
		(*this).nn = pnn;
		(*this).iderror = iderroe;
		(*this).chains = new Chain[(*this).size = psize];
		Chain* ptr = &c;
		for (int i = 0; i < psize; i++)
		{
			(*this).chains[i] = ptr[i];
		}
	}

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...)
	{
		(*this).startN = pstartN;
		(*this).stbottomT = pstbottomT;
		(*this).rules = new Rule[(*this).size = psize];
		Rule* ptr = &r;
		for (int i = 0; i < psize; i++)
		{
			rules[i] = ptr[i];
		}
	}

	short Greibach::GetRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1, k = 0;
		while (k < (*this).size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < (*this).size)
		{
			prule = rules[rc = k];
		}

		return rc;
	}

	Rule Greibach::GetRule(short n)
	{
		Rule rc;
		if (n < (*this).size)
		{
			rc = rules[n];
		}

		return rc;
	}

	char* Rule::GetCRule(char* b, short nchain)
	{
		char buf[200];
		b[0] = Chain::AlphabetToChar(this->nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		this->chains[nchain].GetCChain(buf);
		strcat_s(b, sizeof(buf) + 5, buf);

		return b;
	}

	short Rule::GetNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < (*this).size && (*this).chains[j].nt[0] != t)
		{
			j++;
		}
		rc = (j < (*this).size ? j : -1);
		if (rc >= 0)
		{
			pchain = chains[rc];
		}

		return rc;
	}

	char* Rule::Chain::GetCChain(char* b)
	{
		for (int i = 0; i < this->size; i++)
		{
			b[i] = Chain::AlphabetToChar(this->nt[i]);
		}
		b[(*this).size] = 0;
		
		return b;
	}

	Greibach GetGreibach()
	{
		return greibach;
	}
}