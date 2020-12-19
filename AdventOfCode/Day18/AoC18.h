// Auto-generated file. Do not manually modify

#pragma once

#include <cassert>

namespace aoc18
{
	
	enum class Terminal
	{
		ERROR,
		END,
		NUMBER,
		PLUS,
		MULT,
		PARENTHESIS_OPEN,
		PARENTHESIS_CLOSE,
		QUADRIGA_ANONYM_0 = PARENTHESIS_OPEN, // '('
		QUADRIGA_ANONYM_1 = PARENTHESIS_CLOSE, // ')'
		QUADRIGA_ANONYM_2 = PLUS, // '+'
		QUADRIGA_ANONYM_3 = MULT, // '*'
		QUADRIGA_ANONYM_4 = PARENTHESIS_OPEN, // '('
		QUADRIGA_ANONYM_5 = PARENTHESIS_CLOSE, // ')'
		QUADRIGA_ANONYM_6 = PLUS, // '+'
		QUADRIGA_ANONYM_7 = MULT, // '*'
	};
	
	inline const char32_t* ToString(Terminal terminal)
	{
		switch(terminal)
		{
		case Terminal::ERROR: return U"ERROR";
		case Terminal::END: return U"END";
		case Terminal::NUMBER: return U"NUMBER";
		case Terminal::PLUS: return U"PLUS";
		case Terminal::MULT: return U"MULT";
		case Terminal::PARENTHESIS_OPEN: return U"PARENTHESIS_OPEN";
		case Terminal::PARENTHESIS_CLOSE: return U"PARENTHESIS_CLOSE";
		default: return U"UNKNOWN TERMINAL!";
		}
	}
	
	struct TerminalData
	{
		Terminal type;
		const char32_t *textBeginning;
		int textSize;
		
		TerminalData() = default;
		TerminalData(Terminal _type) : type(_type), textBeginning(nullptr), textSize(0) {}
		TerminalData(Terminal _type, const char32_t *_textBeginning, int _textSize) : type(_type), textBeginning(_textBeginning), textSize(_textSize) {}
		TerminalData(Terminal _type, const char32_t *_textBeginning, const char32_t *_textEnd) : type(_type), textBeginning(_textBeginning), textSize((int)(_textEnd - _textBeginning)) {}
	};
	
	
	class Scanner
	{
	public:
		static const int LLk = 1;
		
		const char32_t *text;
		int currentTextIndex;
		TerminalData nextSymbols[LLk];
		int currentSavedSymbols;
		
		
		Scanner(const char32_t *_text) :text(_text), currentTextIndex(0), currentSavedSymbols(0) {}
		
		
		TerminalData PeekNextSymbol(int index = 0)
		{
			assert(index < LLk);
			while (index >= currentSavedSymbols)
			{
				nextSymbols[currentSavedSymbols] = GetNextSymbol_Internal();
				++currentSavedSymbols;
			}
			return nextSymbols[index];
		}
		
		
		TerminalData GetNextSymbol()
		{
			if (currentSavedSymbols > 0)
			{
				TerminalData result = nextSymbols[0];
				--currentSavedSymbols;
				for (int i = 0; i < currentSavedSymbols; ++i)
				nextSymbols[i] = nextSymbols[i + 1];
				return result;
			}
			
				return GetNextSymbol_Internal();
		}
		
		TerminalData GetNextSymbol_Internal()
		{
			int state = 0;
			const char32_t *textBeginning = &text[currentTextIndex];
			
			
			for (;;)
			{
				char32_t character = text[currentTextIndex];
				
				switch (state)
				{
				case 0:
					switch (character)
					{
					case 0:
						return Terminal::END;
					case 9:
					case 10:
					case 13:
					case U' ':
						state = 1;
						break;
					case U'(':
						state = 2;
						break;
					case U')':
						state = 3;
						break;
					case U'*':
						state = 4;
						break;
					case U'+':
						state = 5;
						break;
					default:
						if ( ( character >= U'0' && character <= U'9' ) )
						{
							state = 6;
						}
						else
						{
							return Terminal::ERROR;
						}
						break;
					}
					break;
				case 1:
					switch (character)
					{
					case 9:
					case 10:
					case 13:
					case U' ':
						// state = 1;
						break;
					default:
						{
							textBeginning = &text[currentTextIndex];
							state = 0;
							--currentTextIndex;
						}
						break;
					}
					break;
				case 2:
					{
						return TerminalData(Terminal::PARENTHESIS_OPEN, textBeginning, &text[currentTextIndex]);
					}
					break;
				case 3:
					{
						return TerminalData(Terminal::PARENTHESIS_CLOSE, textBeginning, &text[currentTextIndex]);
					}
					break;
				case 4:
					{
						return TerminalData(Terminal::MULT, textBeginning, &text[currentTextIndex]);
					}
					break;
				case 5:
					{
						return TerminalData(Terminal::PLUS, textBeginning, &text[currentTextIndex]);
					}
					break;
				case 6:
					if ( ( character >= U'0' && character <= U'9' ) )
					{
						// state = 6;
					}
					else
					{
						return TerminalData(Terminal::NUMBER, textBeginning, &text[currentTextIndex]);
					}
					break;
				default:
					return Terminal::ERROR;
				}
				++currentTextIndex;
			}
		}
	};
	
