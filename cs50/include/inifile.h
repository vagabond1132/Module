/**
 *Unit Name: inifile.h
 *Current Version: 1.0
 *Description:
 *  读写INI文件封装类
 *Author:
 *Copyright:
 *   Copyright(C) 2011.02
 *History:
 *TODO:
 **/
//---------------------------------------------------------------------------
#ifndef _inifile_H
#define _inifile_H

#include "define.h"
#include <algorithm>
#include <vector>
#include <map>

//---------------------------------------------------------------------------

#define MAX_FILEPATH            255         //INI文件最大路径

using namespace std;

typedef map<string, string, less<string> >strMap;
typedef strMap::iterator strMapIt;

const char* const MIDDLESTRING = "____****____";

//去掉字符串首尾的空格
static string strTrim(string aStr)
{
    string s = aStr;
    unsigned int first = s.find_first_not_of(' ');
    unsigned int last = s.find_last_not_of(' ');
    return s.substr(first, last + 1);
}

//字符串转为大写
static char *strUpr(char *str)
{
    char *s;
    if (str)
    {
        for (s = str; *s; ++s)
        {
            *s = toupper(*s);
        }
    }
    return str;
}

struct IniAnalyze
{
    strMap *pMap;
    string strSect;
    bool bSens;
    unsigned int first, last;

    IniAnalyze(strMap& strMap, string strBool):pMap(&strMap)
    {
        bSens = strBool == "1" ? true: false;
    };

    //重载括号()
    void operator()(const string &sIni)
    {
        //是Section?
        first = sIni.find('[');
        last = sIni.rfind(']');
        if( first != string::npos && last != string::npos && first != last + 1)
        {
            strSect = strTrim(sIni.substr(first + 1, last - first - 1));
            return ;
        }

        if (strSect.empty())
        {
            return ;
        }

        //是Key?
        if((first = sIni.find('=')) == string::npos)
        {
            return ;
        }

        string strtmp1 = sIni.substr(0, first);
        string strtmp2 = sIni.substr(first + 1, string::npos);

        first= strtmp1.find_first_not_of(" \t");
        last = strtmp1.find_last_not_of(" \t");

        if(first == string::npos || last == string::npos)
        {
            return ;
        }

        string strkey = strTrim(strtmp1.substr(first, last - first + 1));
        first = strtmp2.find_first_not_of(" \t");

        //取值
        if(((last = strtmp2.find("\t#", first)) != string::npos) ||
                ((last = strtmp2.find(" #", first)) != string::npos) ||
                ((last = strtmp2.find("\t;",first)) != string::npos) ||
                ((last = strtmp2.find(" ;", first)) != string::npos) ||
                ((last = strtmp2.find("\t//", first)) != string::npos)||
                ((last = strtmp2.find(" //",  first)) != string::npos))
        {
            strtmp2 = strtmp2.substr(0, last - first);
        }

        last = strtmp2.find_last_not_of(" \t");
        if(first == string::npos || last == string::npos)
        {
            return ;
        }

        string value = strtmp2.substr(first, last - first + 1);
        string mapkey = strSect + MIDDLESTRING + strkey;
        if (!bSens)
        {
            char *tmp = strUpr((char *)mapkey.c_str());
            mapkey = tmp;
        }

        (*pMap)[mapkey] = value;
    }
};
//---------------------------------------------------------------------------

//读写INI文件封装类
class IniFile
{
    public:
        //默认Section、Key大小写敏感
        IniFile(const char* pIniFile, bool bCaseSensitive = true)
        {
            bSensitive = bCaseSensitive;

            memset(szIniFile, 0, MAX_FILEPATH);
            int iLen = (strlen(pIniFile) > MAX_FILEPATH)? MAX_FILEPATH: strlen(pIniFile);
            memcpy(szIniFile, pIniFile, iLen);

            OpenIni(szIniFile);
        }
        ~IniFile(){};

        //保存INI文件
        bool Update()
        {
            return WriteIni(szIniFile);
        }

        const char* ReadString(const char* pSect, const char* pKey, const char* defaultValue = "")
        {
            string mapkey = pSect;
            mapkey += MIDDLESTRING;
            mapkey += strTrim(pKey);
            if (!bSensitive)
            {
                char *tmp = strUpr((char *)mapkey.c_str());
                mapkey = tmp;
            }

            strMapIt it = iniMap.find(mapkey);
            if(it == iniMap.end())
            {
                return defaultValue;
            }

            return (char *)it->second.c_str();
        };

        void WriteString(const char* pSect, const char* pKey, const char* pValue)
        {
            string mapkey = strTrim(pSect);
            mapkey += MIDDLESTRING;
            mapkey += strTrim(pKey);
            if (!bSensitive)
            {
                char *tmp = strUpr((char *)mapkey.c_str());
                mapkey = tmp;
            }

            strMapIt it = iniMap.find(mapkey);
            if(it != iniMap.end())
            {
                it->second = pValue;
                return ;
            }

            iniMap[mapkey] = pValue;
        };

