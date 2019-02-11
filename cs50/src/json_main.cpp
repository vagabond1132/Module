#include<iostream>
#include "../include/json.h"
#include <exception>


int main()
{
    try{
        std::string input = "{\"session\":\"1234567\", \"key\":\"9712314\"}";
        json::Value value = json::Deserialize(input); 
        std::cout << value["session"].ToString() << "     " << value["key"].ToString() << std::endl; 
    }catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    

    return 0;
}
