#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_LEN 256  // 定義 ID 或 token 值的最大長度

// 定義 token 類型
enum TokenType {
    ID_TOKEN,             // 標識符
    LITERAL_TOKEN,        // 整數
    PLUS_TOKEN,           // +
    MINUS_TOKEN,          // -
    STAR_TOKEN,           // *
    SLASH_TOKEN,          // /
    ASSIGN_TOKEN,         // =
    EQUAL_TOKEN,          // ==
    BANG_EQUAL_TOKEN,     // !=
    LESS_TOKEN,           // <
    GREATER_TOKEN,        // >
    LESSEQUAL_TOKEN,      // <=
    GREATEREQUAL_TOKEN,   // >=
    SEMICOLON_TOKEN,      // ;
    COMMA_TOKEN,          // ,
    LEFTPAREN_TOKEN,      // (
    REFTPAREN_TOKEN,      // )
    LEFTBRACE_TOKEN,      // {
    REFTBRACE_TOKEN,      // }
    LEFTBRACKET_TOKEN,    // [
    REFTBRACKET_TOKEN,    // ]
    TYPE_TOKEN,           // int
    IF_TOKEN,             // if
    ELSE_TOKEN,           // else
    WHILE_TOKEN,          // while
    MAIN_TOKEN,           // main
    EOF_TOKEN             // 文件結束
};

// token 類型對應的字符串，用於輸出
const char *tokenTypeStrings[] = {
    "ID_TOKEN", "LITERAL_TOKEN", "PLUS_TOKEN", "MINUS_TOKEN", "STAR_TOKEN", "SLASH_TOKEN", "ASSIGN_TOKEN",
    "EQUAL_TOKEN", "BANG_EQUAL_TOKEN", "LESS_TOKEN", "GREATER_TOKEN", "LESSEQUAL_TOKEN",
    "GREATEREQUAL_TOKEN", "SEMICOLON_TOKEN", "COMMA_TOKEN", "LEFTPAREN_TOKEN", "REFTPAREN_TOKEN",
    "LEFTBRACE_TOKEN", "REFTBRACE_TOKEN", "LEFTBRACKET_TOKEN", "REFTBRACKET_TOKEN", "TYPE_TOKEN",
    "IF_TOKEN", "ELSE_TOKEN", "WHILE_TOKEN", "MAIN_TOKEN", "EOF_TOKEN"
};

// 函數聲明：獲取下一個 token
int getToken(FILE *file, char *tokenValue, int *tokenType);

// 全局變數：用於模擬 ungetc 的行為
int lastChar = -1;  // 儲存被放回的字元，初始化為 -1 表示無字元

int main(void) {
    char tokenValue[MAX_ID_LEN];  // 儲存 token 的值
    int tokenType;                // 儲存 token 的類型

    // 打開 hw0.c 文件
    FILE *file = fopen("TA.c", "r");
    if (file == NULL) {
        perror("Error opening hw0.c");
        return 1;
    }

    // 持續獲取 token 直到遇到 EOF
    while (getToken(file, tokenValue, &tokenType) != EOF_TOKEN) {
        printf("%s: %s\n",tokenValue,tokenTypeStrings[tokenType]);
    }

    // 關閉文件
    fclose(file);
    return 0;
}

// 從文件中獲取下一個字元，支援模擬 ungetc 的行為
int getNextChar(FILE *file) {
    if (lastChar != -1) {
        int ch = lastChar;
        lastChar = -1;  // 重置 lastChar
        return ch;
    }
    return fgetc(file);
}

// 模擬 ungetc，將字元放回
void putBackChar(int ch) {
    lastChar = ch;
}

// 實現 getToken 函數，負責詞法分析
int getToken(FILE *file, char *tokenValue, int *tokenType) {
    int ch;

    // 跳過空白字元
    while ((ch = getNextChar(file)) != EOF && isspace(ch)) {
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
        while ((ch = getNextChar(file)) != EOF && (isalnum(ch) || ch == '_')) {
            if (i < MAX_ID_LEN - 1) {
                tokenValue[i++] = ch;
            }
        }
        tokenValue[i] = '\0';
        putBackChar(ch);  // 將非 ID 字元放回

        // 檢查是否為關鍵字或 main
        if (strcmp(tokenValue, "int") == 0) {
            *tokenType = TYPE_TOKEN;
        } else if (strcmp(tokenValue, "if") == 0) {
            *tokenType = IF_TOKEN;
        } else if (strcmp(tokenValue, "else") == 0) {
            *tokenType = ELSE_TOKEN;
        } else if (strcmp(tokenValue, "while") == 0) {
            *tokenType = WHILE_TOKEN;
        } else if (strcmp(tokenValue, "main") == 0) {
            *tokenType = MAIN_TOKEN;
        } else {
            *tokenType = ID_TOKEN;
        }
        return *tokenType;
    }
    // 處理整數
    else if (isdigit(ch)) {
        int i = 0;
        tokenValue[i++] = ch;
        // 繼續讀取數字
        while ((ch = getNextChar(file)) != EOF && isdigit(ch)) {
            if (i < MAX_ID_LEN - 1) {
                tokenValue[i++] = ch;
            }
        }
        tokenValue[i] = '\0';
        putBackChar(ch);  // 將非數字字元放回
        *tokenType = LITERAL_TOKEN;
        return *tokenType;
    }
    // 處理運算符和標點符號
    else {
        tokenValue[0] = ch;
        tokenValue[1] = '\0';
        switch (ch) {
            case '+': *tokenType = PLUS_TOKEN; break;
            case '-': *tokenType = MINUS_TOKEN; break;
            case '*': *tokenType = STAR_TOKEN; break;
            case '/': *tokenType = SLASH_TOKEN; break;
            case '=':
                ch = getNextChar(file);
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = EQUAL_TOKEN;
                } else {
                    putBackChar(ch);
                    *tokenType = ASSIGN_TOKEN;
                }
                break;
            case '!':
                ch = getNextChar(file);
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = BANG_EQUAL_TOKEN;
                } else {
                    putBackChar(ch);
                    printf("Lexical error: unexpected character '!'.\n");
                    return -1;
                }
                break;
            case '<':
                ch = getNextChar(file);
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = LESSEQUAL_TOKEN;
                } else {
                    putBackChar(ch);
                    *tokenType = LESS_TOKEN;
                }
                break;
            case '>':
                ch = getNextChar(file);
                if (ch == '=') {
                    tokenValue[1] = '=';
                    tokenValue[2] = '\0';
                    *tokenType = GREATEREQUAL_TOKEN;
                } else {
                    putBackChar(ch);
                    *tokenType = GREATER_TOKEN;
                }
                break;
            case ';': *tokenType = SEMICOLON_TOKEN; break;
            case ',': *tokenType = COMMA_TOKEN; break;
            case '(': *tokenType = LEFTPAREN_TOKEN; break;
            case ')': *tokenType = REFTPAREN_TOKEN; break;
            case '{': *tokenType = LEFTBRACE_TOKEN; break;
            case '}': *tokenType = REFTBRACE_TOKEN; break;
            case '[': *tokenType = LEFTBRACKET_TOKEN; break;
            case ']': *tokenType = REFTBRACKET_TOKEN; break;
            default:
                printf("Lexical error: unexpected character '%c'.\n", ch);
                return -1;
        }
        return *tokenType;
    }
}
