//Noncopyable class
#pragma once
#ifndef NONCOPYABLE_HEAD_FILE
#define NONCOPYABLE_HEAD_FILE

class noncopyable
{
protected:  
	noncopyable() {}  
	~noncopyable() {}  
private:  // emphasize the following members are private  
	noncopyable( const noncopyable& );  
	const noncopyable& operator=( const noncopyable& );  

};
#endif  //NONCOPYABLE_HEAD_FILE