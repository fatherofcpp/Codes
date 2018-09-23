#include "stdafx.h"
#include "Reflect.h"

void* ClassFactory::getClassByName(string className){  
    map<string, PTRCreateObject>::const_iterator iter;  
    iter = m_classMap.find(className) ;  
    if ( iter == m_classMap.end() )  
        return NULL ;  
    else  
        return iter->second() ;  
}  

void ClassFactory::registClass(string name, PTRCreateObject method){  
    m_classMap.insert(pair<string, PTRCreateObject>(name, method)) ;  
}  

ClassFactory& ClassFactory::getInstance(){
    static ClassFactory sLo_factory;  
    return sLo_factory ;  
}  