#include "pch.h"

int _tmain(int argc, _TCHAR* argv[])
{	
	setlocale(LC_ALL, "Russian");
	Parm::PARM parm;
	Log::LOG log = Log::INITLOG;
 	Out::OUT out = Out::INITOUT;
	try
	{
		parm = Parm::GetParm(argc, argv);
		log = Log::GetLog(parm.log);
		out = Out::GetOut(parm.out);
		In::IN in = In::GetIn(parm.in);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		LA::LEX lex = LA::LA(parm, in, log);

		MFST_TRACE_START(*log.stream)
		MFST::Mfst mfst(lex.lextable, GRB::GetGreibach());
		mfst.Start(log);
		mfst.SaveDeducation();
		mfst.PrintRules(log);

		SA::SA(lex.lextable, lex.idtable, log);

		PN::PN(lex.lextable, lex.idtable);
		PN::PrintModifiedExpressions(lex.lextable, log);

		GEN::Generation(lex, out);

		LT::Delete(lex.lextable);
		IT::Delete(lex.idtable);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ':' << e.message << std::endl << std::endl;
		if (e.intext.line != -1)
		{
			std::cout << "Строка:" << e.intext.line;
		}
		if (e.intext.col != -1)
		{
			std::cout << " Позиция:" << e.intext.col;
		}

		Log::WriteError(log, e);
	}

	Log::Close(log);
	Out::Close(out);

	return 0;
}


