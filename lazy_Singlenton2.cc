//嵌套类+静态对象实现单例对象的自动回收
//静态函数实现了单例对象的创建，而嵌套类实现了单例对象的回收。
//单例对象多线程问题的两种解决方法
//法一：在初始化时我们就调用getInstance(),这样就不会有线程进入到getInstance()
//中；法二：在调用getInstance()时进行加锁，锁可以加在判断语句之前和之后都行
//但加在之后的效率更高。
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;
class Singleton
{
	class AutoRelease//唯一作用就是在析构函数中删除Singleton的实例
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
		Singleton(){cout<<"Singleton()"<<endl;}//构造函数和析构函数私有化
		~Singleton(){cout<<"~Singleton()"<<endl;}
	private:
		static Singleton *_pInstance;
		static AutoRelease _ar;//定义静态类成员
};
Singleton *Singleton::_pInstance=getInstance();//初始化
Singleton::AutoRelease Singleton::_ar;
int main()
{
	Singleton *p1=Singleton::getInstance();
	Singleton *p2=Singleton::getInstance();
	printf("p1=%p\n",p1);
	printf("p2=%p\n",p2);
	return 0;
}
