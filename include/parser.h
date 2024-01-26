#ifndef PARSER_H_
#define PARSER_H_

typedef struct ParserStruct {
	const char *token;
	const char *token_type;

} Parser;

Parser* initParser(const char* token, const char* token_type);

Parser* advanceParser(Parser* parser);

#endif