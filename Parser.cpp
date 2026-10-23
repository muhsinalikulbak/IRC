#include "Parser.hpp"


// Bazı hata kodlarını eklenecek
vector<int> Parser::registerParser(Client* cli, vector<string>& lines, const string& pass, size_t& idx)
{
    size_t i = 0;
    vector<int> codes;

    for (; i < lines.size(); i++)
    {
        if (cli->isRegistered())
            break;
        
        vector<string> line = split(lines[i], ' ');
        
        if (line.size() < 2)
            continue;
        
        if (!cli->getPassState())
        {
            if (line[0] != "PASS")
                continue;
            
            if (pass == line[1])
            {
                cli->setPassState(true);
            }
        }
        else if (!cli->getNickState() && line[0] == "NICK")
        {
            cli->setNickState(true);
            cli->setNick(line[1]);
        }
        else if (!cli->getUserState() && line.size() > 3 && line[0] == "USER")
        {
            // Burada real name de eklemem gerekiyor, ve indexler şu an yanlış bunu düzelt
            cli->setUserState(true);
            cli->setUser(line[1]);
        }
    }
    idx = i;
    return codes;
}


void Parser::commandParser(Client* cli, vector<string>& lines)
{

}

vector<string> Parser::split(string& str, char delimiter)
{
    vector<string> words;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) 
    {
        words.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    
    words.push_back(str.substr(start));
    return words;
}
