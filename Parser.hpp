#ifndef PARSER_HPP
#define PARSER_HPP

#include "Client.hpp"
#include <cctype>
#include <map>
#include "utils.hpp"

using std::map;

class Parser
{
private:

    static vector<string>split(string& str, char delimiter);
    static bool isValidNick(const string& nick);

public:
    Parser(/* args */);
    ~Parser();

    static vector<pair<int, string> > registerParser(Client* cli, vector<string>& lines, const string& pass, size_t& idx, map<string, Client*>& nickMap);
    static void commandParser(Client* cli, vector<string>& lines);

};


#endif
