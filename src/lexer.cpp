#include "lexer.hpp"

using namespace std;

vector<Ptr<Token>> tokenize(const string & text) {
    string::const_iterator it = text.cbegin();
    string::const_iterator end = text.cend();
    vector<Ptr<Token>> tokens;
    tokens.reserve(text.size() / 2);
    while (true) {
        while (it != end && isspace(*it)) ++it;
        if (it == end) {
            tokens.push_back(make_unique<End_of_file>());
            break;
        }
        if (*it == ',') {
            tokens.push_back(make_unique<P_comma>());
            ++it;
        } else if (*it == ';') {
            tokens.push_back(make_unique<P_semicolon>());
            ++it;
        } else if (*it == '(') {
            tokens.push_back(make_unique<P_lparen>());
            ++it;
        } else if (*it == ')') {
            tokens.push_back(make_unique<P_rparen>());
            ++it;
        } else if (*it == '{') {
            tokens.push_back(make_unique<P_lbracket>());
            ++it;
        } else if (*it == '}') {
            tokens.push_back(make_unique<P_rbracket>());
            ++it;
        } else if (*it == '=') {
            tokens.push_back(make_unique<P_assign>());
            ++it;
        } else if (*it == '+') {
            tokens.push_back(make_unique<P_plus>());
            ++it;
        } else if (*it == '*') {
            tokens.push_back(make_unique<P_multiply>());
            ++it;
        } else if (*it == '/') {
            tokens.push_back(make_unique<P_divide>());
            ++it;
        } else if (*it == '-') { // P_minus, int_literal, float_literal
            string str = "-";
            ++it;
            if (it == end || !isdigit(*it))
                tokens.push_back(make_unique<P_minus>());
            else {
                str += *it;
                ++it;
                while (it != end && isdigit(*it)) {
                    str += *it;
                    ++it;
                }
                if (it != end && *it == '.') {
                    str += *it;
                    ++it;
                    while (it != end && isdigit(*it)) {
                        str += *it;
                        ++it;
                    }
                    tokens.push_back(make_unique<float_literal>(move(str)));
                } else {
                    tokens.push_back(make_unique<int_literal>(move(str)));
                }
            }
        } else if (('0' <= *it) && (*it <= '9')) {
            string str;
            str += *it;
            ++it;
            while (it != end && isdigit(*it)) {
                str += *it;
                ++it;
            }
            if (it != end && *it == '.') {
                str += *it;
                ++it;
                while (it != end && isdigit(*it)) {
                    str += *it;
                    ++it;
                }
                tokens.push_back(make_unique<float_literal>(move(str)));
            } else {
                tokens.push_back(make_unique<int_literal>(move(str)));
            }
        } else if ((('a' <= *it) && (*it <= 'z'))
                || (('A' <= *it) && (*it <= 'Z'))
                || ('_' == *it)) { 
            string str;
            str += *it;
            ++it;
            while (it != end && ((('a' <= *it) && (*it <= 'z'))
                        || (('A' <= *it) && (*it <= 'Z')) || ('_' == *it))) {
                str += *it;
                ++it;
            }
            if (str == "int") {
                tokens.push_back(make_unique<Kw_int>());
            } else if (str == "float") {
                tokens.push_back(make_unique<Kw_float>());
            } else if (str == "for") {
                tokens.push_back(make_unique<Kw_for>());
            } else if (str == "while") {
                tokens.push_back(make_unique<Kw_while>());
            } else if (str == "if") {
                tokens.push_back(make_unique<Kw_if>());
            } else if (str == "return") {
                tokens.push_back(make_unique<Kw_return>());
            } else {
                tokens.push_back(make_unique<name_id>(move(str)));
            }
        }
    }
    return tokens;
}
