#pragma once
#define LT_MAXSIZE				4096
#define	LT_TI_NULLXDX			0XFFFFFFFF
#define LEX_INTEGER				't'
#define LEX_UNSIGNED_INTEGER	't'
#define LEX_DECIMAL				't'
#define LEX_STRING				't'
#define LEX_BOOL				't'
#define LEX_ID					'i'
#define LEX_LITERAL				'l'
#define LEX_FUNCTION			'f'
#define LEX_MAIN				'm'
#define LEX_WRITE				'w'
#define LEX_WRITELINE			'x'
#define LEX_WHILE				'h'
#define LEX_IF					'z'
#define LEX_ELSE				'e'
#define LEX_NEW					'n'
#define LEX_RETURN				'r'
#define LEX_ARRAY				'a'
#define LEX_SEMICOLON			';'
#define LEX_COMMA				','
#define LEX_LEFTBRACE			'{'
#define LEX_RIGHTBRACE			'}'
#define LEX_LEFTTHESIS			'('
#define LEX_RIGHTTHESIS			')'
#define LEX_PLUS				'+'
#define LEX_MINUS				'-'
#define	LEX_STAR				'*'			
#define	LEX_DIRSLASH			'/'
#define LEX_PERCENT				'%'
#define LEX_MORE				'>'
#define LEX_LESS				'<'
#define LEX_EQUAL				'&'
#define LEX_NOT_EQUAL			'j'
#define LEX_MORE_OR_EQUAL		'p'
#define LEX_LESS_OR_EQUAL		'k'
#define	LEX_EQUAL_SIGN			'='
#define LEX_IND					'@'
#define LEX_PARAMS				'$'
#define DATE					"DATE"
#define TIME					"TIME"
#define LT_FILENAME				"X:\\Курсовой проект\\IPP-2024\\Debug\\LT.txt"


namespace LT									
{
	struct Entry								
	{
		char lexema;							
		int sn;									
		int idxTI = LT_TI_NULLXDX;				

		Entry();
		Entry(char lexema, int sn, int idxTI);
	};
	struct LexTable								
	{
		int maxsize;							
		int size;								
		Entry* table;							
	};
	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void WriteTable(LexTable& lextable);
	void Delete(LexTable& lextable);
};
