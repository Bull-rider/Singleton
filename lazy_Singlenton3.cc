//懒汉模式下：加锁+静态释放
//了解atexit()的用法，通过该函数在线程结束时释放资源。和linux中的pthread_join()
//函数功能类似
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
using std::cout;
using std::endl;
class Singleton
{

	public:
		static Singleton *getInstance()
		{
			pthread_once(&_once,init);//pthread_once()函数使init函数只会被执行一次
			return _pInstance;
		}
		static void init()
		{
			_pInstance=new Singleton();
			atexit(destroy);
		}
		static void destroy()
		{
			if(_pInstance)
			{
				delete _pInstance;
			}
		}
	private:
		Singleton()
		{
			cout<<"Singleton()"<<endl;
		}
		~Singleton()
		{
			cout<<"~Singleton()"<<endl;
		}
	private:
		static Singleton *_pInstance;
		static pthread_once_t _once;
};
Singleton *Singleton::_pInstance=NULL;
pthread_once_t Singleton::_once=PTHREAD_ONCE_INIT;
int main()
{
	Singleton *p1=Singleton::getInstance();
	Singleton *p2=Singleton::getInstance();
	printf("p1=%p\n",p1);
	printf("p2=%p\n",p2);
	return 0;
}
