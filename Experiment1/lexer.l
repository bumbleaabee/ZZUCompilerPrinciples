%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 单词符号种别码定义
#define KEYWORD_MAIN       100
#define KEYWORD_IF         101
#define KEYWORD_THEN       102
#define KEYWORD_ELSE       103
#define KEYWORD_WHILE      104
#define KEYWORD_DO         105
#define KEYWORD_REPEAT     106
#define KEYWORD_UNTIL      107
#define KEYWORD_FOR        108
#define KEYWORD_FROM       109
#define KEYWORD_TO         110
#define KEYWORD_STEP       111
#define KEYWORD_SWITCH     112
#define KEYWORD_OF         113
#define KEYWORD_CASE       114
#define KEYWORD_DEFAULT    115
#define KEYWORD_RETURN     116
#define KEYWORD_INTEGER    117
#define KEYWORD_REAL       118
#define KEYWORD_CHAR       119
#define KEYWORD_BOOL       120
#define KEYWORD_AND        121
#define KEYWORD_OR         122
#define KEYWORD_NOT        123
#define KEYWORD_MOD        124
#define KEYWORD_READ       125
#define KEYWORD_WRITE      126

#define OPERATOR_ASSIGN    200  // =
#define OPERATOR_PLUS      201  // +
#define OPERATOR_MINUS     202  // -
#define OPERATOR_MULTIPLY  203  // *
#define OPERATOR_DIVIDE    204  // /
#define OPERATOR_LT        205  // <
#define OPERATOR_LE        206  // <=
#define OPERATOR_GT        207  // >
#define OPERATOR_GE        208  // >=
#define OPERATOR_NE        209  // !=

#define SEPARATOR_COMMA    300  // ,
#define SEPARATOR_SEMICOLON 301 // ;
#define SEPARATOR_COLON    302  // :
#define SEPARATOR_LBRACE   303  // {
#define SEPARATOR_RBRACE   304  // }
#define SEPARATOR_LBRACKET 305  // [
#define SEPARATOR_RBRACKET 306  // ]
#define SEPARATOR_LPAREN   307  // (
#define SEPARATOR_RPAREN   308  // )

#define IDENTIFIER         400
#define NUMBER             500

int line_num = 1;
%}

/* 正则表达式定义 */
DIGIT       [0-9]
LETTER      [a-zA-Z]
ID          {LETTER}({LETTER}|{DIGIT})*
NUM         {DIGIT}+
WHITESPACE  [ \t]
NEWLINE     \n

%%

"main"      { printf("(%d, KEYWORD_MAIN, %s)\n", KEYWORD_MAIN, yytext); }
"if"        { printf("(%d, KEYWORD_IF, %s)\n", KEYWORD_IF, yytext); }
"then"      { printf("(%d, KEYWORD_THEN, %s)\n", KEYWORD_THEN, yytext); }
"else"      { printf("(%d, KEYWORD_ELSE, %s)\n", KEYWORD_ELSE, yytext); }
"while"     { printf("(%d, KEYWORD_WHILE, %s)\n", KEYWORD_WHILE, yytext); }
"do"        { printf("(%d, KEYWORD_DO, %s)\n", KEYWORD_DO, yytext); }
"repeat"    { printf("(%d, KEYWORD_REPEAT, %s)\n", KEYWORD_REPEAT, yytext); }
"until"     { printf("(%d, KEYWORD_UNTIL, %s)\n", KEYWORD_UNTIL, yytext); }
"for"       { printf("(%d, KEYWORD_FOR, %s)\n", KEYWORD_FOR, yytext); }
"from"      { printf("(%d, KEYWORD_FROM, %s)\n", KEYWORD_FROM, yytext); }
"to"        { printf("(%d, KEYWORD_TO, %s)\n", KEYWORD_TO, yytext); }
"step"      { printf("(%d, KEYWORD_STEP, %s)\n", KEYWORD_STEP, yytext); }
"switch"    { printf("(%d, KEYWORD_SWITCH, %s)\n", KEYWORD_SWITCH, yytext); }
"of"        { printf("(%d, KEYWORD_OF, %s)\n", KEYWORD_OF, yytext); }
"case"      { printf("(%d, KEYWORD_CASE, %s)\n", KEYWORD_CASE, yytext); }
"default"   { printf("(%d, KEYWORD_DEFAULT, %s)\n", KEYWORD_DEFAULT, yytext); }
"return"    { printf("(%d, KEYWORD_RETURN, %s)\n", KEYWORD_RETURN, yytext); }
"integer"   { printf("(%d, KEYWORD_INTEGER, %s)\n", KEYWORD_INTEGER, yytext); }
"real"      { printf("(%d, KEYWORD_REAL, %s)\n", KEYWORD_REAL, yytext); }
"char"      { printf("(%d, KEYWORD_CHAR, %s)\n", KEYWORD_CHAR, yytext); }
"bool"      { printf("(%d, KEYWORD_BOOL, %s)\n", KEYWORD_BOOL, yytext); }
"and"       { printf("(%d, KEYWORD_AND, %s)\n", KEYWORD_AND, yytext); }
"or"        { printf("(%d, KEYWORD_OR, %s)\n", KEYWORD_OR, yytext); }
"not"       { printf("(%d, KEYWORD_NOT, %s)\n", KEYWORD_NOT, yytext); }
"mod"       { printf("(%d, KEYWORD_MOD, %s)\n", KEYWORD_MOD, yytext); }
"read"      { printf("(%d, KEYWORD_READ, %s)\n", KEYWORD_READ, yytext); }
"write"     { printf("(%d, KEYWORD_WRITE, %s)\n", KEYWORD_WRITE, yytext); }

