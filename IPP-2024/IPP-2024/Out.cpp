#include "pch.h"

Out::OUT Out::GetOut(wchar_t outfile[])
{
	Out::OUT out = INITOUT;
	out.stream = new std::ofstream(outfile);

	if (!(*out.stream).is_open())
	{
		throw ERROR_THROW(116);
	}

	std::wcscpy(out.outfile, outfile);

	return out;
}

void Out::WriteOut(OUT out, In::IN in)
{
	(*out.stream) << in.text;
}

void Out::WriteError(OUT out, Error::ERROR error)
{
	if (error.id == 100)
	{
		(*out.stream) << "Ошибка " << error.id << ": " << error.message << "\n";
	}
	else
	{
		(*out.stream) << "Ошибка " << error.id << ": " << error.message << " "
			<< "строка " << error.intext.line << ", позиция: " << error.intext.col << "\n";
	}
}

void Out::Close(OUT out)
{
	(*out.stream).close();
	delete out.stream;
}
