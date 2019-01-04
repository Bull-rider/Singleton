/****************************************
 File Name: Singlenton.cpp
# Author:yxg
# mail:yxi7899@gmail.com
# Created Time: Fri 04 Jan 2019 03:49:47 PM CST
 ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
using std::cout;
using std::endl;
//下面这种方法线程不安全，教科书中用这种方法简单易懂
class Singleton
{
	class AutoRelease
	{
	public:
		AutoRelease()
		{
			cout<<"AutoRelease()"<<endl;
		}
		~AutoRelease()
		{
			if(_pInstance)
			{
				delete _pInstance;
				cout<<"~AutoRelease()"<<endl;
			}
		}
	};
	public:
	static Singleton *getInstance()
	{
		if(_pInstance==NULL)
		{
			_pInstance=new Singleton();
		}
		return _pInstance;
	}
	private:
	Singleton(){cout<<"Singleton()"<<endl;}
	~Singleton(){cout<<"~Singleton()"<<endl;}
	private:
	static Singleton *_pInstance;
	static AutoRelease _ar;
};
Singleton *Singleton::_pInstance=getInstance();
Singleton::AutoRelease Singleton::_ar;
int main()
{
	Singleton *p1=Singleton::getInstance();
	Singleton *p2=Singleton::getInstance();
	cout<<p1<<endl;
	cout<<p2<<endl;
	return 0;
}
//懒汉模式故名思义，不到万不得已就不会去实例化类，也就是说在第一次//用到类实例的时候才会去实例化，所以上边的经典方法被归为懒汉实现
//----------线程安全的懒汉模式--采用加锁的方式实现
class Singleton
{
public:
	static pthread_mutex mutex;
	static Singleton *instance()
	{
		if(p==NULL)
		{
			pthread_mutex_lock(&mutex);
			if(p==NULL)
			{
				p=new Singleton();
			}
			pthread_mutex_unlock(&mutex);
		}
		return p;
	}
private:
	Singleton()
	{
		pthread_mutex_init(&mutex);
	}
private:
	static Singleton *p;
};
pthread_mutex_t Singleton::mutex;
Singleton *Singleton::p=NULL;
//----------------内部静态变量的懒汉模式实现
//在instance函数里定义一个静态的实例，也可以保证拥有唯一实例，在返//回时只需要返回其指针就可以了。推荐这种实现方法，真得非常简单
class Singleton
{
public:
	static pthread_mutex_t mutex;
	static Singleton *instance()
	{
		pthread_mutex_lock(&mutex);
		static Singleton obj;
		pthread_mutex_unlock(&mutex);
		return &obj;
	}
private:
	Singleton()
	{
		pthread_mutex_init(&mutex);
	}
};
pthread_mutex_t Singleton::mutex;
//饿了肯定要饥不择食。所以在单例类定义的时候就进行实例化
//本来就是线程安全的
class Singleton
{
public:
	static Singleton *instance();
private:
	Singleton()
	{}
private:
	static Singleton *p;
};
Singleton *Singleton::p=new Singleton;
Singleton *Singleton::instance()
{
	return p;
}
//------------------特别注意懒汉和饿汉的区别就是懒汉创建单利对象
//是在调用函数Instance()的时候初始化类内定义的对象指针，而饿汉直接//给内部静态对象指针初始化了，调用函数只能返回初始化的值。
