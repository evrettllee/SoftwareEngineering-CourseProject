#pragma once
#include "LT.h"
#include "IT.h"
#define LA_MAXSIZE		4096
#define LA_MARK			'\''
#define LA_NEW_LINE		'|'
#define LA_SEMICOLON	';'
#define LA_COMMA		','
#define LA_LEFTIND		'['
#define LA_RIGHTIND		']'
#define LA_LEFTBRACE	'{'
#define LA_RIGHTBRACE	'}'
#define LA_LEFTTHESIS	'('
#define LA_RIGHTTHESIS	')'
#define LA_PLUS			'+'
#define LA_MINUS		'-'
#define LA_STAR			'*'
#define LA_EQUAL		'='
#define LA_DIRSLASH		'/'
#define LA_PERCENT		'%'
#define LA_MORE			'>'
#define LA_LESS			'<'
#define LA_NOT			'!'


namespace LA
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	 idtable;
	};
	char FST(char* str);
	void ProcessMainEntry(const char* buffer, const std::vector<char*>& scope,IT::IdTable& idtable, LT::Entry& currentEntryLT, 
						  IT::Entry& currentEntryIT,const int currentLine, const int column);
	void ProcessStringLiteral(IT::Entry& currentEntryIT, const char* buffer);
	void ProcessIntegerLiteral(LT::LexTable& lextable, IT::Entry& currentEntryIT, const char* buffer, const In::IN& in, const int i, const int currentLine, 
		                       const int column);
	void ProcessBooleanLiteral(IT::Entry& currentEntryIT);
	void ProcessLiteralEntry(const char* buffer, const std::vector<char*>& scope, LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT,
							 IT::Entry& currentEntryIT, const In::IN& in, const int currentLine, int& numberOfLiterals, const int i, const int column);
	void ProcessDateTimeLibraryFunctions(LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, IT::IdTable& idtable, const int currentLine);
	void ProcessFunctionDeclaration(LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, int currentLine,
								    int column);
	void ProcessFunctionParametr(LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, int currentLine,
							     int column, int& countOfParams);
	void ProcessFunctionParametrs(LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, int currentLine,
								  int column, int& countOfParams);
	void ProcessVariableDeclaration(LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, int currentLine,
								    int column);
	void ProcessArrayDeclaration(LT::LexTable& lextable, IT::IdTable& idtable, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT, int currentLine,
								 int column);
	void ProcessReadingStringLiteral(char* buffer, int& bufferIndex, int& column, int& i, const int line, const In::IN& in);
	void SearchIdInExternalScope(IT::IdTable& idtable, const std::vector<char*>& scope, LT::Entry& currentEntryLT, IT::Entry& currentEntryIT,
								 const int currentLine, const int column);
	LEX LA(Parm::PARM parm, In::IN in, Log::LOG& log);
}