	class Parser
	{
	public:
		
		bool ParseAdvancedAddition(Scanner &scanner)
		{
			if( !BeginAdvancedAddition() )
				return false;
			// < AdvancedParenthesis > < AdvancedAddition_SUB_1 >
			// < AdvancedParenthesis >
			if( !ParseAdvancedParenthesis(scanner))
				return false;
			// < AdvancedAddition_SUB_1 >
			if( !ParseAdvancedAddition_SUB_1(scanner))
				return false;
			if( !EndAdvancedAddition() )
				return false;
			return true;
		}
		
		bool ParseAdvancedAddition_SUB_1(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::PLUS:
				// PLUS < AdvancedParenthesis > < AdvancedAddition_SUB_1 >
				// PLUS
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::PLUS )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::PLUS);
						return false;
					}
				}
				// < AdvancedParenthesis >
				if( !ParseAdvancedParenthesis(scanner))
					return false;
				// < AdvancedAddition_SUB_1 >
				if( !ParseAdvancedAddition_SUB_1(scanner))
					return false;
				break;
			case Terminal::END:
			case Terminal::MULT:
			case Terminal::PARENTHESIS_CLOSE:
				// <>
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::PLUS, Terminal::END, Terminal::MULT, Terminal::PARENTHESIS_CLOSE };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseAdvancedMultiplication(Scanner &scanner)
		{
			if( !BeginAdvancedMultiplication() )
				return false;
			// < AdvancedAddition > < AdvancedMultiplication_SUB_1 >
			// < AdvancedAddition >
			if( !ParseAdvancedAddition(scanner))
				return false;
			// < AdvancedMultiplication_SUB_1 >
			if( !ParseAdvancedMultiplication_SUB_1(scanner))
				return false;
			if( !EndAdvancedMultiplication() )
				return false;
			return true;
		}
		
		bool ParseAdvancedMultiplication_SUB_1(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::MULT:
				// MULT < AdvancedAddition > < AdvancedMultiplication_SUB_1 >
				// MULT
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::MULT )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::MULT);
						return false;
					}
				}
				// < AdvancedAddition >
				if( !ParseAdvancedAddition(scanner))
					return false;
				// < AdvancedMultiplication_SUB_1 >
				if( !ParseAdvancedMultiplication_SUB_1(scanner))
					return false;
				break;
			case Terminal::END:
			case Terminal::PARENTHESIS_CLOSE:
				// <>
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::MULT, Terminal::END, Terminal::PARENTHESIS_CLOSE };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseAdvancedOperation(Scanner &scanner)
		{
			if( !BeginAdvancedOperation() )
				return false;
			// < AdvancedMultiplication > END
			// < AdvancedMultiplication >
			if( !ParseAdvancedMultiplication(scanner))
				return false;
			// END
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::END )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::END);
					return false;
				}
			}
			if( !EndAdvancedOperation() )
				return false;
			return true;
		}
		
		bool ParseAdvancedParenthesis(Scanner &scanner)
		{
			if( !BeginAdvancedParenthesis() )
				return false;
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::NUMBER:
				// NUMBER
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::NUMBER )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::NUMBER);
						return false;
					}
				}
				break;
			case Terminal::PARENTHESIS_OPEN:
				// < AdvancedParenthesis_SUB_1 >
				if( !ParseAdvancedParenthesis_SUB_1(scanner))
					return false;
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::NUMBER, Terminal::PARENTHESIS_OPEN };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			if( !EndAdvancedParenthesis() )
				return false;
			return true;
		}
		
		bool ParseAdvancedParenthesis_SUB_1(Scanner &scanner)
		{
			// PARENTHESIS_OPEN < AdvancedMultiplication > PARENTHESIS_CLOSE
			// PARENTHESIS_OPEN
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::PARENTHESIS_OPEN )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::PARENTHESIS_OPEN);
					return false;
				}
			}
			// < AdvancedMultiplication >
			if( !ParseAdvancedMultiplication(scanner))
				return false;
			// PARENTHESIS_CLOSE
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::PARENTHESIS_CLOSE )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::PARENTHESIS_CLOSE);
					return false;
				}
			}
			return true;
		}
		
		bool ParseOperation(Scanner &scanner)
		{
			if( !BeginOperation() )
				return false;
			// < Operation_SUB_1 > < Operation_SUB_2 >
			// < Operation_SUB_1 >
			if( !ParseOperation_SUB_1(scanner))
				return false;
			// < Operation_SUB_2 >
			if( !ParseOperation_SUB_2(scanner))
				return false;
			if( !EndOperation() )
				return false;
			return true;
		}
		
		bool ParseOperation_SUB_1(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::PARENTHESIS_OPEN:
				// < Parenthesis >
				if( !ParseParenthesis(scanner))
					return false;
				break;
			case Terminal::NUMBER:
				// NUMBER
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::NUMBER )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::NUMBER);
						return false;
					}
				}
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::PARENTHESIS_OPEN, Terminal::NUMBER };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseOperation_SUB_2(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::MULT:
			case Terminal::PLUS:
				// < Operation_SUB_3 > < Operation_SUB_4 > < Operation_SUB_2 >
				// < Operation_SUB_3 >
				if( !ParseOperation_SUB_3(scanner))
					return false;
				// < Operation_SUB_4 >
				if( !ParseOperation_SUB_4(scanner))
					return false;
				// < Operation_SUB_2 >
				if( !ParseOperation_SUB_2(scanner))
					return false;
				break;
			case Terminal::END:
			case Terminal::PARENTHESIS_CLOSE:
				// <>
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::MULT, Terminal::PLUS, Terminal::END, Terminal::PARENTHESIS_CLOSE };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseOperation_SUB_3(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::PLUS:
				// PLUS
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::PLUS )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::PLUS);
						return false;
					}
				}
				break;
			case Terminal::MULT:
				// MULT
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::MULT )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::MULT);
						return false;
					}
				}
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::PLUS, Terminal::MULT };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseOperation_SUB_4(Scanner &scanner)
		{
			switch (scanner.PeekNextSymbol().type)
			{
			case Terminal::PARENTHESIS_OPEN:
				// < Parenthesis >
				if( !ParseParenthesis(scanner))
					return false;
				break;
			case Terminal::NUMBER:
				// NUMBER
				{
					TerminalData terminal = scanner.GetNextSymbol();
					if ( terminal.type == Terminal::NUMBER )
					{
						if(! ParseTerminal(terminal) )
							return false;
					}
					else
					{
						ErrorUnexpectedTerminal(terminal, Terminal::NUMBER);
						return false;
					}
				}
				break;
			default:
				{
					static constexpr Terminal expectedTerminals[] = { Terminal::PARENTHESIS_OPEN, Terminal::NUMBER };
					ErrorUnexpectedTerminal(scanner.PeekNextSymbol(), expectedTerminals, sizeof(expectedTerminals) / sizeof(expectedTerminals[0]));
					return false;
				}
			}
			return true;
		}
		
		bool ParseParenthesis(Scanner &scanner)
		{
			if( !BeginParenthesis() )
				return false;
			// PARENTHESIS_OPEN < Operation > PARENTHESIS_CLOSE
			// PARENTHESIS_OPEN
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::PARENTHESIS_OPEN )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::PARENTHESIS_OPEN);
					return false;
				}
			}
			// < Operation >
			if( !ParseOperation(scanner))
				return false;
			// PARENTHESIS_CLOSE
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::PARENTHESIS_CLOSE )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::PARENTHESIS_CLOSE);
					return false;
				}
			}
			if( !EndParenthesis() )
				return false;
			return true;
		}
		
		bool ParseSimpleOperation(Scanner &scanner)
		{
			if( !BeginSimpleOperation() )
				return false;
			// < Operation > END
			// < Operation >
			if( !ParseOperation(scanner))
				return false;
			// END
			{
				TerminalData terminal = scanner.GetNextSymbol();
				if ( terminal.type == Terminal::END )
				{
					if(! ParseTerminal(terminal) )
						return false;
				}
				else
				{
					ErrorUnexpectedTerminal(terminal, Terminal::END);
					return false;
				}
			}
			if( !EndSimpleOperation() )
				return false;
			return true;
		}
		
	protected:
		
		struct ExpectedTerminalList
		{
			int length;
			Terminal expectedTerminals[Scanner::LLk];
		};
		
		virtual bool BeginAdvancedAddition() { return true; }
		virtual bool EndAdvancedAddition() { return true; }
		
		virtual bool BeginAdvancedMultiplication() { return true; }
		virtual bool EndAdvancedMultiplication() { return true; }
		
		virtual bool BeginAdvancedOperation() { return true; }
		virtual bool EndAdvancedOperation() { return true; }
		
		virtual bool BeginAdvancedParenthesis() { return true; }
		virtual bool EndAdvancedParenthesis() { return true; }
		
		virtual bool BeginOperation() { return true; }
		virtual bool EndOperation() { return true; }
		
		virtual bool BeginParenthesis() { return true; }
		virtual bool EndParenthesis() { return true; }
		
		virtual bool BeginSimpleOperation() { return true; }
		virtual bool EndSimpleOperation() { return true; }
		
		virtual bool ParseTerminal(TerminalData const &terminalData) { return true; }
		
		virtual void ErrorUnexpectedTerminal(TerminalData const &terminalData, Terminal expectedTerminal) { }
		virtual void ErrorUnexpectedTerminal(TerminalData const &terminalData, Terminal const *expectedTerminals, size_t expectedTerminalCount) { }
		
	};
	// <<< [ NUMBER ][ PARENTHESIS_OPEN ] >>>
	// AdvancedAddition = <AdvancedParenthesis>, <AdvancedAddition_SUB_1>;
	
	// <<< [ PLUS ] >>>
	// AdvancedAddition_SUB_1 = PLUS, <AdvancedParenthesis>, <AdvancedAddition_SUB_1>;
	// <<< [ END ][ MULT ][ PARENTHESIS_CLOSE ] >>>
	// AdvancedAddition_SUB_1 = [];
	
	// <<< [ NUMBER ][ PARENTHESIS_OPEN ] >>>
	// AdvancedMultiplication = <AdvancedAddition>, <AdvancedMultiplication_SUB_1>;
	
	// <<< [ MULT ] >>>
	// AdvancedMultiplication_SUB_1 = MULT, <AdvancedAddition>, <AdvancedMultiplication_SUB_1>;
	// <<< [ END ][ PARENTHESIS_CLOSE ] >>>
	// AdvancedMultiplication_SUB_1 = [];
	
	// <<< [ NUMBER ][ PARENTHESIS_OPEN ] >>>
	// AdvancedOperation = <AdvancedMultiplication>, END;
	
	// <<< [ NUMBER ] >>>
	// AdvancedParenthesis = NUMBER;
	// <<< [ PARENTHESIS_OPEN ] >>>
	// AdvancedParenthesis = <AdvancedParenthesis_SUB_1>;
	
	// <<< [ PARENTHESIS_OPEN ] >>>
	// AdvancedParenthesis_SUB_1 = PARENTHESIS_OPEN, <AdvancedMultiplication>, PARENTHESIS_CLOSE;
	
	// <<< [ NUMBER ][ PARENTHESIS_OPEN ] >>>
	// Operation = <Operation_SUB_1>, <Operation_SUB_2>;
	
	// <<< [ PARENTHESIS_OPEN ] >>>
	// Operation_SUB_1 = <Parenthesis>;
	// <<< [ NUMBER ] >>>
	// Operation_SUB_1 = NUMBER;
	
	// <<< [ MULT ][ PLUS ] >>>
	// Operation_SUB_2 = <Operation_SUB_3>, <Operation_SUB_4>, <Operation_SUB_2>;
	// <<< [ END ][ PARENTHESIS_CLOSE ] >>>
	// Operation_SUB_2 = [];
	
	// <<< [ PLUS ] >>>
	// Operation_SUB_3 = PLUS;
	// <<< [ MULT ] >>>
	// Operation_SUB_3 = MULT;
	
	// <<< [ PARENTHESIS_OPEN ] >>>
	// Operation_SUB_4 = <Parenthesis>;
	// <<< [ NUMBER ] >>>
	// Operation_SUB_4 = NUMBER;
	
	// <<< [ PARENTHESIS_OPEN ] >>>
	// Parenthesis = PARENTHESIS_OPEN, <Operation>, PARENTHESIS_CLOSE;
	
	// <<< [ NUMBER ][ PARENTHESIS_OPEN ] >>>
	// SimpleOperation = <Operation>, END;
	
	
}
