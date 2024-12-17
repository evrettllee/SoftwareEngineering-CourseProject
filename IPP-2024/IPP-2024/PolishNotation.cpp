#include "pch.h"

namespace PN
{
	void PN(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema == LEX_EQUAL_SIGN)
			{
				if (lextable.table[i + 2].lexema == LEX_SEMICOLON)
				{
					continue;
				}
				else
				{
					PolishNotation(++i, lextable, idtable, LEX_SEMICOLON);
				}
			}
			if (lextable.table[i].lexema == LEX_IF || lextable.table[i].lexema == LEX_WHILE)
			{
                i += 2;
                PolishNotation(i, lextable, idtable, LEX_RIGHTTHESIS);
			}
		}
	}

    void PolishNotation(int pos, LT::LexTable& lextable, IT::IdTable& idtable, char endLexem)
    {
        std::stack<LT::Entry> operatorsStack;
        std::queue<LT::Entry> output;
        int countOfLex = 0;
        int expressionPosition = pos;

        for (expressionPosition; lextable.table[expressionPosition].lexema != endLexem; expressionPosition++, countOfLex++)
        {
            switch (lextable.table[expressionPosition].lexema)
            {
            case LEX_ID:
                if (idtable.table[lextable.table[expressionPosition].idxTI].idtype == IT::F)
                {
                    operatorsStack.push(lextable.table[expressionPosition]);
                }
                else
                {
                    output.push(lextable.table[expressionPosition]);
                }
                break;
            case LEX_LITERAL:
                output.push(lextable.table[expressionPosition]);
                break;
            case LEX_LEFTTHESIS:
                if (lextable.table[expressionPosition - 1].lexema == LEX_ID && lextable.table[expressionPosition - 1].idxTI != TI_NULLIDX &&
                    idtable.table[lextable.table[expressionPosition - 1].idxTI].idtype == IT::F)
                {
                    output.push(LT::Entry{ '$', lextable.table[expressionPosition].sn, -1 });
                }
                operatorsStack.push(lextable.table[expressionPosition]);
                break;
            case LEX_RIGHTTHESIS:
                while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTTHESIS)
                {
                    output.push(operatorsStack.top());
                    operatorsStack.pop();
                }
                if (!operatorsStack.empty() && operatorsStack.top().lexema == LEX_LEFTTHESIS)
                {
                    operatorsStack.pop();
                }

                if (!operatorsStack.empty() && operatorsStack.top().idxTI != TI_NULLIDX && idtable.table[operatorsStack.top().idxTI].idtype == IT::F)
                {
                    output.push(operatorsStack.top());
                    operatorsStack.pop();
                }
                break;
            case LEX_COMMA:
                while (!operatorsStack.empty() && operatorsStack.top().lexema != LEX_LEFTTHESIS)
                {
                    output.push(operatorsStack.top());
                    operatorsStack.pop();
                }
                break;
            case LEX_PLUS:
            case LEX_MINUS:
            case LEX_STAR:
            case LEX_DIRSLASH:
            case LEX_PERCENT:
            case LEX_LESS:
            case LEX_MORE:
            case LEX_MORE_OR_EQUAL:
            case LEX_LESS_OR_EQUAL:
            case LEX_EQUAL:
            case LEX_NOT_EQUAL:
                if (lextable.table[expressionPosition].lexema == LEX_MINUS && lextable.table[expressionPosition + 1].lexema == LEX_ID &&
                    lextable.table[expressionPosition + 1].idxTI != TI_NULLIDX && idtable.table[lextable.table[expressionPosition + 1].idxTI].idtype == IT::F)
                {
                    output.push(lextable.table[expressionPosition]);
                    continue;
                }
                while (!operatorsStack.empty() && GetPriority(lextable.table[expressionPosition], idtable) <= GetPriority(operatorsStack.top(), idtable))
                {
                    output.push(operatorsStack.top());
                    operatorsStack.pop();
                }
                operatorsStack.push(lextable.table[expressionPosition]);
                break;
            default:
                break;
            }
        }

        while (!operatorsStack.empty())
        {
            output.push(operatorsStack.top());
            operatorsStack.pop();
        }

        for (int i = 0; i < countOfLex; i++)
        {
            if (!output.empty())
            {
                lextable.table[pos + i] = output.front();
                output.pop();
            }
            else
            {
                lextable.table[pos + i] = LT::Entry{ PN_FILLER, lextable.table[pos].sn, -1 };
            }
        }
    }

	int GetPriority(LT::Entry entry, IT::IdTable& idtable)
	{
        switch (entry.lexema)
        {
        case LEX_LEFTTHESIS:
        case LEX_RIGHTTHESIS:
            return 0;
        case LEX_COMMA:
            return 1;
        case LEX_PLUS:
        case LEX_MINUS:
            return 2;
        case LEX_MORE:
        case LEX_LESS:
        case LEX_MORE_OR_EQUAL:
        case LEX_LESS_OR_EQUAL:
        case LEX_EQUAL:
        case LEX_NOT_EQUAL:
            return 3;
        case LEX_STAR:
        case LEX_DIRSLASH:
        case LEX_PERCENT:
            return 4;
        case LEX_IND:
            return 5;
        }
		
		return -1;
	}

    void PrintModifiedExpressions(LT::LexTable& lextable, Log::LOG& log)
    {
        (*log.stream) << "\n--------------------------------- Распечатка выражений, после преобразования в ПОЛИЗ ---------------------------------\n";
        int currentLine = 1;
        (*log.stream) << currentLine << '\t';
        for (int i = 0; i < lextable.size; i++)
        {
            LT::Entry temp = LT::GetEntry(lextable, i);

            if (currentLine != temp.sn)
            {
                currentLine = temp.sn;
                (*log.stream) << '\n' << currentLine << '\t';
            }

            (*log.stream) << temp.lexema;
        }
        (*log.stream) << "\n-----------------------------------------------------------------------------------------------------------------------\n";
    }
}

