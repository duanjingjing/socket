/*************************************************************************
	> File Name: NonCopyable.h
	> Author: canjian
	> Mail:410001089@qq.com 
	> Created Time: Sun 03 Aug 2014 07:37:35 PM PDT
 ************************************************************************/

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class NonCopyable{
public:
	NonCopyable(){}
	~NonCopyable(){}

private:
	NonCopyable(const NonCopyable&);
	void operator=(const NonCopyable&);
};

#endif
