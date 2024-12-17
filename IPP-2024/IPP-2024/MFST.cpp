#include "pch.h"

namespace MFST
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	};

	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	};

	Mfst::Mfst() 
	{ 
		lenta = 0; 
		lenta_size = 0;
		lenta_position = 0;
	};

	Mfst::Mfst(LT::LexTable& lextable, GRB::Greibach pgrebach)
	{
		grebach = pgrebach;
		lex = lextable;
		lenta = new short[lenta_size = lex.size];
		for (int k = 0; k < lenta_size; k++)
		{
			lenta[k] = GRB::Rule::Chain::T(lex.table[k].lexema);
		}
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;
	}

	Mfst::RC_STEP Mfst::Step(Log::LOG& log)
	{
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
 			if (GRB::Rule::Chain::isN(st.top()))
			{
				GRB::Rule rule;
				if ((nrule = grebach.GetRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.GetNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
					{
						MFST_TRACE1(*log.stream)
						SaveState(log); 
						st.pop(); 
						PushChain(chain); 
						rc = NS_OK;
						MFST_TRACE2(*log.stream)
					}
					else
					{
						MFST_TRACE4(*log.stream, "TNS_NORULECHAIN/NS_NORULE")
						SaveDiagnosis(NS_NORULECHAIN); 
						rc = ResetState(log) ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else 
				{
					rc = NS_ERROR;
				}
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++; 
				st.pop();
				nrulechain = -1; 
				rc = TS_OK;
				MFST_TRACE3(*log.stream)
			}
			else 
			{ 
				MFST_TRACE4(*log.stream, TS_NOK / NS_NORULECHAIN) 
				rc = ResetState(log) ? TS_NOK : NS_NORULECHAIN; 
			};
		}
		else
		{
			rc = LENTA_END;
			MFST_TRACE4(*log.stream, LENTA_END);
		};

		return rc;
	};

	bool Mfst::PushChain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) 
		{
			st.push(chain.nt[k]);
		}

		return true;
	};

	bool Mfst::SaveState(Log::LOG& log)
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6(*log.stream, "SAVESTATE:", storestate.size());

		return true;
	};

	bool Mfst::ResetState(Log::LOG& log)
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5(*log.stream, "RESSTATE")
			MFST_TRACE2(*log.stream)
		};

		return rc;
	};

	bool Mfst::SaveDiagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;

		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}

		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);

			for (int i = k + 1; i < MFST_DIAGN_NUMBER; i++)
			{
				diagnosis[i].lenta_position = -1;
			}
		}

		return rc;
	};

	bool Mfst::Start(Log::LOG& log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE]{};
		rc_step = Step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = Step(log);
		}

		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4(*log.stream, "------>LENTA_END")
			(*log.stream) << "------------------------------------------------------------------------------------------------" << std::endl << std::endl;
			(*log.stream) << "------------------------------------------------------------------------------------------------" << std::endl;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			(*log.stream) << std::setw(4) << std::left << 0 << "всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок" << std::endl;
			(*log.stream) << "------------------------------------------------------------------------------------------------" << std::endl;
			rc = true;
			break;
		}
		case NS_NORULE:
		{
			MFST_TRACE4(*log.stream, "------>NS_NORULE")
			std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
			std::cout << GetDiagnosis(0, buf) << std::endl;
			*log.stream << GetDiagnosis(0, buf) << std::endl;
			std::cout << GetDiagnosis(1, buf) << std::endl;
			*log.stream << GetDiagnosis(1, buf) << std::endl;
			std::cout << GetDiagnosis(2, buf) << std::endl;
			*log.stream << GetDiagnosis(2, buf) << std::endl;
			throw ERROR_THROW(207); 
			break;
		}
		case NS_NORULECHAIN:	
			MFST_TRACE4(*log.stream, "------>NS_NORULECHAIN") 
			break;
		case NS_ERROR:			
			MFST_TRACE4(*log.stream, "------>NS_ERROR") 
			break;
		case SURPRISE:			
			MFST_TRACE4(*log.stream, "------>NS_SURPRISE") 
			break;
		}
		return rc;
	};

	char* Mfst::GetCSt(char* buf)
	{
		short p;
		for (int k = (signed)st.size() - 1; k >= 0; --k)
		{
			p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::AlphabetToChar(p);
		}
		buf[st.size()] = '\0';

		return buf;
	}

	char* Mfst::GetCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++) 
		{
			buf[i - pos] = GRB::Rule::Chain::AlphabetToChar(lenta[i]);
		}
		buf[i - pos] = 0x00;
		
		return buf;
	}

	char* Mfst::GetDiagnosis(short n, char* buf)
	{
		char* rc = new char[200] {};
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.GetRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::GetError(errid);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d,%s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		}

		return rc;
	}

	void Mfst::PrintRules(Log::LOG& log)
	{
		MfstState state;
		GRB::Rule rule;
		(*log.stream) << "\n--------------------------------- Дерево разбора ---------------------------------\n";
		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			rule = grebach.GetRule(state.nrule);
			MFST_TRACE7(*log.stream)
		};
		(*log.stream) << "-----------------------------------------------------------------------------------\n";
	};

	bool Mfst::SaveDeducation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];

		for (unsigned short i = 0; i < storestate.size(); i++)
		{
			state = storestate.c[i];
			deducation.nrules[i] = state.nrule;
			deducation.nrulechains[i] = state.nrulechain;
		}

		return true;
	}
}