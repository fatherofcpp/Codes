/*
//	Reflect.h C++反射类
*/
#pragma once
#include <map>
#include <iostream>
#include <string>
using namespace std;

typedef void* (*PTRCreateObject)(void);  

class ClassFactory{
private:  
    map<string, PTRCreateObject> m_classMap ;  
    ClassFactory(){}; //构造函数私有化

public:   
    void* getClassByName(string className);  
    void registClass(string name, PTRCreateObject method) ;  
    static ClassFactory& getInstance() ;  
};

class RegisterAction{
public:
    RegisterAction(string className,PTRCreateObject ptrCreateFn){
        ClassFactory::getInstance().registClass(className,ptrCreateFn);
    }
};

#define REGISTER(className)                                             \
    className* objectCreator##className(){                              \
        return new className;                                           \
    }                                                                   \
    RegisterAction g_creatorRegister##className(                        \
        #className,(PTRCreateObject)objectCreator##className);
