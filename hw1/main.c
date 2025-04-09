#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_LEN 256  // 定義 ID 或 token 值的最大長度

// 定義 token 類型
enum TokenType {
    ID,             // 標識符
    INTLITERAL,     // 整數
    PLUS,           // +
    MINUS,          // -
    STAR,           // *
    SLASH,          // /
    EQUAL,          // =
    EQUAL_EQUAL,    // ==
    BANG_EQUAL,     // !=
    LESS,           // <
    GREATER,        // >
    LESS_EQUAL,     // <=
    GREATER_EQUAL,  // >=
    SEMICOLON,      // ;
    COMMA,          // ,
    LPAREN,         // (
    RPAREN,         // )
    LBRACE,         // {
    RBRACE,         // }
    LBRACKET,       // [
    RBRACKET,       // ]
    KEYWORD_INT,    // int
    KEYWORD_IF,     // if
    KEYWORD_ELSE,   // else
    KEYWORD_WHILE,  // while
    EOF_TOKEN       // 文件結束
};

// token 類型對應的字符串，用於輸出
const char *tokenTypeStrings[] = {
    "ID", "INTLITERAL", "PLUS", "MINUS", "STAR", "SLASH", "EQUAL",
    "EQUAL_EQUAL", "BANG_EQUAL", "LESS", "GREATER", "LESS_EQUAL",
    "GREATER_EQUAL", "SEMICOLON", "COMMA", "LPAREN", "RPAREN",
    "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "KEYWORD_INT",
    "KEYWORD_IF", "KEYWORD_ELSE", "KEYWORD_WHILE", "EOF"
};

// 函數聲明：獲取下一個 token
int getToken(char *tokenValue, int *tokenType);

int main(void) {
    char tokenValue[MAX_ID_LEN];  // 儲存 token 的值
    int tokenType;                // 儲存 token 的類型
    // 持續獲取 token 直到遇到 EOF
    while (getToken(tokenValue, &tokenType) != EOF_TOKEN) {
        printf("%s: %s\n", tokenTypeStrings[tokenType], tokenValue);
    }
    return 0;
}

// 實現 getToken 函數，負責詞法分析
int getToken(char *tokenValue, int *tokenType) {
    int ch;

    // 跳過空白字元
    while ((ch = getchar()) != EOF && isspace(ch)) {
        // 什麼也不做
    }

    // 檢查是否到達文件末尾
    if (ch == EOF) {
        *tokenType = EOF_TOKEN;
        return EOF_TOKEN;
    }

    // 處理 ID 或關鍵字
    if (isalpha(ch) || ch == '_') {
        int i = 0;
        tokenValue[i++] = ch;
        // 繼續讀取字母、數字或下劃線
        while ((ch = getchar()) != EOF && (isalnum(ch) || ch == '_')) {
            if (i < MAX_ID_LEN - 1) {
                tokenValue[i++] = ch;
            }
        }
        tokenValue[i] = '\0';
        ungetc(ch, stdin);  // 將非 ID 字元放回輸入流

        // 檢查是否為關鍵字
        if (strcmp(tokenValue, "int") == 0) {
            *tokenType = KEYWORD_INT;
        } else if (strcmp(tokenValue, "if") == 0) {
            *tokenType = KEYWORD_IF;
        } else if (strcmp(tokenValue, "else") == 0) {
            *tokenType = KEYWORD_ELSE;
        } else if (strcmp(tokenValue, "while") == 0) {
            *tokenType = KEYWORD_WHILE;
        } else {
            *tokenType = ID;
        }
        return *tokenType;
    }
    // 處理整數
    else if (isdigit(ch)) {
        int i = 0;
        tokenValue[i++] = ch;
        // 繼續讀取數字
        while ((ch = getchar()) != EOF && isdigit(ch)) {
            if (i < MAX_ID_LEN - 1) {
                tokenValue[i++] = ch;
            }
        }
        tokenValue[i] = '\0';
        ungetc(ch, stdin);  // 將非數字字元放回輸入流
        *tokenType = INTLITERAL;
        return *tokenType;
    }
    // 處理運算符和標點符號
    else {
        tokenValue[0] = ch;
        tokenValue[1] = '\0';
        switch (ch) {
            case '+': *tokenType = PLUS; break;
            case '-': *tokenType = MINUS; break;
            case '*': *tokenType = STAR; break;
            case '/': *tokenType = SLASH; break;
            case '=':
                ch = getchar();
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = EQUAL_EQUAL;
                } else {
                    ungetc(ch, stdin);
                    *tokenType = EQUAL;
                }
                break;
            case '!':
                ch = getchar();
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = BANG_EQUAL;
                } else {
                    ungetc(ch, stdin);
                    printf("Lexical error: unexpected character '!'.\n");
                    return -1;
                }
                break;
            case '<':
                ch = getchar();
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = LESS_EQUAL;
                } else {
                    ungetc(ch, stdin);
                    *tokenType = LESS;
                }
                break;
            case '>':
                ch = getchar();
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = GREATER_EQUAL;
                } else {
                    ungetc(ch, stdin);
                    *tokenType = GREATER;
                }
                break;
            case ';': *tokenType = SEMICOLON; break;
            case ',': *tokenType = COMMA; break;
            case '(': *tokenType = LPAREN; break;
            case ')': *tokenType = RPAREN; break;
            case '{': *tokenType = LBRACE; break;
            case '}': *tokenType = RBRACE; break;
            case '[': *tokenType = LBRACKET; break;
            case ']': *tokenType = RBRACKET; break;
            default:
                printf("Lexical error: unexpected character '%c'.\n", ch);
                return -1;
        }
        return *tokenType;
    }
}
