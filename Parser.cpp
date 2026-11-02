#include "Parser.hpp"
static bool isValidNick(const string& str)
{
    if (str.size() > 9)
        return false;
    
    string allowed = "[]\\`^_{|}";
    char ch = str[0];

    if (!std::isalpha(ch) && allowed.find(ch) == string::npos)
        return false;
    
    // Nick 9 özel karakter '-'  rakam ve harfleri içerir
    // Sadece ilk karakter rakam tire ve 9 özel karakter dışında bir şey olamaz.
    
    for (size_t i = 0; i < str.size(); i++)
    {
        if (std::isalpha(str[i]) || std::isdigit(str[i]) || str[i] == '-')
            continue;
        else if (allowed.find(str[i]) == string::npos)
            return false;
    }
    return true;
}

vector<pair<int, string> > Parser::registerParser(Client* cli, vector<string>& lines, const string& pass, size_t& idx, map<string, Client*>& nickMap)
{
    size_t i = 0;
    vector<pair<int, string> > codes;

    for (; i < lines.size(); i++)
    {
        if (cli->isRegistered())
        {
            // 001 Welcome
            codes.push_back(std::make_pair(1, ""));
            break;
        }
        
        vector<string> line = split(lines[i], ' ');
        
        if (line.size() == 0)
            continue;
        
        if (line[0] == "PASS")
        {
            if (line.size() < 2 || line[1].empty())
            {
                codes.push_back(std::make_pair(461, "PASS"));
                continue;
            }

            if (!cli->getPassState())
            {
                if (pass == line[1])
                    cli->setPassState(true);
                else if (cli->getNickState() && cli->getUserState())
                {
                    codes.push_back(std::make_pair(464, ""));
                    return codes;
                }
            }
        }
        else if (line[0] == "NICK")
        {
            if (line.size() < 2 || line[1].empty())
                codes.push_back(std::make_pair(431, ""));
            else if (!isValidNick(line[1]))
                codes.push_back(std::make_pair(432, line[1]));
            else if (nickMap.find(utils::toLower(line[1])) != nickMap.end())
                codes.push_back(std::make_pair(433, line[1]));
            else
            {
                cli->setNick(line[1]);
                cli->setNickState(true);
            }
        }
        else if (line[0] == "USER")
        {
            if (!cli->getUserState())
            {
                size_t pos = lines[i].find(':');
                if (pos != string::npos)
                {
                    string str = lines[i].substr(0, pos);
                    line = split(str, ' ');
                    line.push_back(lines[i].substr(pos));
                }

                if (line.size() < 5)
                {
                    codes.push_back(std::make_pair(461, "USER"));
                    continue;
                }

                cli->setUser(line[1]);
                cli->setRealName(line[4]);
                
            }
            else
            {
                codes.push_back(std::make_pair(462, "USER"));
            }
        }
        else
            codes.push_back(std::make_pair(451, ""));
    }
    idx = i;
    return codes;
}


void Parser::commandParser(Client* cli, vector<string>& lines)
{
    // Eğer command yerine pass nick  tarzı kayıt verisi gelirse
    // 462 DÖN
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