"="         { printf("(%d, OPERATOR_ASSIGN, %s)\n", OPERATOR_ASSIGN, yytext); }
"+"         { printf("(%d, OPERATOR_PLUS, %s)\n", OPERATOR_PLUS, yytext); }
"-"         { printf("(%d, OPERATOR_MINUS, %s)\n", OPERATOR_MINUS, yytext); }
"*"         { printf("(%d, OPERATOR_MULTIPLY, %s)\n", OPERATOR_MULTIPLY, yytext); }
"/"         { printf("(%d, OPERATOR_DIVIDE, %s)\n", OPERATOR_DIVIDE, yytext); }
"<"         { printf("(%d, OPERATOR_LT, %s)\n", OPERATOR_LT, yytext); }
"<="        { printf("(%d, OPERATOR_LE, %s)\n", OPERATOR_LE, yytext); }
">"         { printf("(%d, OPERATOR_GT, %s)\n", OPERATOR_GT, yytext); }
">="        { printf("(%d, OPERATOR_GE, %s)\n", OPERATOR_GE, yytext); }
"!="        { printf("(%d, OPERATOR_NE, %s)\n", OPERATOR_NE, yytext); }

","         { printf("(%d, SEPARATOR_COMMA, %s)\n", SEPARATOR_COMMA, yytext); }
";"         { printf("(%d, SEPARATOR_SEMICOLON, %s)\n", SEPARATOR_SEMICOLON, yytext); }
":"         { printf("(%d, SEPARATOR_COLON, %s)\n", SEPARATOR_COLON, yytext); }
"{"         { printf("(%d, SEPARATOR_LBRACE, %s)\n", SEPARATOR_LBRACE, yytext); }
"}"         { printf("(%d, SEPARATOR_RBRACE, %s)\n", SEPARATOR_RBRACE, yytext); }
"["         { printf("(%d, SEPARATOR_LBRACKET, %s)\n", SEPARATOR_LBRACKET, yytext); }
"]"         { printf("(%d, SEPARATOR_RBRACKET, %s)\n", SEPARATOR_RBRACKET, yytext); }
"("         { printf("(%d, SEPARATOR_LPAREN, %s)\n", SEPARATOR_LPAREN, yytext); }
")"         { printf("(%d, SEPARATOR_RPAREN, %s)\n", SEPARATOR_RPAREN, yytext); }

{ID}        { printf("(%d, IDENTIFIER, %s)\n", IDENTIFIER, yytext); }
{NUM}       { printf("(%d, NUMBER, %s)\n", NUMBER, yytext); }

{WHITESPACE} { /* 忽略空白字符和制表符 */ }
{NEWLINE}    { line_num++; /* 换行时增加行号 */ }
.           { printf("Error: Invalid character '%s' at line %d\n", yytext, line_num); }

%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    // 设置输入文件
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Error: Cannot open file %s\n", argv[1]);
        return 1;
    }

    printf("=== Lexical Analysis Result ===\n");
    printf("(Token_Type, Description, Lexeme)\n");
    printf("--------------------------------\n");

    // 开始词法分析
    yylex();

    fclose(yyin);
    printf("--------------------------------\n");
    printf("Lexical analysis completed. Total lines: %d\n", line_num);
    return 0;
}

int yywrap() {
    return 1;
}