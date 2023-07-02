#include <iostream>
#include <string>
#include <stack>

bool isWhiteSpace(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

bool isNumeric(char ch) {
    return (ch >= '0' && ch <= '9');
}

bool isQuote(char ch) {
    return (ch == '\"');
}

enum class TokenType {
    LBRACE, // {
    RBRACE, // }
    LBRACKET, // [
    RBRACKET, // ]
    COLON, // :
    COMMA, // ,
    STRING,
    NUMBER,
    BOOLEAN,
    NULLVAL,
    INVALID
};

struct Token {
    TokenType type;
    std::string value;
};

bool isValidNumber(const std::string& str) {
    if (str.empty()) return false;
    if (str[0] == '0' && str.length() > 1) return false; // leading zeros not allowed

    bool hasDecimal = false;
    for (char ch : str) {
        if (!isNumeric(ch)) {
            if (ch == '.') {
                if (hasDecimal) return false; // multiple decimal points
                hasDecimal = true;
            } else {
                return false;
            }
        }
    }

    return true;
}

bool isValidBoolean(const std::string& str) {
    return (str == "true" || str == "false");
}

bool isValidNull(const std::string& str) {
    return (str == "null");
}

Token getNextToken(const std::string& json, size_t& pos) {
    Token token;
    token.type = TokenType::INVALID;

    while (pos < json.length() && isWhiteSpace(json[pos])) {
        pos++;
    }

    if (pos == json.length()) {
        token.type = TokenType::INVALID;
        return token;
    }

    char ch = json[pos];
    pos++;

    switch (ch) {
        case '{':
            token.type = TokenType::LBRACE;
            break;
        case '}':
            token.type = TokenType::RBRACE;
            break;
        case '[':
            token.type = TokenType::LBRACKET;
            break;
        case ']':
            token.type = TokenType::RBRACKET;
            break;
        case ':':
            token.type = TokenType::COLON;
            break;
        case ',':
            token.type = TokenType::COMMA;
            break;
        case '\"': {
            size_t start = pos;
            while (pos < json.length() && !isQuote(json[pos])) {
                pos++;
            }

            if (pos == json.length()) {
                token.type = TokenType::INVALID;
                return token;
            }

            token.type = TokenType::STRING;
            token.value = json.substr(start, pos - start);
            pos++;
            break;
        }
        default: {
            size_t start = pos - 1;
            while (pos < json.length() && !isWhiteSpace(json[pos]) && json[pos] != ',' && json[pos] != '}' && json[pos] != ']') {
                pos++;
            }

            std::string value = json.substr(start, pos - start);
            if (isValidNumber(value)) {
                token.type = TokenType::NUMBER;
                token.value = value;
            } else if (isValidBoolean(value)) {
                token.type = TokenType::BOOLEAN;
                token.value = value;
            } else if (isValidNull(value)) {
                token.type = TokenType::NULLVAL;
                token.value = value;
            } else {
                token.type = TokenType::INVALID;
            }
            break;
        }
    }

    return token;
}

bool isValidJSON(const std::string& json) {
    std::stack<TokenType> stack;

    size_t pos = 0;
    while (pos < json.length()) {
        Token token = getNextToken(json, pos);

        if (token.type == TokenType::INVALID) {
            return false;
        }

        if (token.type == TokenType::LBRACE || token.type == TokenType::LBRACKET) {
            stack.push(token.type);
        } else if (token.type == TokenType::RBRACE || token.type == TokenType::RBRACKET) {
            if (stack.empty() || (stack.top() != TokenType::LBRACE && stack.top() != TokenType::LBRACKET)) {
                return false;
            }
            stack.pop();
        }
    }

    return stack.empty();
}

int main() {
    //std::string json = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";
    std::string json = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\",\"hobbies\":[\"reading\",\"traveling\",\"cooking\"],\"address\":{\"street\":\"123 Main St\",\"city\":\"New York\",\"country\":\"USA\"}}";


    if (isValidJSON(json)) {
        std::cout << "Valid JSON" << std::endl;
    } else {
        std::cout << "Invalid JSON" << std::endl;
    }

    return 0;
}


