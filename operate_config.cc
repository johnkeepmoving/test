/*
 * =====================================================================================
 *
 *       Filename:  bench.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/2014 04:50:33 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "operate_config.h"

bool IsSpace(char c)
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

bool IsCommentChar(char c)
{
    switch(c) {
    case COMMENT_CHAR:
        return true;
    default:
        return false;
    }
}

void Trim(string & str)
{
    if (str.empty()) {
        return;
    }
    int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) {
        str = "";
        return;
    }
    
    start_pos = i;
    
    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool operate_config::getValue(std::map<string, string> &config)
{
    string line;
    string new_line;
	while (!infile.eof() && getline(infile, line) ) 
	{
		if (line.empty())
		{
            continue;
		}
		int start_pos = 0, end_pos = line.size() - 1, pos;
		if ((pos = line.find(COMMENT_CHAR)) != -1) 
		{
			if (0 == pos) 
			{  
                continue;
			}
            end_pos = pos - 1;
        }
        new_line = line.substr(start_pos, start_pos + 1 - end_pos);  
        if ((pos = new_line.find('=')) == -1)
        {
            std::cout << "error in bench conf:" << line << std::endl; 
			return false;  // no =
		}
        string key=new_line.substr(0, pos);
		Trim(key);
        string value=new_line.substr(pos + 1, end_pos + 1- (pos + 1));
		Trim(value);
        
        config[key] = value; 
    }
    return true;
}

operate_config::operate_config(string filename)
        :infile(filename.c_str())
{
    if (!infile) 
	{
        std::cout << "can't open bench conf file: " << filename << std::endl;
        exit(1);
    }
}

operate_config::~operate_config()
{
    infile.close();
}

/*
int main(int argc, const char *argv[])
{
    operate_config oc("tmp");
    std::map<string, string> config;
    oc.getValue(config);
    
    std::map<string, string>:: iterator i;
    for(i = config.begin(); i!=config.end(); i++)
        std::cout << i->first << " " << i->second << std::endl;
    return 0;
}
*/
