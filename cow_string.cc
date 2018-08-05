//赋值运算符函数和复制构造函数都没有真正的copy
//写时copy才真正copy了内容
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
class String
{
	class Charproxy
	{
		public:
			Charproxy(String self,size_t idex)
			:_self(self)
			,_idex(idex)
			{}
			char &operator=(const char &ch);
			friend std::ostream &operator<<(std::ostream &os,const Charproxy &rhs);
		private:
			String &_self;//引用String类中的对象
			size_t _idex;
	};
	friend std::ostream &operator<<(std::ostream &os,const Charproxy &rhs);
	public:
		String();
		String(const char *_pstr);//构造函数
		String(const String &rhs);//复制构造函数
		String &operator=(const String &rhs);//赋值运算符函数
		~String();
		char &operator[](size_t index);//下标重载运算符
		const  char &operator[](size_t index) const;
		size_t size() const//操作的字符串长度
		{
			return strlen(_pstr);
		}
		Charproxy operator[](size_t idex);
		const char *c_str() const//返回输出字符串
		{
			return _pstr;
		}
		size_t refcount() const//返回引用计数的个数
		{
			return *(int *)(_pstr-4);
		}
		friend std::ostream &operator<<(std::ostream &os,const String &rhs);
	private:
		void Initrefcount()
		{
			*(int *)(_pstr-4)=1;//引用计数的初始化
		}
		void Increatrefcount()//引用计数加1
		{
			++(*(int *)(_pstr-4));
		}
		void decreatrefcount()//引用计数减1
		{
			--(*(int *)(_pstr-4));
		}
		void release()
		{
			decreatrefcount();
			if(refcount()==0)
			{
				delete [] (_pstr-4);
			}
		}
	private:
		char *_pstr;
};
String::String()
{
	_pstr=(new char[5]()+4);//_pstr指向字符串的首地址
	Initrefcount();
}
String::String(const char *pstr)
:_pstr(new char[strlen(pstr)+5]()+4)
{
	strcpy(_pstr,pstr);
	Initrefcount();
}
String::String(const String &rhs)//复制构造函数只是把rhs的_pstr传给指针然后引用计数加1，
:_pstr(rhs._pstr)
{
	Increatrefcount();
}
String &String::operator=(const String &rhs)//赋值运算符:
{
	if(this!=&rhs)//自赋值的情况就是引用计数为1
	{
		release();
		/*decreatrefcount();
		if(refcount()==0)
		{
			delete [] (_pstr-4);
		}*/
	}
	_pstr=rhs._pstr;//如果不是自赋值，就把地址传给当前指针，然后引用计数加1
	Increatrefcount();
}
String::Charproxy String::operator[](size_t idex)//函数返回一个String域中的Charproxy对象成员。
{
	return Charproxy(*this,idex);
}
//返回String域中对象成员Charproxy中的字符的引用
char &String::Charproxy::operator=(const char &ch)
{
	if(_idex<_self.size())
	{
		if(_self.refcount()>1)
		{
			_self.decreatrefcount();
			char *temp=new char[_self.size()+5]()+4;
			strcpy(temp,_self._pstr);
			_self._pstr=temp;
			_self.Initrefcount();
		}
		_self._pstr[_idex]=ch;
	}
	return _self._pstr[_idex];
}
/*char &String::operator[](size_t index)
{//当要修改字符时，先执行写时copy再去改变那个字符的值。
	if(index<size())//size()得到的是_pstr字符串的长度
	{
		if(refcount()>1)
		{
			decreatrefcount();
			char *temp=new char[size()+5]()+4;
			strcpy(temp,_pstr);
			_pstr=temp;
			Initrefcount();
		}
		return _pstr[index];
	}
	else
	{
		static char nullchar='\0';
		return nullchar;
	}
}*/
String::~String()
{
	release();
	/*decreatrefcount();
	if(refcount()==0)
	{
		delete [] (_pstr-4);
	}*/
}
std::ostream &operator<<(std::ostream &os,const String &rhs)
{
	os<<rhs._pstr;
	return os;
}
void test1()
{
	String s1;
	String s2=s1;
	cout<<"s1=:"<<s1<<endl;
	cout<<"s2=:"<<s2<<endl;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	cout<<"s2's refcount="<<s2.refcount()<<endl;
}
//当调用复制构造函数引用计数加1,两个相同字符串的地址也相同
void test2()
{
	String s1="Hello world";
	String s2=s1;
	cout<<"s1=:"<<s1<<endl;
	cout<<"s2=:"<<s2<<endl;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	printf("s1's addr is:%p\n",s1.c_str());
	printf("s2's addr is:%p\n",s2.c_str());
}
//当调赋值运算符函数时,引用计数加1
void test3()
{
	String s1="Hello world";
	String s2="Henansheng";
	cout<<"s1=:"<<s1<<endl;
	cout<<"s2=:"<<s2<<endl;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	cout<<"s2's refcount="<<s2.refcount()<<endl;
	printf("s1's addr is:%p\n",s1.c_str());
	printf("s2's addr is:%p\n",s2.c_str());
	cout<<"赋值后:"<<endl;
	s2=s1;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	cout<<"s2's refcount="<<s2.refcount()<<endl;
	printf("s1's addr is:%p\n",s1.c_str());
	printf("s2's addr is:%p\n",s2.c_str());
}
//修改一个字符时
void test4()
{
	String s1="Hello world";
	String s2=s1;
	String s3="Henansheng";
	cout<<"s1=:"<<s1<<endl;
	cout<<"s2=:"<<s2<<endl;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	cout<<"s2's refcount="<<s2.refcount()<<endl;
	cout<<"s3's refcount="<<s3.refcount()<<endl;
	printf("s1's addr is:%p\n",s1.c_str());
	printf("s2's addr is:%p\n",s2.c_str());
	printf("s3's addr is:%p\n",s3.c_str());
	cout<<"修改字符后:"<<endl;
	s2[0]='W';
	cout<<"s1=:"<<s1<<endl;
	cout<<"s2=:"<<s2<<endl;
	cout<<"s1's refcount="<<s1.refcount()<<endl;
	cout<<"s2's refcount="<<s2.refcount()<<endl;
	printf("s1's addr is:%p\n",s1.c_str());
	printf("s2's addr is:%p\n",s2.c_str());
}
int main()
{
	//test1();
	//test2();
	//test3();
	test4();
	return 0;
}