        int ReadInteger(const char* pSect, const char* pKey, int iValue = 0)
        {
            const char *pValue = ReadString(pSect, pKey, "0");
            if (0 == strlen(pValue))
            {
                return iValue;
            }
            return atoi(pValue);
        }

        void WriteInteger(const char* pSect, const char* pKey, int iValue)
        {
            char szValue[25] = {0};
            sprintf(szValue, "%d", iValue);
            WriteString(pSect, pKey, szValue);
        }

        double ReadDouble(const char* pSect, const char* pKey, double fValue = 0)
        {
            const char *pValue = ReadString(pSect, pKey, "0");
            if (0 == strlen(pValue))
            {
                return fValue;
            }
            return atof(pValue);
        }

        void WriteDouble(const char* pSect, const char* pKey, double fValue)
        {
            char szValue[25] = {0};
            sprintf(szValue, "%f", fValue);
            WriteString(pSect, pKey, szValue);
        }

        bool ReadBool(const char* pSect, const char* pKey, bool bValue = false)
        {
            const char *pValue = ReadString(pSect, pKey, "0");
            if (0 == strlen(pValue))
            {
                return bValue;
            }
            return strcmp(pValue, "1") == 0? true : false;
        }

        void WriteBool(const char* pSect, const char* pKey, bool bValue)
        {
            char szValue[2] = {0};
            szValue[0] = '0';
            if (bValue) szValue[0] = '1';
            WriteString(pSect, pKey, szValue);
        }

        //删除某Session下的某键值
        void DeleteKey(const char* pSect, const char* pKey)
        {
            string mapkey = strTrim(pSect);
            mapkey += MIDDLESTRING;
            mapkey += strTrim(pKey);
            if (!bSensitive)
            {
                char *tmp = strUpr((char *)mapkey.c_str());
                mapkey = tmp;
            }

            strMapIt it = iniMap.find(mapkey);
            if(it != iniMap.end())
            {
                iniMap.erase(it);
            }
        }

        //删除某Session下的所有条目
        void DeleteSession(const char* pSect)
        {
            string strDest = strTrim(pSect);
            if (!bSensitive)
            {
                char *tmp = strUpr((char *)strDest.c_str());
                strDest = tmp;
            }

            strMapIt it;
            string sSess, strSect;
            unsigned int uPos = 0;
            for (it = iniMap.begin(); it != iniMap.end(); ++it)
            {
                strSect = it->first;
                uPos = strSect.find(MIDDLESTRING);
                sSess = strTrim(strSect.substr(0, uPos));
                if (sSess == strDest)
                {
                    iniMap.erase(it);
                }
            }
        }

        //清空INI所有内容
        void Clear()
        {
            iniMap.clear();
        }

    private:
        bool OpenIni(const char* pIniFile)
        {
            ifstream fin(pIniFile);
            if(!fin.is_open())
            {
                fprintf(stderr, "Cannot open file: %s!\n", pIniFile);
                return false;
            }

            vector<string> strVect;

            string strLine;
            unsigned int last;
            while(getline(fin, strLine, '\n').good())
            {
                if ((last = strLine.find_last_not_of('\r')) != string::npos)
                {
                    strLine = strLine.substr(0, last + 1);
                }
                strVect.push_back(strLine);
            }

            fin.close();

            if(strVect.empty())
            {
                return false;
            }

            string strBool = bSensitive? "1": "0";
            for_each(strVect.begin(), strVect.end(), IniAnalyze(iniMap, strBool));

            return !iniMap.empty();
        }

        bool WriteIni(char* pIniFile)
        {
            ofstream fout(pIniFile);
            if (!fout.is_open())
            {
                return false;
            }

            strMapIt it;
            int iMID_LEN = strlen(MIDDLESTRING);
            string sSessSave = "", sSess, sKey, strSect;
            unsigned int uPos;
            for (it = iniMap.begin(); it != iniMap.end(); ++it)
            {
                strSect = it->first;
                uPos = strSect.find(MIDDLESTRING);
                sSess = strSect.substr(0, uPos);
                if (sSessSave != sSess)
                {
                    if (sSessSave != "")
                    {
                        fout << endl;       //Section之间留一空行
                    }
                    sSessSave = sSess;
                    fout << "[" << sSess << "]" << endl;
                }

                sKey = strSect.substr(uPos + iMID_LEN, strlen(strSect.c_str()) - uPos - iMID_LEN);
                fout << sKey << "=" << it->second << endl;
            }

            fout.close();

            return true;
        }

        char szIniFile[MAX_FILEPATH];
        bool bSensitive;
        strMap iniMap;
};
//---------------------------------------------------------------------------

#endif
