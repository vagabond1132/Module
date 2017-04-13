#include "json.h"


#include <iostream>
#include <map>


int main(){
    
    std::string jsonstr = "{ \"ginname\":\"mop_t0_test001\", \"loginpwd\": \"33f2248a543ecc75b1f91142f1b1bd56\", \"ligang\":}";


    json::Object oval = json::Deserialize(jsonstr).ToObject();

    std::cout << "size :  " << oval.size() << std::endl; 
    


    for(json::Object::ValueMap::const_iterator  iter = oval.begin(); iter != oval.end(); ++iter)
        std::cout << " iter.key = " << iter->first << std::endl;

    



    return 0;
}
