#pragma once
#define TI_ID_MAXSIZE				16
#define TI_MAXSIZE					4096
#define TI_UINT_DEFAULT				0x00000000
#define TI_STR_DEFAULT				'\0'
#define TI_BOOL_DEFAULT				false
#define TI_NULLIDX					0xffffffff
#define TI_STR_MAXSIZE				255
#define TI_UINT_MINSIZE				0
#define TI_UINT_MAXSIZE				UINT_MAX
#define TI_INT_MINSIZE				INT_MIN
#define TI_INT_MAXSIZE				INT_MAX
#define TI_INT_LITERAL_MAXSIZE		UINT_MAX
#define TI_INT_LITERAL_MINSIZE		INT_MIN
#define TI_FILENAME					"X:\\Курсовой проект\\IPP-2024\\Debug\\IT.txt"


namespace IT
{
	enum IDDATATYPE { VOID = 0, UINT = 1, STRING = 2, BOOL = 3, INT = 4 };																	
	enum IDTYPE		{ V = 1, F = 2, P = 3, L = 4, A = 5};																				
	struct Entry
	{
		int idxfirstLE;													
		char id[TI_ID_MAXSIZE];											
		char scope[TI_ID_MAXSIZE];
		IDDATATYPE iddatatype;											
		IDTYPE idtype;													
		union
		{
			bool vbool;													
			unsigned int vuint;										
			int vint;													
			struct
			{
				int len;												
				char str[TI_STR_MAXSIZE];							
			} vstr;														
		} value;														
		Entry();
	};
	struct IdTable														
	{
		int maxsize;													
		int size;														
		Entry* table;													
	};
	IdTable Create(int size);
	void Add(IdTable& idtable, Entry entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, char id[TI_ID_MAXSIZE]);
	int Search(IdTable& idtable, Entry entry);
	void WriteTable(IdTable& idtable);
	void Delete(IdTable& idtable);
}