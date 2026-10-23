#ifndef PARSER_HPP
#define PARSER_HPP

#include "Client.hpp"

class Parser
{
private:

public:
    Parser(/* args */);
    ~Parser();

    static vector<int> registerParser(Client* cli, vector<string>& lines, const string& pass, size_t& idx);
    static void commandParser(Client* cli, vector<string>& lines);
    static vector<string>split(string& str, char delimiter);

};


#endif
