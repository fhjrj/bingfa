#include<iostream>
#include<thread>
#include <unistd.h>
#include<functional>
#include<vector>
#include<mutex>
#include<stack>
#include <memory>
#include <queue>
#include <condition_variable>
#include <future>
/*
void hello1{
std::cout<<"hello"<<std::endl;
}
*/

/*

class thread_guard{
    private:
    std::thread& t;//注意这里是引用 所以相当于是同一个线程 只是判断可不可join()
    //如果要变为常量 可以将其用移动语义
    public:
    explicit thread_guard(std::thread& t):t{t} {}
    ~thread_guard(){
        if(t.joinable()){
            t.join();
        }
    }
    thread_guard(thread_guard const &)=delete;
    thread_guard& operator =(thread_guard const &)=delete;
};

class hello{
    public:
    void operator()(){
        std::cout<<"hello"<<std::endl;
    }
};


void tim(){
    hello tx;
    std::function<void()> times=[]()->void{
        std::cout<<1<<std::endl;
    };
    std::thread x{times};
    std::thread m{tx};
    std::thread t{std::move(m)};
    //t=std::move(x); 一个线程变量只能维持一个函数 不能关联多个函数 不然会崩溃调用std::termiate()
    thread_guard a(t);
std::cout<<"顺序"<<std::endl;
}


int main(){
    tim();
   // const int &a=3;     常量引用的值必须是左值 
   //char m[]="ssds"; 
   //std::string s="dedw";
   //const std::string&  a=m;
//std::thread t{hello1};
//std::cout<<"this"<<std::endl;//可以看做 创建线程时 线程和主进程有进行相关函数 但是执行完此语句后 等待线程结束再执行下面程序 
//t.join();
//std::cout<<"that"<<std::endl;
}
*/


/*
 std::thread tex(){
std::function<void()> exe=[]()->void{
    std::cout<<"hello"<<std::endl;
};
std::thread m{exe};
return std::thread{std::move(m)}; //利用移动语义转移 从函数返回std::thread变量 将线程所属权交给函数外部
//同理 形参为std::thread类的常量 可以通过移动把线程所属权交给函数内部
}
int main(){
    std::thread mi=tex();
    mi.join();
}

*/

//修改进程守卫 在其对象离开作用域前 确保线程已经完全 
class scoped_guard{
    private:
    std::thread t;
    public:
    explicit scoped_guard(std::thread t):t{std::move(t)} {
        if(!t.joinable()){//定义scoped_guard变量管理目标前程，若在此之前目标线程已经结束了 则会在构造函数抛出异常 反之在析构才会调用
            throw std::logic_error("No thread");
        }

    }
    ~scoped_guard(){
            t.join();
    }
    scoped_guard(scoped_guard const &)=delete;
   scoped_guard& operator =(scoped_guard const &)=delete;
};/*


void sum(int m,int g){
    int sum=0;
    for(int i=0;i<=m;i++){
sum+=i;
    }
    std::cout<<"the"<<g<<"thread:  "<<sum<<std::endl;
}


void print(std::string &a){
    std::cout<<a<<std::endl;
}
int main(){
    std::string a="asddf";

    std::thread k{print,std::ref(a)};//对于形参为CONST引用  传递参数时必要要std::ref包装为左值引用 这样才能匹配，不然编译器默认以右值引用赋给形参 但是左值引用不能引用右值 
    k.join();

//线程计算 
    std::cout<<k.joinable()<<std::endl;
    std::cout<<"the thread k pid:"<<k.get_id()<<std::endl;//std::thread::get_id()可以获得当前的线程ID 并且可以比大小
  std::vector<std::thread> ans;
    for(int i=0;i<10;i++){
        ans.emplace_back(sum,i,i);
    }
    for(auto&c:ans){
        c.join();
}

}

*/

/*

//封装线程
template<typename iterator,typename T>
struct accumulate_blok{
    void opertaor()(iterator first,iterator end,T& result){
result=std::accumulate(first,end,result);
    }
};


template<typename iterator,typename T>
T parallel_accm(iterator first,iterator end,T init){
    unsigned long const lenth=std::distance(first,end);//需要算的距离
    if(!lenth){
        return init;
    }
    unsigned long min_per_thread=25;//每个线程 算25个元素
    unsigned long max_thread=(lenth+min_per_thread-1)/min_per_thread;//需要的线程数
    unsigned long const hard_wara_thread=std::thread::hardware_concurrency();//CPU核数
    unsigned long num_thread=std::min(hard_wara_thread!=0:hard_wara_thread:2,max_thread);//线程数尽可能小 上下文切换开销小
    unsigned long const block_size=lenth/num_thread;
    std::vector<std::thread> threads(num_thread-1);//排除当前的线程数
    std::vector<int> results(num_thread);
    itertaor start=first;
    for(int i=0;i<num_thread-1;i++){
        itertaor end=start;
        std::advance(end,block_size);
        threads[i]=std::thread(accumulate_blok<iterator,T>(),start,end,std::ref(results[num_thread-1]));
        start=end;
    }
    accumulate_blok<iterator,T>()(start,end,results[num_thread-1]);//当前线程在算一个
    for(auto& enrty:threads){
        enrty.join();
    }
return std::accumulate(results.begin(),result.end(),init);
}

void use_par_acc(){
    std::vector<int> vec(10000);
    for(int i=0;i<10000;i++)
    vec.push_back(i);

    int sum=0;
    sum=parallel_accm<std::vector<int>::iterator,int>(vec.begin(),vec.end(),sum);
    std::cout<<"the sum:"<<std::endl;

}

int main(){
use_par_acc();
}
*/


/*
std::mutex my1;
int shared_aate = 100;

void use_lock() {
	while (true) {
		//my1.lock();
		shared_aate++;
		std::cout << " this id" << std::this_thread::get_id() << std::endl;
		std::cout << "shared_date :" << shared_aate << std::endl;
		//my1.unlock();
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
}

void text_lock() {
	std::thread t1(use_lock);
	std::thread t2([]()->void {
		while (true) {
			//my1.lock();
			shared_aate--;
			std::cout << " this id" << std::this_thread::get_id() << std::endl;
			std::cout << "shared_date :" << shared_aate << std::endl;
			//my1.unlock();
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
		});
	t1.join();
	t2.join();
}
int main() {
	text_lock();
}

*/


/*
struct empty_stack :std::exception {
	const char* what() const throw ();
};

template<typename T>
class threadsafe_grade {
private:
	std::stack<T> data;
	mutable std::mutex x1;//锁是可变的 即使成员函数是const函数 也可以改变成员变量 要关键词mutable
public:
	threadsafe_grade() {}
	threadsafe_grade(const threadsafe_grade& other) {//构造函数不能加const 加了怎么初始化元素呢
		std::lock_guard<std::mutex> lock(other.x1);//取代x1.lock(); 到结束该函数作用域后 lock_guard自动调用其析构函数解锁
		data = other.data;
	}

	threadsafe_grade& operator=(const threadsafe_grade& othrt) = delete;

	void push(T value) {
		std::lock_guard<std::mutex> lock(x1);
		data.push(std::move(value));//减少一次拷贝
	}

//返回的是TOP元素的智能指针
	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(x1);
		if (data.empty()) {
			throw empty_stack();
		}
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();//只是在stack中存储的元素（stack.top()）被删除了 全部的程序空间中，别的位置还有
		return res;

	}
	//把元素TOP存在引用当中
	void pop(T& value) {
		std::lock_guard<std::mutex> lock(x1);
		if (data.empty()) {
			throw empty_stack();
		}
		value = data.top();//用引用接受 这样就保存了stack.top()
		data.pop();

	}//上述两种方法就可以通过指针或者引用找到那个元素

	bool empty()const {
		std::lock_guard<std::mutex> lock(x1);
		return data.empty();
	}
};

void test(){
    threadsafe_grade<int> safe;
    safe.push(1);

    std::thread m1{[&]()->void{
if(!safe.empty()){
    safe.pop();
}
    }};
    std::thread m2{[&]()->void{
if(!safe.empty()){
safe.pop();
    }
    }};                  
    m1.join();
    m2.join();
}

int main(){
    test();
}


*/


/*
std::mutex m1;
std::mutex m2;
int x1 = 0;
int x2 = 1;
void atomic_x1() {
	std::cout << "m1 start" << std::endl;
	m1.lock();
	x1 = 1024;
	m1.unlock();
	std::cout << "m1 end" << std::endl;
}

void atomic_x2() {
	std::cout << "m2 start" << std::endl;
	m2.lock();
	x2 = 4028;
	m2.unlock();
	std::cout << "m2 end" << std::endl;
}

void safe1() {
	while (true) {
		atomic_x1();//不存在死锁 当前操作进行完 马上释放锁
		atomic_x2();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

void safe2() {
	while (true) {
		atomic_x2();
		atomic_x1();
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

void text() {
	std::thread y1{ safe1 };
	std::thread y2{ safe2 };
	y1.join();
	y2.join();
}

int main(){
    text();//原子操作
}

//对于使用两个互斥量 尽可能让一个线程只占用一个互斥变量进行枷锁（如上面的atomic_x1和atomic_x2）  枷锁了后不能在占用另一个互斥量 当一个线程使用完临界资源后 要马上释放锁
//一个线程内不同时加锁可能会死锁

*/


//一般情况下 最好一个线程只占有一个锁 如果非要非要使用多个锁的化， 必须同时加锁（重要）

//自定义类必须要重载= 才能拷贝赋值

/*
class big_data {
public:
	big_data& operator = (const big_data & pp){
		if (this == &pp) {
			return *this;
	}
	_data = pp._data;
	return *this;
	}

	big_data(int data):_data(data){}
	//拷贝构造
	big_data(const big_data& b2):_data(b2._data){}
	//移动构造
	big_data(big_data&& m1) :_data(std::move(m1._data)){}//强转右值进行初始化
	//重载
	friend std::ostream& operator<<( std::ostream& os, const big_data& big) {//注意临时变量不能返回引用 所以这里的重载输出运算符是引用
		os << big._data;
		return os;
	}
	 void prin() {
		 std::cout <<"the messgae" << _data << std::endl;
	}
	friend void swap(big_data& m1, big_data& m2) {
		big_data tmp = std::move(m1);
		m1 = std::move(m2);
		m2 = std::move(tmp);
	}
private:
	int _data;
};

class big_obj {
public:
big_obj(int x):_find(x){}

 void printmmm() {
	std::cout << "finish" <<_find<< std::endl;
}
friend void danwao(big_obj& obj1, big_obj& obj2);
friend void safewaap(big_obj& obj1, big_obj& obj2);
friend void dsafescopewap(big_obj& obj1, big_obj& obj2);
private:
	std::mutex mtx;
	big_data _find;
};

void safewaap(big_obj& obj1, big_obj& obj2) {
	std::cout << "this threadn is" << std::this_thread::get_id() << std::endl;
	if (&obj1 == &obj2) {
		return;
	}
	std::lock(obj1.mtx, obj2.mtx);//同时上锁  当第一个线程开始时 两个互斥量都被占有  即使线程二抢占，但是这里已经被上锁了 无法访问 只有当线一访问结束才能访问线程二
	std::lock_guard<std::mutex> gard(obj1.mtx, std::adopt_lock);//后面参数告诉obj1.mtx已经加锁了 这个gard只会负责解锁

    //上面两个代码加上下面的std::lock_guard 等效于 std::scoped_lock garud(obj1.mtx, obj2.mtx); 他等效于同时加锁其在析构时同时解锁
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::lock_guard<std::mutex> gard1(obj2.mtx, std::adopt_lock);
	swap(obj1._find, obj2._find);
	std::cout << "this threadn is" << std::this_thread::get_id() << std::endl;
}
void text_anquan() {
	big_obj h1(5);
	big_obj h2(10);

	std::thread s1(safewaap, std::ref(h1), std::ref(h2));
	std::thread s2(safewaap, std::ref(h2), std::ref(h1));
	s1.join();
	s2.join();
	h1.printmmm();
	h2.printmmm();
}

*/

/*层级锁

class hierarchical_mutex{
    public:
    explicit hierarchical_mutex(unsigned long value):this_value(value), _previous_value(0){}
    hierarchical_mutex(const hierarchical_mutex&)=delete;//禁止默认拷贝构造和，默认拷贝运算符
    hierarchical_mutex& operator=(const hierarchical_mutex&)=delete;
//未定义移动构造，发生移动时会走拷贝构造，定义了移动构造，std::move才会移动构造，如上未定义移动构造和删除了拷贝构造，则不能走移动语义
void lock(){
    check_for_this_lock();
    _inter_mutex.lock();
    update_this_lock();
}
 
 void unlock(){
    if(this_value!=_previous_value){
        throw std::logic_error("mutex error");
    }
  _true_value =_previous_value;//回复
    _inter_mutex.unlock();
 }

 bool try_lock(){
    check_for_this_lock();
    if(!_inter_mutex.try_lock()){
        return false;
    }
    update_this_lock();
    return true;
 }
    private:
    std::mutex _inter_mutex;
    unsigned long const this_value;//初始化层级锁变量时的编号
    unsigned long _previous_value;//把加锁看成链表 这个就是上一次加锁的链表
    static thread_local unsigned long _true_value;// 存入的是当前线程最后一次加锁所涉及的层级编号
    //thread_local 把这个变量存在线程区间的 多个线程不会公用   及一个线程运行从开始到结束 都有这个变量
    void check_for_this_lock(){
        if(_true_value<=this_value){//要加锁的层级大于当前线程的层级 加不了
throw std::logic_error("mutex error");
        }
    }
    void update_this_lock(){//假设已经持有锁了 要加下一个锁，（当前线程最后一次加锁操作就是上一次操作）
        _previous_value= _true_value;
         _true_value=this_value;
    }
};
thread_local unsigned long hierarchical_mutex::_true_value(_SC_ULONG_MAX);//初始化最大 开始就可以加锁


hierarchical_mutex lower_mutex(500);
hierarchical_mutex mid_mutex(600);
hierarchical_mutex high_mutex(1000);

int do_lower_stuff(){
    return 1;
}

int do_lower(){
    std::lock_guard<hierarchical_mutex> gurad(lower_mutex);
    return do_lower_stuff();
}

void high_stuff(int tmpt){
    std::cout<<tmpt+1<<std::endl;
}
void high_fun(){
    std::lock_guard<hierarchical_mutex> guard1(high_mutex);
    high_stuff(do_lower());
}//从高到底进行加锁调用

void do_other_stuff(){
    std::cout<<"doing othrt"<<std::endl;
}

void othrt_stuff(){
  high_fun();//
    do_other_stuff();
}

void thread_b(){
    std::lock_guard<hierarchical_mutex> guard3(mid_mutex);
    othrt_stuff;
}

// 注意层级锁的用法是调用一个函数时用一个层级锁来锁定，确定线程调用多个函数的顺序或者防止死锁（该线程层级小于该函数层级，无法调用）

*/


//unique_lock 可以手动解锁或者自动析构解锁 lock_guard是结束调用析构函数解锁

/*
#include<mutex>
#include<iostream>
std::mutex mtx1;
std::mutex mtx;
std::mutex m2;
int share_data = 0;

void unique_lock() {
    std::unique_lock<std::mutex> guard(mtx);//一个mutex参数，guard将mutex上锁
    share_data++;
    guard.unlock();//guard将mutex手动解锁
    if (!guard.owns_lock())
        std::cout << "guard not has lock" << std::endl;        //判断是否加锁 加锁返回TRUE 未加锁返回FALSE
}

void jiesui_lovk() {
    std::unique_lock<std::mutex> x(mtx1, std::defer_lock);//延迟加锁
    x.lock();
    x.unlock();
    std::unique_lock<std::mutex> m(m2, std::adopt_lock);//std::adopt_lock会使m领养锁mtx，但m会保持mtx的锁状态，不会擅自加锁解锁
    if (m.owns_lock()) {//虽然没加锁 但m已经占用锁了 此函数结束时会调用m的析构函数对未加锁的锁进行解锁(这种情况下unique_lock内部变量无法判断是否加锁了，无法在析构时正确解锁)所以会崩溃 要std::adopt_lock一定要领养加了锁的互斥量
        std::cout << "jaisuo" << std::endl;
    }
    else {
        std::cout << "meiyoujiasuo" << std::endl;
    }
}//运行程序jiesui_lovk会崩溃 
int main() {
    //unique_lock();
    jiesui_lovk();
}

*/


/*注意std::thread不能用拷贝赋值 即
std::thread m1;
std::thread m2;
m2=m1; 这是错误的
还有std::mutex 转移线程所属全只能用std::move
*/


/*
#include<mutex>
#include<iostream>
#include<thread>
int a = 10;
int b = 20;
std:: mutex tx1;
std::mutex tx2;
std::mutex x3;
void safe_swap() {
	std::lock(tx1, tx2);
	std::unique_lock<std::mutex> guard1(tx1, std::adopt_lock);
	std::unique_lock<std::mutex> guard2(tx2, std::adopt_lock);
	std::swap(a, b);
	//错误操作  因为用了std::adopt_lock 将上锁解锁的权限交给了guard1，原来的互斥量不在有对锁的操作权，这个牢记，所以只能通过guard来解锁上锁 
	//tx1.unlock();
	//tx2.unlock();
	guard1.unlock();
	guard2.unlock();//std::unique_lock因为可以手动上锁和解锁，所以里面会有一个变量判断最后作用域结束是否要调用析构函数解锁 
}

void safe_swap2() {
	std::unique_lock<std::mutex> guard1(tx1, std::defer_lock);
	std::unique_lock<std::mutex> guard2(tx2, std::defer_lock);
	std::lock(guard1, guard2);
	//同上错误 std::lock(tx1,tx2),因为互斥量没有权力加锁解锁
	std::swap(a, b);

}

int main() {
	safe_swap2();
	std::cout << a << std::endl;
	std::cout << b <<std::endl;
	return 0;
}

*/


//std::unique_lock可移动(std::move,加锁权限的转移)不可复制，没有拷贝构造和拷贝运算符，std::mutex不可移动不可复制
//std::adopt_lock参数最好领养锁时，保证互斥量是上锁的，std::defer_lock使用时，保证互斥量是没上锁的

/*
//锁的转移使用
std::unique_lock<std::mutex> get_lock(){
    extern std::mutex some_lock;
    std::unqiue_lock<std::mutex> m1(some_lock);//some_lock上锁
    prepare_data();//上锁执行
    return std::move(m1);//std::unique_lock十分灵活，已经被移动出去了
}

void process_data(){
    std::unique_lock<std::lock> m2(get_lock());//接受右值，利用移动构造初始化， 加锁解锁权限交给m2
    do_something();
}

//std::unique_lock在多个函数中移动，保持一个副本
*/


  //std::shared_mutex变量  共享互斥量，多个线程可以同时锁住一个共享锁，
std::shared_mutex s1;
std::shared_lock<std::shared_mutex> m1(s1);//shared_lock<>创建共享锁并对共享互斥量上锁，创建过程需要共享互斥量 类似于lock_guard<>，被共享锁锁起来的数据可以被多个线程访问
std::lock_guard<std::shared_mutex> guard(s1);
std::unique_guard<std::shared_mutex> guard2(s1);
//上面两种方式创建的锁就只能一个进程访问，即上述方式，shared_mutex只属于一个线程，这不同于shared_lock<>

//对于共享锁，如果一些线程std::shared_lock方式持有共享锁
// 另一些线程想lock_guard或unique_guard占有锁，只能等所有用std::shared_lock方式持有共享锁的线程全部释放（这期间，想获取排他锁的线程会阻塞）,共享锁才能用lock_guard或unique_guard占有
//共享互斥量被排他锁方式占用时（在函数M里），其他想以std::shared_lock占用锁的一些线程或想调用函数M的一些线程都会被阻塞

class dnsserver {
public:
	dnsserver() {}
	//读数据，采用std::shared_lock，以std::shared_lock方式上锁的数据可以多个线程一起访问(一起占有这个共享锁)
	std::string querysns(std::string dnsname) {
		std::shared_lock<std::shared_mutex> shared_locks(_shared_mutex);
		auto iter = dns_info.find(dnsname);
		if (iter != dns_info.end()) {
			return iter->second;
		}
		return"";
	}

	//写操作
	void adddnsname(std::string name, std::string densrnty) {
		std::lock_guard < std:; shared_mutex > guardm(_shared_mutex);
		_dns_info.insert(std::make_pair(name, densrnty));
	}
private:
	std::map<std::string, std::string> _dns_info;
	mutable std::shared_mutex _shared_mutex;
};
//注意共享锁还是可变的,mutale    lock_guard，std::shared_lock,unique_lock模板类参数都是shared_mutex
//递归锁 std::recursive_mutex 可以重复加锁解锁，但不要使用 不推荐。
//共享锁作用 互斥写和写，互斥读和写（阻塞），不互斥读和读





/*
C++ 单例模式
这种模式涉及到一个单一的类，该类负责创建自己的对象，同时确保只有单个对象被创建。这个类提供了一种访问其唯一的对象的方式，可以直接访问，不需要实例化该类的对象。
单例模式是一种创建型设计模式，它确保一个类只有一个实例，并提供了一个全局访问点来访问该实例。
单例模式用于频繁创建和销毁全局使用的类实例的问
*/


/*
class singlepointer{
    private:
    singlepointer(){}
    singlepointer(const singlepointer&)=delete;
    singlepointer& operator=(const singlepointer&)=delete;//单例模式只保持一种初始化方式
    public:
    static singlepointer* getinst(){
        //此方式就是多线程获得实例，防止加锁阻塞，所以不加锁 但是不安全，需要安全则加锁
        if(single!=nullptr){
            return single;
        }
//这里是可能是两个线程同步访问，但访问到都没加锁，但只有一个线程会加锁进入，但该线程初始化返回后解锁（此时已经不是空）
//另一个线程上锁访问，所以还得判断一次是否为空，不为空直接返回
        m_mutex.lock();
        if(single!=nullptr){
            m_mutex.unlock();
            return single;
        }
        single=new singlepointer();
        m_mutex.unlock();
        return single;
    }
    private:
    static singlepointer* single;
    static std::mutex m_mutex;  
};

 singlepointer*  singlepointer::single=nullptr;
 std::mutex singlepointer::m_mutex;
// 上述单例模式问题 问题1.一定要delete掉 问题二.不能让多线程重复DELETE
//改进方法 用智能指针
*/

/*

class S{
    private:
    S(){}
    S(const S&)=delete;
    S& operator=(const S&)=delete;

    public:
    static std::shared_ptr<S> get(){
        if(single!=nullptr){
            return single;
        }
        m_mutex.lock();
        if(single!=nullptr){
            m_mutex.unlock();
            return single;
        }
        single=std::shared_ptr<S>(new S);//调用默认构造函数
        m_mutex.unlock();
        return single;
    }  
    private:
    static std::shared_ptr<S> single;
    static std::mutex m_mutex; 
};
std::shared_ptr<S> S::single = nullptr;
std::mutex S::m_mutex;
//智能指针解决了手动释放的方式
void test(int i) {
    auto m1 = S::get();
    auto m2 = S::get();
    std::cout << i << std::endl;
    std::cout << "m1 is" << m1 << std::endl;
    std::cout << "m2 is" << m2 << std::endl;
     //此时还有隐患 可以手动释放智能指针指向的区域  如果手动释放了在结束时再调用析构函数会删除裸指针报错
 //delete m1.get();
 //delete m2.get();
//修改方法 为了避免用户手动删除，定义一个辅助类取帮助实现
}

void m() {
    for (int j = 0; j <= 3; j++) {
        std::thread x1(test, j);
        x1.join();
    }
}

int main() {
    m();
}

*/


//std::call_once函数能保证多个函数进行一次

/*
class once{
    private:
    once (const once&)=delete;
    once& operator()(const once&)=delete;
    once()=default;
    static std::shared_ptr<once> _int;
    
    public:
    static std::shared_ptr<once> get(){
        static std::once_flag ff;//与std::call_once搭配使用 刚开始的时候初始化为false;
        std::call_once(ff,[&](){//当一个线程先访问时发现是false,会执行std::once里面的代码并且把ff变为true，后面的线程来访问时不会再次执行 会跳过此部分代码
            _int=std::shared_ptr<once>(new once);
        });//并发情况也不会，始终能保持函数只被调用一次

        return _int;
    }

    void print(){
        std::cout<<_int.get()<<std::endl;
    }

    ~once(){
        std::cout<<"over"<<std::endl;
    }
  
};

std::shared_ptr<once> once::_int=nullptr;

void tie() {
    std::thread m1([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        once::get()->print();//但凡调用了类中的成员函数，当程序结束时，成员函数分配被回首，也会调用析构函数。
        });
    m1.join();
}

int main() {
    tie();
}
*/


// std::thread m.join()是停止等待线程的，但是其能力有限，如果在一个线程里面，我想确定两个事件（线程）的发生顺序，可以使用标准库的等待事件的工具 
//比如在线程甲使用，受用后则需要线程乙执行某一特定的触发事件才能使得甲执行，该工具就是条件变量
//std;:condition_variable和std::condition_variable_any,前者只适合与std::mutex使用，后者只要某一类型能符合互斥的最低标准，足以充当互斥量，可以使用

/*
int mus = 1;
std::condition_variable ca;
std::condition_variable cb;
std::mutex nn;
void s() {
    std::thread t1([]() {
        for (;;) {
            std::unique_lock<std::mutex> lock(nn);
            ca.wait(lock, []() {
                return mus == 1;
                });
            mus++;
            std::cout << "thread t1" << std::endl;//执行完毕 通知线程B开始进行
            // lock.unlock();
            cb.notify_one();
        }

        });
    std::thread t2([]() {
        for (;;) {
            std::unique_lock<std::mutex> lock(nn);
            cb.wait(lock, []() {
                return mus == 2;
                });
            mus--;
            std::cout << "thread t2" << std::endl;
            //lock.unlock();
            ca.notify_one();
        }
        });
 
    t1.join();
    t2.join()
 // 之所以用unique_lock，是因为在一个线程等带的过程中，必须要释放自己做占有的资源，比如锁，通过调用WAIT函数判断，如果函数返回FALSE,,就会把线程挂起，等满足条件后，重新唤醒线程并加锁 std::lock_guard无法做到这样灵活
 // 如果使用std::lock_guard 在线程被挂起使未释放锁，而另一个线程需要锁，另一个线程就会被阻塞，这一点要注意，不是死锁       
}*/
//当 std::condition_variable 对象的某个 wait 函数被调用的时候，它使用 std::unique_lock(通过 std::mutex) 来锁住当前线程。当前线程会一直被阻塞，直到另外一个线程在相同的 std::condition_variable 对象上调用了 notification 函数来唤醒当前线程。

//上述t1.t2中最标准的写法是先解锁再发送通知，这样先解锁后t1，t2立刻获得锁并t1会通知t2可以执行了，t2开始往下执行，
//如果没有上述操作，t1运行,通知t2可以开始，但是t2没有获得锁被阻塞，相当于通知无意义，但t2获得锁以后依然可以执行 这点注意

//安全列队
/*
template <typename T>
class queuee {
private:
    mutable std::mutex mut;
    std::condition_variable data_cond;
    std::queue<T> data_queue;
public:
    queuee(){}
    queuee(const queuee& other) {
        std::lock_guard<std::mutex> gua1(other.mut);
        data_queue = other.data_queue;
    }
    void push(T data) {
        std::lock_guard<std::mutex> fa(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
    void wait_and_pop(T& data) {
        std::unique_lock<std::mutex> ss(mut);
        data_cond.wait(ss, [this]() {
            return !data_queue.empty();
            });//列队为空 就会一直挂起 等待元素插入 有元素后 通知可以取出
        data = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> ss(mut);
        data_cond.wait(ss, [this]() {
            return !data_queue.empty();
            });当不满足条件时，会解锁然后将线程挂起，但此时wait()未执行完毕，（相当于线程被阻塞在调用wait()函数中，但wait()函数还未返回）当有其他线程唤醒它时，wait函数会进行重新加锁，这时候wait函数才算执行完毕，然后该线程会从wait（）后面执行下去。
            如果满足条件 直接返回，运行下面代码
        std::shared_ptr<T> mm{ std::make_shared<T>(data_queue.front())};
        data_queue.pop();
        return mm;
     }
    //注意返回智能指针后并不是马上回收内存，要等返回的智能指针干了些事情才会回收，比如把返回值赋值给另一个智能指针，或者用返回的智能指针调用一些东西，等他们干完以后才会回收
    //这时引用计数减一

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> m1(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.pop();
        return true;
    }
        std::shared_ptr<T> try_pop() {
            std::lock_guard<std::mutex> m1(mut);
            if (data_queue.empty())
                return std::shared_ptr<T>();
            std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
            data_queue.pop();
            return res;
        }

        bool empty()const {
            std::lock_guard<std::mutex> gg(mut);
            return data_queue.empty();
        }
};

void tws() {
    queuee<int> safe_que;
    std::mutex m1;
    std::thread m11([&]() {
        for (int i = 0;; i++) {
            safe_que.push(i);
            {
                std::lock_guard<std::mutex> ms(m1);
                std::cout << "noe the pushdata is " << i << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
        });

    std::thread m2([&](){
        for (;;) {
            auto data = safe_que.wait_and_pop();//若为空 此处被阻塞
            {
                std::lock_guard<std::mutex> sh(m1);
                std::cout << "the curmser1 popdata is" << data << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(200));
        }
        });

    std::thread m3([&]() {
        for (;;) {
            auto data = safe_que.try_pop();
            if (data != nullptr) {
                {
                    std::lock_guard<std::mutex> sh(m1);
                    std::cout << "the curmser2 popdata is" << *data << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }
        });
    m11.join();
    m2.join();
    m3.join();
}

//安全列队
int main() {
    tws();
}
*/

//std::future 可以不像std::condition_variable那行，后者必须等待某个特定事件执行后才会继续执行下去（std::condition_variable被阻塞）， 
//std::future可以不用等待 当线程B等待线程A触发某件事之前 线程B不会被阻塞，可以去执行其他的事情，线程B会反复检查目标事情是否发生 。
//或者改变std::future的工作状态，先慢慢执行当前任务，在特定情况下才回头等待目标事情发生。（中途不会反复回头检查目标事件是否发生）
//用std::future变量代替特殊事件，且该特殊事件进入就绪状态时是不能被重置的，std::future变量启动一次，使用future等待 一次性事件的发生
//std::future的两种类型 std::future和std::shared_future
//前者 同一事件只允许关联唯一一个std::future对象 而后者 同一事件可以关联多个std::shared_future对象（一个std::shared_future对象派出多个副本给其他线程使用，其都指向同一个异步结果），并且目标事件发生，所有关联的实例会同时开始就绪
//而且他们全部都可以访问和该目的事件相关的所有数据
//从上可得std::shared_future无需同步执行，是异步的，而std::future不提供同步操作，若多个线程要同时访问一个std::future对象，必须使用互斥量或者其他同步操作进行保护
//std::thread不能从任务中返回计算结果，但是std::future可以
//std::future和std::async();后者是目给标函数传递参数 其构造和std::thread类似，有函数指针和给目标函数传数据


//std::launch::deferred 此种策略是任务将在等待结果时同步执行（任务在调用std::future::wait()或std;:future::get()时才会同步执行，之前一直未调用，该任务被阻塞）
//std::launch::async ,这种启动策略在调用std::async时 任务函数会异步并发执行


//模拟异步执行
std::string GETDB(std::string query) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return "data:" + query;
}

std::string GETS(std::string s) {
    std::this_thread::sleep_for(std:: chrono::seconds(3));
    return s + "this is test";

}

int ms(int n) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return n + 3;
}

void use_async() {
    //使用std::async异步调用GETDB，返回的数据存在std::future对象中 在这里不会被阻塞 data传输给query
    //std::anync
    std::future<std::string> result = std::async(std::launch::async, GETDB, "data");
    //主线程做其他事
    std::cout << "doing something" << std::endl;
    std::future<int> gs = std::async(std::launch::deferred, ms, 6);
    std::future<std::string> sm = std::async(std::launch::async | std::launch::deferred, GETS, "daks");//两种启动方式 std::async()自己选择一种 这个要注意

    std::string get2 = sm.get();

    int k = gs.get();//因为gs是std::deferred，当程序执行到gs.get()时，gs对应的任务才会执行,没有则是在后台异步执行
    std::string getdata = result.get();//从future对象获得数据  如果GETDB还没有执行完毕，程序就会在这里被阻塞 这里注意（未执行完毕,调用get会被阻塞）
   
    std::cout << getdata << std::endl;
}

int main() {
        use_async();
}

//std::async执行完毕返回一个std::future对象 要获得future对象存的值，直接调用GET函数
//对于std::async传递参数是右值时，是移动原始数据构建副本，则仅仅可移动的类型既可以作为函数对象，又可以充当std::async（）的参数

//注意 std::future对象只能get一次,get完毕后资源被回收,类似于std::join() 如果异步任务还没有完成 get()就会阻塞当前线程，直到任务结束
//std::future::wait(), 与get()区别的是wait()不会返回任务的结果，而是等待异步任务的完成，如果完成 wait()函数会立刻返回，如果异步任务未完成，会阻塞当前线程
//std::future::wait()，可以被future对象调用多次 这个注意。

//std::packaged_task<>连接可调用对象和future对象，std::packaged_task<>将调用函数的返回值储存在futuer对象
//std::packaged_task<>对象的模板是函数签名  比如指定的函数签名为 int(const string&,int);  则用std::packaged_task<>对象封装调用函数 std::packaged_task<int(const string&,int)> m;
//由于模板参数先行规定了函数签名 所以函数签名的形参类和返回类是必须可以转换的指定类型 这些类型不必严格匹配 比如包装一个int(int)类函数，可以指定为std::packaged_task<double(double)>  
//std::packaged_task<>包装函数以后 调用该函数和传参数都调用std::packaged_task<>中的成员函数opertaor()，或者移动给std::thread进行分开执行 如下例子


/*std::packaged_task<>,std::future不能进行拷贝赋值操作和拷贝构造,但支持右值拷贝构造函数（移动构造）和右值赋值（m& operator=(m&&)）   拷贝赋值没有m&& operator=(const m&&)这种类型*/
/*记住thread,future,promise,packaged_task<>不论赋值还是构造还是转移所属权，都用std::move()*/
/**/

int my_tast(int m) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "my tast is running 5s" << std::endl;
    return 42+m;
}


void use_package() {
    int a = 3;
    std::packaged_task<int(int)> task(my_tast);
  //task=my_task 这样是错误的，不允许函数用=进行赋值
    std::future<int> ans = task.get_future();//通过std::packaged_task<>中的get_future获得其关联的future对象  future类的模板是根据future关联的函数值确认的,获得future对象不会被阻塞

    //另一个线程执行任务
    //std::thread m2(std::move(task),a);//将tast的关联的函数归属权交给线程m2  托付给线程只能用std::move()
    //std::packaged_task<int(int)> task2=std::move(task)  将tast的关联的函数归属权交给同类型的packaged_task<>。
     task.operator()(a);//task(a);
    //std::packaged_task<>只支持移动构造函数进行初始化，所以初始化一个std::packaged_task<>只能移动构造（右值），且只能是括号初始化，对于同类型的packaged_task<>,能使用右值重载=函数进行战役管理，只有在这种情况下才能使用等号。
  // m2.detach();

    int m = ans.get();//获得返回值
    std::cout << "the ans is" << m << std::endl;
}

int main() {
    use_package();
}


//注意std::packaged_task<>有个缺点就是只能等待目标函数结束后才能获得其值，如果我们想在一个线程还没结束就获得其值，则采用std::promise
//std::promise给出了一个异步求值的方法，可以在一个线程还未结束就传给其他线程值， std::promise与其相关联的std::future可实现：等待数据的future对象阻塞，而提供数据的线程通过promise设定相关的值使其就绪
//std::promise获得其关联的future对象 还是通过get_future方法，promise的值通过成员函数set_value获得，只要设置好以后，future即准备就绪，凭借它就可以获得该值。


void set_value(std::promise<int> ss) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ss.set_value(10);
    std::cout << "promise is set over:  " << std::endl;
}

void set_value1(std::promize<int>& ss){
  std::this_thread::sleep_for(std::chrono::seconds(5));
    ss.set_value(10);
    std::cout << "promise is set over:  " << std::endl;
}


void use_prom() {
    std::promise<int> ft;
    std::future<int> gets = ft.get_future();//获得和其关联的std::future对象 使用std::future主线程不会被阻塞
    std::thread m1{set_value,std::move(ft) };//新线程设置promise的值,这时use_prom中ft已经为空 但是ft中设置的值已经关联到std::future对象了
     /*注意 这里传递只能进行右值传递，或者将set_value改造为接受std::promise的引用，则线程传递参数要用到std::ref()*/
     std::thread m3(set_value1,std::ref(ft));
    std::cout << "has get promise" << gets.get() << std::endl;//获得promise设置的值 调用get对象 ,等待返回结果.
    m1.join();
}

int main() {
    use_prom();
}

//promise和packaegd_task都是移动构造 所以只能用std::move 牢记

//可以将子线程抛出的异常通过try_catch块存储在future对象中，然后主线程必须要通过try_catch块获得主线程中future对象保存的子线程的异常 不然要崩
//同步和异步 同步是所有操作都做完了才能“返回消息” 异步是不需要全部都执行完就可以返回部分消息 
//std::thread 和std::future一样 前者只能被join()一次 后者只能被get一次，所以多个线程如果使用同一个std::future对象，只能被get一次，不然会报错 两者都是可以在不同线程之间进行转移，但同一时刻只能有唯一一个future对象实例指向异步完成的结果 
//std::shared_future类可以创建出副本，安全做法是给每一个线程一个std::shared_future对象副本，他们都可以get来获得值，他们都指向同一个异步任务的状态数据（共享异步状态）
//多个线程等待同一个执行结果

//shared_future支持拷贝和移动，只用std::future支持move

void test(std::promise<int>&& ss) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ss.set_value(42);
}

void test2(t> std::shared_future<int> s1 ) {
    try {
        int result = s1.get();
        std::cout << "result" << std::endl;
    }
    catch(std::future_error& e){
        std::cout << "error" << e.what() << std::endl;
    }
}


void use_shared() {
    std::promise<int> nums;
    std::shared_future<int> future = nums.get_future();//这里返回的是std::future 但其隐式转换为std::shared_future(移动构造)
    //上述等价 auto sss=nums.get_future().share()
    //std::future成员函数有share() 自动创建std::shared_future对象并且自动转移所属权，原来不在保有。
    std::thread js(test, std::move(nums));//移动到里面进行promise设值
    std::thread a1(test2, future);
    std::thread a2(test2, future);//不是std::move 
    //线程a1 a2都有shared_future变量 这里注意形参类型是拷贝 是以拷贝创建了副本 而且传参不是std::move ，加入这样对线程以这样做 future对象已经跑到 a1的test2里面了 future已经为空，a2不能在进行
    js.join();
    a1.join();
    a2.join();
}
//shared_future是支持拷贝的 不想std::future只支持移动
int main() {
    use_shared();
}

//shared_future拷贝副本时是形参传参都是拷贝操作，不是移动 


 
//声明一个可调对象T
using T = std::function<int(int)>;		//等同于typedef std::function<int(int)> T;
 
int Test_Fun(int iVal)
{
	std::cout << "Value is:" << iVal << std::endl;
	return iVal + 232;
}
 
void Thread_Fun1(std::promise<T> &p)
{
	//为了突出效果，可以使线程休眠5s
	std::this_thread::sleep_for(std::chrono::seconds(5));
 
	std::cout << "传入函数Test_Fun" << std::endl;
 
	//传入函数Test_Fun
	p.set_value(std::bind(&Test_Fun, std::placeholders::_1));
}
 
void Thread_Fun2(std::future<T> &f)
{
	//阻塞函数，直到收到相关联的std::promise对象传入的数据
	auto fun = f.get();		//iVal = 233
 
	int iVal = fun(1);
 
	std::cout << "收到函数并运行，结果：" << iVal << std::endl;
}
 
int main()
{
	//声明一个std::promise对象pr1，其保存的值类型为int
	std::promise<T> pr1;
	//声明一个std::future对象fu1，并通过std::promise的get_future()函数与pr1绑定
	std::future<T> fu1 = pr1.get_future();
 
	//创建一个线程t1，将函数Thread_Fun1及对象pr1放在线程里面执行
	std::thread t1(Thread_Fun1, std::ref(pr1));
	//创建一个线程t2，将函数Thread_Fun2及对象fu1放在线程里面执行
	std::thread t2(Thread_Fun2, std::ref(fu1));
 
	//阻塞至线程结束
	t1.join();
	t2.join();
 
	return 1;
}







struct X{
    void foo(int,std::string const &);
    std::string bar(std::string const&);
};

X x;
auto f1=std::async(&X::foo,&x,42,"hello");//对函数去地址是函数指针 调用&x->foo(42,hello);
auto f2=std::async(&X::bar,x,"sdsdd");//调用x.bar("sdsdd");
//注意第二个参数的区别 一个类变量表示地址 ，另一个是地址 两种调用方法不一样
struct Y{
    double operator()(double);
};

Y y;

auto f3=std::async(std::ref(y),2.712);//引用方式调用仿函数 就是y(2.712);
auto f4=std::async(Y(),3.1231);//前者是生成了一个匿名对象传递给了std::async，进而发生移动构造(std::async()内部产生一个对象，再执行Y::opertaor());
//std::async都是返回的是std::future对象 将其关联函数值存在future对象中
//shared_future同std::future可以try catch捕获异常 这个重要
//通过try catch捕获的异常存放在std::future对象里，然后拿出去，依然在主线程中要用try-catch捕捉 这个是很重要的



//线程池的实现
class ThreadPool  {
public:
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool&operator=(const ThreadPool&) = delete;

    static ThreadPool& instance() {
        static ThreadPool ins;//所有该类都共享这一个变量
        return ins;
    }
//线程池一定是保证其是唯一的 所以用单例模式
    using Task = std::packaged_task<void()>;
//任务队列都是存储的该类型


    ~ThreadPool() {
        stop();
    }

    template <class F, class... Args>
    auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));
        if (stop_.load())
            return std::future<RetType>{};

        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<RetType> ret = task->get_future();
        {
            std::lock_guard<std::mutex> cv_mt(cv_mt_);
            tasks_.emplace([task] { (*task)(); });
        }
        cv_lock_.notify_one();
        return ret;
    }

    int idleThreadCount() {
        return thread_num_;
    }

private:
    ThreadPool(unsigned int num = 5)
        : stop_(false) {
            {
                if (num < 1)
                    thread_num_ = 1;
                else
                    thread_num_ = num;
            }
            start();//初始化线程池的数量
                   //启动线程池
    }
    void start() {
        for (int i = 0; i < thread_num_; ++i) {
            pool_.emplace_back([this]() {
                while (!this->stop_.load()) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        this->cv_lock_.wait(cv_mt, [this] {
                            return this->stop_.load() || !this->tasks_.empty();
                        });
                        if (this->tasks_.empty())
                            return;

                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    this->thread_num_--;
                    task();
                    this->thread_num_++;
                }
            });
        }
    }
    void stop() {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto& td : pool_) {
            if (td.joinable()) {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
    }

private:
    std::mutex               cv_mt_;
    std::condition_variable  cv_lock_;
    std::atomic_bool         stop_;
    std::atomic_int          thread_num_;
    std::queue<Task>         tasks_;
    std::vector<std::thread> pool_;
};


//c++17中引入了内置函数 gcd求最大公约数 lcm求最小公倍数


//时钟类 std::chrono类中有时间操作的一些函数 具体见书P 97 次数只介绍支持超时延迟的函数 需要用到std::chrono::duration<> 如下面例子 我们等待某个future对象进入就绪状态，并以35毫秒为限制
std::future<int> f=std::async(some_task);
if(f.wait_for(std::chrono::milliseconds(35))==std::future_status::ready){
    do_something(f.get());
}

//f在35毫秒内就绪，即f关联的函数已经执行完并且返回值存在.get()函数中，wait_for()函数返回std::future_status::ready,如果超过35毫秒 则返回std::future::deferred


//归并排序
void mergesort(vector<int>& nums,int left,int mid,int right){
    vector<int> ans(right-left+1);
    int i=left,k=0,j=mid+1;
    while(i<=mid&&j<=right){
        if(nums[i]<=nums[j]){
            ans[k++]=nums[i++];
        }else{
            ans[k++]=nums[j++];
        }
    }
    while(i<=mid){
        ans[k++]=nums[i++];
    }
    while(j<=right){
        ans[k++]=nums[j++];
    }
    for(int k=0;k<ans.size();k++){
        nums[left+k]=ans[k];
    }
}

void merge(vector<int>& nums,int left,int rght){
    if(left>=right) return;
    int mid=left+(right-left)/2;
    merge(nums,left,mid);
    merge(nums,mid+1,right);
    mergesort(nums,left,mid,right);

}








//快排的上升排序  先找到一个基准元素 比其小的放在左边 比它大的放右边，接下来继续下去 最终是有序的
template<typename T>
void quick_sort(T arr[],int start,int end){
    if(start>=end) return;
    T key=arr[start];
    int left=start,right=end;
    while(left<right){
        while(arr[right]>=key&&right>left) right--;
        while(arr[left]<=key&&right>left) left++;
        std::swap(arr[left],arr[right]);
    }
    //终止 箭头相遇了 停止交换 
    if(arr[left]<key){
        std::swap(arr[left],arr[start]);
    }

    //分治
    quick_sort(arr,start,left-1);
    quick_sort(arr,left+1,end);
}

//函数式编程进行快速排序
template<typename T>
std::list<T> quick_so(std::list<T> input){
    if(input.empty()){
        return input;
    }
    std::list<T> reuslt;
    reuslt.splice(reuslt.begin(),input,input.begin());//将INPUT的第一个元素移动到reuslt中
    T const& tmp=*input.begin();
    auto divide_point=std::partition(input.begin(),input.end(),[&](T const& t){
        return t<tmp;
    });
    //std::partition是标准库函数 使得满足条件的元素排在不满足条件的元素之前 divide_point返回第一个大于tmp的元素 如上比tmp小的排在左边
    std::list<T> lower;
    lower.splice(lower.end(),input,input.begin(),divide_point);//这里相当于将比tmp小的元素从input里拿出来放在新的列表中 >tmp的还在input里


//lower是副本 所以其实可以用线程并行执行
//std::future<std::list<T>> dfs=std::async(&quick_so<T>,std::move(lower)); （当然用线程池可行）


    auto new_lower=quick_so(std::move(lower));//开始递归排序 <tmp的又开始
    auto new_highwe=quick_so(std::move(input));//＞tmp的部分也开始  
    //最后已经是排好序了
   reuslt.splice(reuslt.end(),new_highwe);
   result.splice(reuslt.begin(),new_lower);
   //result.splice(result.begin(),dfs.get()); std::future类的拼接
   return reuslt;

}

//并发模型actor channl模型

/*
template<typename T>
class channel {
private:
    std::queue<T> que;
    std::mutex mtx;
    std::condition_variable produce;
    std::condition_variable consummer;
    size_t size;
    bool closed = false;
public:
    channel(size_t m = 0) :size(m) {}
    bool send(T value) {
        std::unique_lock<std::mutex> lock(mtx);
        produce.wait(lock, [this]() {
            return (size == 0 && que.empty()) || que.size() < size || closed;//如果列队为空，里面还没有东西，或者容量还没有慢，我们可以生产者就可往里面插入东西，满了就要挂起了，等消费者消费才能走
            });
        if (closed) {
            return false;
        }
        que.push(value);
        consummer.notify_one();//插入数据了 消费者可以去消费了。
        return true;
    }

    bool recver(T& value) {//消费者取数据
        std::unique_lock<std::mutex> lock(mtx);
        consummer.wait(lock, [this]() {
            return !que.empty() || closed;
            });
        if (closed && que.empty())
            return false;
        value = que.front();
        que.pop();
        produce.notify_one();//已经取出数据了 通知可以往里面插入数据了
        return true;
    }


    //用于外部调用 进行消费停止关闭
    void close() {
        std::lock_guard<std::mutex> lock(mtx);
        closed = true;
        produce.notify_all();
        consummer.notify_all();
    }
};


int main() {

    channel<int> ch(10);
    std::thread m1([&]() {
        for (int i = 0; i < 5; i++) {
            ch.send(i);
            std::cout << "has sent over" << std::endl;
        }
        ch.close();//插入元素结束 开始通知所有消费者 生产者,channel关闭 （但其实生产者不会活动了，消费者会继续）
        });
    std::thread m2([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        int val;//recver函数传的是引用 所以取出来的值存入VAL中;
        while (ch.recver(val)) {
            std::cout << "recver:" << val << std::endl;
        }
        });

    m1.join();
    m2.join();
    return 0;
}
*/
//什么是原子操作，即对应底层硬件只是进行一部操作（细分到最细的步骤） 例如i++,本质是三步原子操作，第一步将变量i被cpu提取，第二部cpu进行计算，第三步将计算后的i返回并更新所有缓存中的信息


// 原子操作的目的之一是CPU缓存一致性 使得多级高速缓存区，在一个变量的值改变后，所有多级缓存区的储存相应的副本 最终都会跟新
//原子操作是防止发生多个线程恶行竞争，原子操作就是强制让多个线程按照一定顺序执行（内存模型），和强制按照一定顺序加锁解锁一样，可以防止死锁
//原子操作的出现就是取代使用锁来完成互斥操作的，从而原子操作引发出了无锁操作
//对于原子类型来说，原子变量无锁的形式去运用在程序中是线程安全的，因为原子操作本来就是代替锁操作的，C++17的所有原子类型都含有静态常量表达式 std::atomic::is_always_lock_free 该表达式可以判断原子类型X是否始终以无锁结构形式实现，如果都是无锁结构(线程安全的)，返回true,不是就返回false
//返回false,说明机器不支持原子操作，要自己进行加锁操作

//自旋锁是一种在多线程环境下保护共享资源的同步机制。它的基本思想是，当一个线程尝试获取锁时，如果锁已经被其他线程持有，那么该线程就会不断地循环检查锁的状态，直到成功获取到锁为止。
//与普通锁的区别的是当一个线程已经加了锁，另一个线程想持有锁，但发现锁已经被持有了，就会被阻塞挂起，而不会循环检查锁的状态
//所有原子类型禁止拷贝复制和拷贝构造



/*
原子变量的操作主要分为三中，分别调用的是内置类中的函数
1.store()储存操作，即以原子方式进行储存（以原子操作进行赋值）
2.load() 载入操作，以原子操作进行读变量
3.read-modify-write(),读改写操作，以原子方式进行，

与原子方式操作的还有其对应的内存顺序

*/
class Slock {
public:
    void lock() {
        while (falg.test_and_set(std::memory_order_acquire));//载入操作，自选等待，只当成功获得锁
    }
    void unlock() {
        falg.clear(std::memory_order_release);//释放锁
    }
private:
    std::atomic_flag falg = ATOMIC_FLAG_INIT;//std::atomic_flag变量只能用宏 ATOMIC_FLAG_INIT初始化为0,这里赋值是原子值
};
//std::atomic_flag只有0，1两个值，且只支持销毁，置零，读取原有的值并且设置标志成立


void lock1() {
    Slock lockm1;
    std::thread m1([&lockm1]() {
        lockm1.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "*";
        }
        std::cout << std::endl;
        lockm1.unlock();
        });
    
    std::thread mw([&lockm1]() {
        lockm1.lock();
        for (int i = 0; i < 3; i++) {
            std::cout << "?";
        }
        std::cout << std::endl;
        lockm1.unlock();
        });

    m1.join();
    mw.join();
}


/*
关于std::memory_order_relaxed具备如下几个功能：

1 作用于原子变量 2 不具有synchronizes-with关系 3 对于同一个原子变量，在同一个线程中具有happens-before关系, 在同一线程中不同的原子变量不具有happens-before关系，可以乱序执行。 4 多线程情况下不具有happens-before关系。
（happens-before：如果事件A发生在事件B之前，则事件A的结果对B是可见的）
由上述可知，如果采用最松散的内存顺序模型，在一个线程中，如果某个表达式已经看到原子变量的某个值a，则该表达式的后续表达式只能看到a或者比a更新的值。

它只保证操作的原子性，并不能保证多个变量之间的顺序性，也不能保证同一个变量在不同线程之间的可见顺序。

所以 CPU在执行线程时，可能不会按照代码编程顺序执行，这个要看cpu的调度算法，但一般是按照代码顺序执行  
++i;
i--;
可能cpu先执行2，再执行1
总结  线程的执行顺序是多样的，不确定的
std::memory_order_relaxed是最宽松的内存模型，所以CPU不一定按照线程中代码编写的顺序来执行
*/


/*
std::atomic<bool> 

其也像std::atomic_flag,不能进行 拷贝构造或者拷贝赋值，但std::atomic<bool>类 可以依据非原子布尔量创建其对象，还能接受非原子布尔值的赋值
对于原子变量，它们支持的赋值操作符不返回引用，而是按值进行返回，这个注意。

*/

/*
 std::atomic<bool> sa;
 bool time = sa.load(std::memory_order_acquire);
 sa.store(true);
 std::cout << time << std::endl;//0
 std::cout << sa << std::endl;//1
 time = sa.exchange(false, std::memory_order_acq_rel);//exchange函数和text_and_set一样 读取前面原子变量中储存的值，然后再修改值
 std::cout <<time << std::endl;//1
 std::cout << sa << std::endl;//0


 大部分的原子类都是exchange()函数，其属于读与写部分，注意读与写部分返回的是非原子变量的普通类型，而不是std::atomic<>对象本生
 */

/*
std::atomic<T*> 类  同样不能拷贝赋值或者拷贝复制，同样具有成员函数is_free_lock(),load(),store(),exchange(),compare_exchange_weak(),ompare_exchange_strong();
同样可以进行接受参数和返回参数非原子类型T*，
该原子变量增有指针运算， fetch_add(),fetch_sub(),（读改写操作，可以选择任何内存次序）分别对应储存地址进行原子化的加减，还包装了重载运算符--，++(前后版本)，+=，-= 

 int arr[] = { 1,2,3,4,5,6,7 };
 std::atomic<int*> sp(arr);
 std::atomic<int*> k;
 int* pp = nullptr;
 k.store(arr, std::memory_order_relaxed);
 std::cout << *k << "  " << *sp << std::endl;
 pp = sp.fetch_add(4, std::memory_order_release);//返回的是原来的int* 类型
 std::cout << *pp << "  " << *sp << std::endl;
 int* m = sp.load();
 std::cout << *m << std::endl; //返回的是原来的int* 类型

对于泛化的std::atomic<>类，最好不要用（即是自定义类的原子变量），用std::mutex代替
*/


/*
void tett(){
    std::atomic<bool> mm1, mm2;
    std::atomic<int> z(0);
    mm1 = false;
    mm2 = false;

    std::thread m1([&]() {
        mm1.store(true, std::memory_order_release);
        mm2.store(true, std::memory_order_release);
        });

    std::thread m2([&]() {
        while (!mm2.load(std::memory_order_acquire));
        if (mm1.load(std::memory_order_acquire)) {
            z++;
        }
        });
    m1.join();
    m2.join();
    std::cout << z.load() << std::endl;
}

int main() {
    tett();
} 
*/

std::memory_order_relaxed//最宽松内存序
std::memory_order_release std::memory_order_acquire//同步组合使用，可以确定不同线程之间的，对同一原子变量的操作顺序，对于不同线程之间，如果先发生释放操作，那么对获得操作是可见的（即执行acquire时，该线程会通知跟新所有数据，保证数据一致性）
//不是说一定先发生std::memory_order_release操作(读)，再会发生std::memory_order_acquire操作（写与改），不同线程之间并发不一定。   如果同一线程有对同一原子变量的两个操作，这是先进行release,再发生acquire
std::memory_order_seq_cst//最严格的内存序列，保持全局一致性，能同步及时更新缓存的信息，确保所有线程都可见，都能使用最新数据
//std::lock,上锁中一个线程进行变量的修改就是对全部线程都是可见的，所以锁是简单而强大的，但代价自然是开销高
//再次注意happens-before关系是 如果A先发生，那么对于B是可见的最新结果，不是A一定发生在B之前

//通过原子变量想要设计不同线程之间的进行次序，必须是对同一个原子变量进行“读”，“改”，“写“操作

//获得释放次序，只能同步于一对操作，比如同一个原子变量，两个释放操作（A,B），一个获得操作(C)，那么只有(A,C)或（B,C）为同步关系，(A,B)不构成任何操作序列
//std::memory_order_consume std::memory_order_release相搭配 前者就类似于std::memory_order_acquire
/*
release只能保证其他线程都可见，并不能保证其他线程可以正确读到，这就是才引入acquire， 而consume只能保证相关原子（调用store（）采用std::memory_order_release）才能读到。
*/



//获得释放操作 一个释放操作（std::memory_order_release)A，两个获得操作（std::memory_order_acquire)B,C   则(A,B) (A,C)同步，（B,C）不同步，B,C调用load（）得出来的值可能相同

// 栅栏  栅栏可以在没有同步和先行关系下引入如此关系
std::atomic<bool> yyy,xxx;
std::atomic<int> zzz;

void w_x_y_() {
    xxx.store(true, std::memory_order_relaxed);//A
    std::atomic_thread_fence(std::memory_order_release);//加入释放栅栏
    yyy.store(true, std::memory_order_relaxed);//B
}
//栅栏只能放在中间才能强制确定服从先后服从顺序，这个注意,不能把释放栅栏放在yyy储存操作后面
void ree() {
    while (!yyy.load(std::memory_order_relaxed));//C
    std::atomic_thread_fence(std::memory_order_acquire);//加入获得栅栏
    if (xxx.load(std::memory_order_relaxed)) {//D
        zzz++;
    }
}

//如上加入两个栅栏，使得A D构成同步操作,  B C构成同步操作
//栅栏的具体作用是，储存操作在释放栅栏后面，而储存操作的结果为获得操作可见，则该释放操作和获得操作同步（同一变量）
//若载入操作在获得栅栏之前，而载入操作看见了释放操作的结果 则该获得栅栏和释放操作同步。 （同一变量）
//结合上述两个 两个栅栏进行同步。注意同步点是栅栏
//栅栏的作用还可以将部分非原子进行同步，但是注意释放栅栏和获得栅栏内：变量操作的释放和储存是对原子变量的，比如以上可以将xxx变为非原子变量类，但yyy依然是原子变量类
int main() {
    yyy = false;
    xxx = false;
    zzz = 0;
    std::thread m1(w_x_y_);
    std::thread m2(ree);
    m1.join();
    m2.join();
    std::cout << zzz.load() << std::endl;
    return 0;

}
//栅栏可以隔断线程打破如上储存顺序std::memory_order_relaxed


//利用原子操作从而改良单例模式
//注意单例模式由于是共享的数据，所以用static变量 但不要忘了编译时刻进行初始化
class SingleMemoryModel
{
private:
    SingleMemoryModel()
    {
    }
    SingleMemoryModel(const SingleMemoryModel&) = delete;
    SingleMemoryModel& operator=(const SingleMemoryModel&) = delete;
public:
    ~SingleMemoryModel()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleMemoryModel> GetInst()
    {
        // 1 处
        if (_b_init.load(std::memory_order_acquire))
        {
            return single;
        }
        // 2 处
        s_mutex.lock();
        // 3 处
        if (_b_init.load(std::memory_order_relaxed))//为什么采用内存序 std::memory_order_relaxed?因为已经上锁了，只能有一个线程可以访问的到，所以是最新的内容
        {
            s_mutex.unlock();
            return single;
        }
        // 4处
        single = std::shared_ptr<SingleMemoryModel>(new SingleMemoryModel);
        _b_init.store(true, std::memory_order_release);
        s_mutex.unlock();
        return single;
    }
private:
    static std::shared_ptr<SingleMemoryModel> single;
    static std::mutex s_mutex;
    static std::atomic<bool> _b_init ;
};

std::shared_ptr<SingleMemoryModel> SingleMemoryModel::single = nullptr;
std::mutex SingleMemoryModel::s_mutex;
std::atomic<bool> SingleMemoryModel::_b_init = false;
//不要忘了静态变量初始化

//基于单线程安全的列队/链表

template<typename T>
class queue {
private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        node(T _data) :data(std::move(_data)){}
    };
    std::unique_ptr<node> head1;//利用智能指针的性质回收内存
    node* tail;
public:
    queue():tail(nullptr) {}
    queue& operator=(const queue&) = delete;
    std::shared_ptr<T> try_pop() {
        if (head1 == nullptr) return std::shared_ptr<T>();
        std::shared_ptr<T> const res(std::make_shared<T>(head1->data));//因为head1管理的是node类 node里面还有个智能指针 将head里面的元素移动交付给res管理后，head1->data没了
        std::unique_ptr<node> const old_node = std::move(head1);//这是原来head1引用计数为0 其指向的内存被释放 但该变量没有被释放,head1变为空指针
        head1 = std::move(old_node->next);//转移指针
        if (!head1)
            tail = nullptr;
        return res;
    }
    void push(T new_value) {
        std::unique_ptr<node> p(new node(std::move(new_value)));//调用构造函数
        node* const new_tail = p.get();//获得p指向的地址
        if (tail) {
            tail->next = std::move(p);//node里面有一个智能指针
        }
        else {
            head1 = std::move(p);
        }
        tail = new_tail;
    }
};
//如果改为多线程则会有问题 必须要两个锁互斥保护头节点和尾结点的操作 
//并且push和try_pop()可能会并发访问同一个结点的next指针解决方法是加一个虚拟头节点
//加一个虚拟头节点的好处就是 pop()和push（）不在操作相同的结点，则可以各用一个互斥量访问
//head和tail会指向不同的结点（有数据的情况下），而不像上例线程代码如果只有一个数据,head和tail会指向同一个位置，则调用pop和push时可能会有竞争，加入虚拟结点后可以减少该竞争了
//代码如下

template<typename T>//指针结点本来就包含一个数据域 一个指针域
class queue {//tail head2当作全局的指针
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::unique_ptr<node> head2;
    node* tail;
public:
    queue() :head2(new node), tail(head2.get()) {}//这里的new node就是在初始化的时候的虚拟头节点
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;//拷贝构造和拷贝操作符给他删了
    std::shared_ptr<T> try_pop() {
        if (head2.get() == tail) {
            return std::shared_ptr<T>();//没元素
        }
        std::shared_ptr<T> const res(head2->data);//本来data就是std::shared_ptr类 调用拷贝构造进行赋值 这里引用计数为2 
        std::unique_ptr<node> olde_head = std::move(head2);
        head2 = std::move(olde_head->next);//结点后移
        return res;
    }
    void push(T new_value) {
        std::shared_ptr<T> news(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);//创建一个空结点
        tail->data = news;//先更新 存储结点
        node* const new_tail = p.get();
        tail->next = std::move(p);//指向新的游离节点
        tail = new_tail;//更新
    }
};

//上述本质先创建一个queue实例时 tail和head2都指向空node 每当插入数据时 比如先插入2 ,2储存在开始的空node中,然后再创建一个空node，tail2更新指针域后指向新的空结点
//比如插入A,B,C  则是A->B->C->node(空)
//                  |           |
//                   
//
/*               head2        tail    注意tail和 head2
int main() {
    std::cout << "hello";
    queue<int> a;

    a.push(1);
    a.push(2);
    a.push(3);
    std::shared_ptr<int> s = a.try_pop();
    std::cout << *(s.get()) << std::endl;

}
*/


//下面是实现多线程安全的列队
 template<typename T>
 class thread_queue {
 private:
     struct node {
         std::shared_ptr<T> data;
         std::unique_ptr<node> next;
     };
     std::unique_ptr<node> head;
     node* tail;
     std::mutex head_mut;
     std::mutex tail_mut;
     node* get_tail() {
         std::lock_guard<std::mutex> lock(tail_mut);
         return tail;
     }

     std::unique_ptr<node> pop_head() {
         std::lock_guard<std::mutex> lock(head_mut)//之所以将get_tail保护在head_mut之内 可以保证一旦一个线程要执行pop_head() 则其他线程没有能力改变head指针,确保old_head和head一致 所以上锁一般是一来就上
             //不要在线程中途上 可能有不好的发生
             if (head.get() == get_tail()) {
                 return nullptr;
             }
         std::unique_ptr<node> old = std::move(head);
         head = std::move(old->next);
         return old;
     }
 public:
      thread_queue():head(new node),tail(head.get()){}
      thread_queue& operator=(const thread_queue&) = delete;
      thread_queue(const thread_queue&) = delete;
      std::shared_ptr<T> try_pop() {//本生值就是以std::shared_ptr储存 所以直接返回
          std::unique_ptr<T> old = pop_head();
          return old == nullptr ? std::shared_ptr<T>() : old->data;//
      }

      void push_new(T new_value) {
          std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
          std::unique_ptr<node> p(new node);
          node* const new_tail = p.get();
          std::lock_guard<std::mutex> taill(tail_mut);
          tail->data = new_data;
          tail->next = std::move(p);
          tail = new_tail;//tail=p.get()
      }
 };
 //类的私有函数无法被调用 执行try_pop()时必然会持有head_mut，进而会持有tail_mut; 多线程并发执行try_pop()没有问题
 // 多线程执行push_new/try_pop，不会发生死锁 
 //但是还是老问题 线程阻塞时什么也不能干 所以下面改用条件控制 一旦有数据进入就可以来取出
 //智能指针的赋值不会引发异常 即使创建失败也不会污染数据
//下面是最终的并发线程安全列队
//std::queue封装
template<typename T>
class threadsafe_queue_ptr
{
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue_ptr()
    {}
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data_queue.empty(); });
        value = std::move(*data_queue.front());    //⇽-- - 1
            data_queue.pop();
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(*data_queue.front());   // ⇽-- - 2
            data_queue.pop();
        return true;
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data_queue.empty(); });
        std::shared_ptr<T> res = data_queue.front();   // ⇽-- - 3
            data_queue.pop();
        return res;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res = data_queue.front();   // ⇽-- - 4
            data_queue.pop();
        return res;
    }
    void push(T new_value)
    {
        std::shared_ptr<T> data(
            std::make_shared<T>(std::move(new_value)));   // ⇽-- - 5
            std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};


/*非std::queue进行封装的线程安全队列有一个空，每当有数据插入时，先将数据赋值进空结点，然后再创造一个空结点，由尾指针指向，即上述创造的队列，屁股后面始终有一个空结点，插入时
先进行数据赋值给空结点，然后在创造空结点接在屁股后面。*/

//非std::queue封装
template<typename T>
class threadsafe_queue_ht
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;//凡是涉及有改动数据或者结构的 都注意要进行上锁
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()   
    {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);//往前移动
        return old_head;
    }
        std::unique_lock<std::mutex> wait_for_data()   
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock,[&] {return head.get() != get_tail(); }); //说明有数据
        return std::move(head_lock);   
    }//这里移动锁 保证是同一个锁 从而确保数据一致
        std::unique_ptr<node> wait_pop_head()
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data());   
                return pop_head();
        }
        std::unique_ptr<node> wait_pop_head(T& value)
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data());  
                value = std::move(*head->data);
            return pop_head();
        }


        std::unique_ptr<node> try_pop_head()
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail())
            {
                return std::unique_ptr<node>();
            }
            return pop_head();
        }
        std::unique_ptr<node> try_pop_head(T& value)
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail())
            {
                return std::unique_ptr<node>();
            }
            value = std::move(*head->data);
            return pop_head();
        }
public:

    threadsafe_queue_ht() :  
        head(new node), tail(head.get())
    {}

    threadsafe_queue_ht(const threadsafe_queue_ht& other) = delete;
    threadsafe_queue_ht& operator=(const threadsafe_queue_ht& other) = delete;

    std::shared_ptr<T> wait_and_pop() 
    {
        std::unique_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    void wait_and_pop(T& value)  //  <------4
    {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
    }


    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        if(old_head){
            return false;
        }else{
            return true;
        }
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }

    void push(T new_value)  //<------2
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
            {
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
            }
            data_cond.notify_one();
    }
};


//同理的线程安全的栈
template<typename  T>
class threadsafe_stack_waitable
{
private:
    std::stack<T> data;
    mutable std::mutex m;
    std::condition_variable cv;
public:
    threadsafe_stack_waitable() {}

    threadsafe_stack_waitable(const threadsafe_stack_waitable& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    }

    threadsafe_stack_waitable& operator=(const threadsafe_stack_waitable&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));    // ⇽-- - 1
        cv.notify_one();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]()  
            {
                if(data.empty())
                {
                    return false;
                }
                return true;
            }); //  ⇽-- - 2
//注意 这里捕获的为this 这样就可以调用该类成员的成员函数了 而不是捕获为空

        std::shared_ptr<T> const res(
            std::make_shared<T>(std::move(data.top())));   // ⇽-- - 3
        data.pop();   // ⇽-- - 4
        return res;
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]()
            {
                if (data.empty())
                {
                    return false;
                }
                return true;
            });

        value = std::move(data.top());   // ⇽-- - 5
        data.pop();   // ⇽-- - 6
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty())
        {
            return false;
        }

        value = std::move(data.top());
        data.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty())
        {
            return std::shared_ptr<T>();
        }

        std::shared_ptr<T> res(std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }

};




//环形列队的实现 锁的形式

template<typename T, size_t Cap>
class CircularQueLk :private std::allocator<T> {
public:
    CircularQueLk() :_max_size(Cap + 1),_data(std::allocator<T>::allocate(_max_size)), _head(0), _tail(0) {}
    CircularQueLk(const CircularQueLk&) = delete;
    CircularQueLk& operator = (const CircularQueLk&) volatile = delete;
    CircularQueLk& operator = (const CircularQueLk&) = delete;

    ~CircularQueLk() {
        //循环销毁
        std::lock_guard<std::mutex>  lock(_mtx);
        //调用内部元素的析构函数
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);//开始进行偏移，逐个销毁，刚开始时，head=0
            _head = (_head+1)% _max_size;
        } //std::allocator类调用destory不会进行回收空间 这个要注意
        //调用回收操作
        std::allocator<T>::deallocate(_data, _max_size);//轮训一圈后 回到head位置
    }

    //先实现一个可变参数列表版本的插入函数最为基准函数（插入一个元素）
    template <typename ...Args>
    bool emplace(Args && ... args) {
        std::lock_guard<std::mutex> lock(_mtx);
        //判断队列是否满了
        if ((_tail + 1) % _max_size == _head) {
            std::cout << "circular que full ! " << std::endl;
            return false;
        }
        //在尾部位置构造一个T类型的对象，构造参数为args... 指针先移动到tail上
        std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);
        //更新尾部元素位置
        _tail = (_tail + 1) % _max_size;
        return true;
    }
    //allocate是先创建空间，再插入元素
    //push 实现两个版本，一个接受左值引用，一个接受右值引用

    //接受左值引用版本
    bool push(const T& val) {
        std::cout << "called push const T& version" << std::endl;
        return emplace(val);
    }

    //接受右值引用版本，当然也可以接受左值引用，T&&为万能引用
    // 但是因为我们实现了const T&
    bool push(T&& val) {
        std::cout << "called push T&& version" << std::endl;
        return emplace(std::move(val));
    }

    //出队函数
    bool pop(T& val) {
        std::lock_guard<std::mutex> lock(_mtx);
        //判断头部和尾部指针是否重合，如果重合则队列为空
        if (_head == _tail) {
            std::cout << "circular que empty ! " << std::endl;
            return false;
        }
        //取出头部指针指向的数据
        val = std::move(_data[_head]);
        //更新头部指针
        _head = (_head + 1) % _max_size;
        return true;
    }
private:
    size_t _max_size;
    T* _data;
    std::mutex _mtx;
    size_t _head = 0;
    size_t _tail = 0;
};


//下面是基于原子变量的无锁列队 利用compare_exchange_strong（和期望值匹配 改变其值返回true,不匹配返回false）

template<typename T, size_t Cap>
class CircularQueSeq :private std::allocator<T> {
public:
    CircularQueSeq() :_max_size(Cap + 1), _data(std::allocator<T>::allocate(_max_size)), _atomic_using(false),_head(0), _tail(0) {}
    CircularQueSeq(const CircularQueSeq&) = delete;
    CircularQueSeq& operator = (const CircularQueSeq&) volatile = delete;
    CircularQueSeq& operator = (const CircularQueSeq&) = delete;

    ~CircularQueSeq() {
        //循环销毁
        bool use_expected = false;
        bool use_desired = true;
        do//类似于自旋锁
        {
            use_expected = false;
            use_desired = true;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
        //调用内部元素的析构函数
        while (_head != _tail) {
            std::allocator<T>::destroy(_data + _head);
            _head = (_head+1)% _max_size;
        }
        //调用回收操作
        std::allocator<T>::deallocate(_data, _max_size);

        do
        {
            use_expected = true;
            use_desired = false;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
    }

    //先实现一个可变参数列表版本的插入函数最为基准函数
    template <typename ...Args>
    bool emplace(Args && ... args) {

        bool use_expected = false;
        bool use_desired = true;
        do
        {
            use_expected = false;
            use_desired = true;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));

        //判断队列是否满了
        if ((_tail + 1) % _max_size == _head) {
            std::cout << "circular que full ! " << std::endl;
            do
            {
                use_expected = true;
                use_desired = false;
            }
            while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));
            return false;
        }
        //在尾部位置构造一个T类型的对象，构造参数为args...
        std::allocator<T>::construct(_data + _tail, std::forward<Args>(args)...);//_data先移动到tail位置
        //更新尾部元素位置
        _tail = (_tail + 1)% _max_size;

        do
        {
            use_expected = true;
            use_desired = false;
        }
        while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));//解锁

        return true;
    }

    //push 实现两个版本，一个接受左值引用，一个接受右值引用

    //接受左值引用版本
    bool push(const T& val) {
        std::cout << "called push const T& version" << std::endl;
        return emplace(val);
    }

    //接受右值引用版本，当然也可以接受左值引用，T&&为万能引用
    // 但是因为我们实现了const T&
    bool push(T&& val) {
        std::cout << "called push T&& version" << std::endl;
        return emplace(std::move(val));
    }

    //出队函数
    bool pop(T& val) {

        bool use_expected = false;
        bool use_desired = true;
        do
        {
              use_expected = false;
              use_desired = true;
        } while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));//不匹配将期望值修改  
        //判断头部和尾部指针是否重合，如果重合则队列为空
        if (_head == _tail) {
            std::cout << "circular que empty ! " << std::endl;
            do
            {
                use_expected = true;
                use_desired = false;
            }
            while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));//这种情况解锁
            return false;
        }
        //取出头部指针指向的数据
        val = std::move(_data[_head]);
        //更新头部指针
        _head = (_head + 1) % _max_size;

        do
        {
            use_expected = true;
            use_desired = false;
        }while (!_atomic_using.compare_exchange_strong(use_expected, use_desired));//解锁  
        return true;
    }
private:
    size_t _max_size;
    T* _data;
    std::atomic<bool> _atomic_using;
    size_t _head = 0;
    size_t _tail = 0;
};

//注意原子操作的无锁结构调用 变量初始化方式相反 如上



//下面是多线程安全的散列表
//并发散列表
//typename告诉编译器后面的东西是一个合法的类型，让其可以通过编译阶段，因为只有在运行期间才能模板实例化，从而推到出合法类型究竟是什么类型
//typedef来重命名类型
template<typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table {
private:
    class bucket_type
    {
    private:
        friend class threadsafe_lookup_table;
        //存储元素的类型为pair，由key和value构成
        typedef std::pair<Key, Value> bucket_value;
        //由链表存储元素构
        typedef std::list<bucket_value> bucket_data;
        //链表的迭代器
        typedef typename bucket_data::iterator bucket_iterator;
        //链表数据
        bucket_data data;
        //改用共享锁
        mutable std::shared_mutex mutex;
        //查找key值，找到返回对应的value，未找到则返回默认值
        bucket_iterator find_entry_for(const Key& key)
        {
            return std::find_if(data.begin(), data.end(),
                [&](bucket_value const& item)
                {return item.first == key; });
        }
    public:
        //查找key值，找到返回对应的value，未找到则返回默认值
        Value value_for(Key const& key, Value const& default_value)
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry = find_entry_for(key);
            return (found_entry == data.end()) ?
                default_value : found_entry->second;
        }
        //添加key和value，找到则更新，没找到则添加
        void add_or_update_mapping(Key const& key, Value const& value)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry = find_entry_for(key);
            if (found_entry == data.end())
            {
                data.push_back(bucket_value(key, value));
            }
            else
            {
                found_entry->second = value;
            }
        }
        //删除对应的key
        void remove_mapping(Key const& key)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry = find_entry_for(key);
            if (found_entry != data.end())
            {
                data.erase(found_entry);
            }
        }
    };
private:
        std::vector<std::unique_ptr<bucket_type>> buckets;
        //hash<Key> 哈希表 用来根据key生成哈希值
        Hash hasher;

        //根据key生成数字，并对桶的大小取余得到下标，根据下标返回对应的桶智能指针
        bucket_type& get_bucket(Key const& key) const
        {
            std::size_t const bucket_index = hasher(key) % buckets.size();
            return *buckets[bucket_index];
        }
   


public:
    threadsafe_lookup_table(
        unsigned num_buckets = 19, Hash const& hasher_ = Hash()) :
        buckets(num_buckets), hasher(hasher_)
    {
        for (unsigned i = 0; i < num_buckets; ++i)
        {
            buckets[i].reset(new bucket_type);
        }
    }

    threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
    threadsafe_lookup_table& operator=(
        threadsafe_lookup_table const& other) = delete;

    Value value_for(Key const& key,
        Value const& default_value = Value())
    {
        return get_bucket(key).value_for(key, default_value);
    }

    void add_or_update_mapping(Key const& key, Value const& value)
    {
        get_bucket(key).add_or_update_mapping(key, value);
    }

    void remove_mapping(Key const& key)
    {
        get_bucket(key).remove_mapping(key);
    }

    std::map<Key, Value> get_map()
    {
        std::vector<std::unique_lock<std::shared_mutex>> locks;
        for (unsigned i = 0; i < buckets.size(); ++i)
        {
            locks.push_back(
                std::unique_lock<std::shared_mutex>(buckets[i]->mutex));
        }
        std::map<Key, Value> res;
        for (unsigned i = 0; i < buckets.size(); ++i)
        {
            //需用typename告诉编译器bucket_type::bucket_iterator是一个类型，以后再实例化
            //当然此处可简写成auto it = buckets[i]->data.begin();
            typename bucket_type::bucket_iterator it = buckets[i]->data.begin();
            for (; it != buckets[i]->data.end(); ++it)
            {
                res.insert(*it);
            }
        }
        return res;
    }
};

//下面是并发安全的单向链表

template<typename T>
class threadsafe_list
{
private:
    struct node
    {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node() :
            next()
        {}
        node(T const& value) :
            data(std::make_shared<T>(value))
        {}
    };

    node head;
public:
    threadsafe_list()
    {}

    ~threadsafe_list()
    {
        remove_if([](node const&) {return true; });
    }

    threadsafe_list(threadsafe_list const& other) = delete;
    threadsafe_list& operator=(threadsafe_list const& other) = delete;

    template<typename Predicate>
    void remove_if(Predicate p)
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            if (p(*next->data))
            {
                std::unique_ptr<node> old_next = std::move(current->next);//转移指针控制权 局部作用域释放后自动消失
                current->next = std::move(next->next);//// A B C now是A，要删除B A的指针域重新连接到C 
                next_lk.unlock();
                //这个注意 如过找到了（假设 B满足） 删除B 并且解锁B的互斥量 不是解锁A的互斥量。这时A->C 下一次循环判断C，因为没解锁，所以下一次循环结点A还是锁住的
            }
            else
            {
                lk.unlock();
                current = next;
                lk = std::move(next_lk);
            }
        }
    }
    void push_front(T const& value)//头插
    {
        std::unique_ptr<node> new_node(new node(value));//data被存入 但next调用默认构造函数
        std::lock_guard<std::mutex> lk(head.m);//锁住头部
        new_node->next = std::move(head.next);//移动交付给next(移动构造)
        head.next = std::move(new_node);
    }

    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p)//λ表达式 谓词
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);
        while (node* const next = current->next.get())
        {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            if (p(*next->data))//如过找到 返回 注意是返回智能指针
            {
                return next->data;
            }
            current = next;
            lk = std::move(next_lk);
        }
        return std::shared_ptr<T>();
    }

    template<typename Function>
    void for_each(Function f)
    {
        node* current = &head;
        std::unique_lock<std::mutex> lk(head.m);//该上锁目的是保证能安全获得该节点的下一个结点
        while (node* const next = current->next.get())//当前结点的下一个结点不为空
        {
            std::unique_lock<std::mutex> next_lk(next->m);//上锁当前的下一个结点的结点 不是锁住该for_each线程
            lk.unlock();
            f(*next->data);
            current = next;
            lk = std::move(next_lk);//转移状态，将锁住next_lk的所属权交给lock 下一次循环时就是操作next的next，要安全获得next的next 要保证next上锁
        }
    }
    // A->B->C->D->E
    //哑 因为第一个结点开始为哑巴结点 所以我们的当前结点的下一个结点才是目标判断结点 当前结点是已知的。
    //上述锁的情况如下 先上锁当前结点，其他线程无法进入 来安全获得下一个结点，获得之后以下一个结点的互斥量进行上锁  因为要开始判断 防止其他线程进入
    //结束后 移动当前结点做为下一个结点并且将锁住next_lk的权力交给lock 下一次循环中，当前循环中的下一个结点 ，作为下一次循环的起始结点 要安全获得下一个循环的起始结点的下一个结点 所以要保证
    //其被锁住，所以锁变量要在while外面

};


/*


单线程链表例子

class MyClass
{
public:
    MyClass(int i) :_data(i) {}

    friend std::ostream& operator << (std::ostream& os, const MyClass& mc) {
        os << mc._data;
        return os;
    }
    int GetData() {
        return _data;
    }

private:
    int _data;
};

std::set<int> removeSet;
void TestThreadSafeList()
{

    threadsafe_list<MyClass> thread_safe_list;
    std::thread t1([&]()
        {
            for (unsigned int i = 0; i < 100; i++)
            {
                MyClass mc(i);
                thread_safe_list.push_front(mc);
            }

        });


    std::thread t2([&]()
        {
            for (unsigned int i = 0; i < 100; )
            {

                auto find_res = thread_safe_list.find_first_if([&](auto& mc)
                    {
                        return mc.GetData() == i;//调用这个谓词判断 传入的智能指针是这个值 那么直接删
                    });

                if (find_res == nullptr)//说明要删除的还没有插入 睡一会再检查
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    continue;
                }

                removeSet.insert(i);
                i++;
            }
        });

    t1.join();
    t2.join();
    for (auto i = removeSet.begin(); i != removeSet.end(); i++) {
        std::cout << *i << std::endl;
    }

}


int main() {
    std::shared_ptr<int> a(std::make_shared<int>(4));
    TestThreadSafeList();

}
*/

//无锁栈

template<typename T>
class lock_free_stack
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		node* next;
		node(T const& data_) : //
			data(std::make_shared<T>(data_)),next(nullptr)
		{}
	};
	lock_free_stack(const lock_free_stack&) = delete;
	lock_free_stack& operator = (const lock_free_stack&) = delete;
	std::atomic<node*> head;
	std::atomic<unsigned> threads_in_pop;//记录调用pop函数的个数
	std::atomic<node*> to_be_deleted;//待删列表

	void try_reclaim(node* old_head)//一个原子变量 都是以1时去执行删除结点和删除待删链表，判断两个并发情况
	{
		//1 原子变量判断仅有一个线程进入pop()线程
		if (threads_in_pop == 1)
		{
			//2 当前线程把待删列表取出
			node* nodes_to_delete = to_be_deleted.exchange(nullptr);//---------------------------------
			//3 更新原子变量获取准确状态，判断是否有多线程进行try_reclaim       这里可能原子变量被改变
			if (!--threads_in_pop)//----------------------------------------------------------------------
			{
				//4 如果唯一调用则将待删列表删除
				delete_nodes(nodes_to_delete);
			}
			else if (nodes_to_delete)//多个线程进行try_reclaim  一个线程刚--后为0，另一个线程又--为负，则将其重新加入待删列表
			{
				//5 如果pop还有其他线程调用且待删列表不为空，
				//则将待删列表首节点更新给to_be_deleted
				chain_pending_nodes(nodes_to_delete);
			}
			delete old_head;
		}
		else {//引用计数>1
			//多个线程pop竞争head节点，此时不能删除old_head
			//将其放入待删列表
			chain_pending_node(old_head);
			--threads_in_pop;
		}
	}
	static void delete_nodes(node* nodes)
	{
		while (nodes)
		{
			node* next = nodes->next;
			delete nodes;
			nodes = next;
		}
	}
	void chain_pending_node(node* n)
	{
		chain_pending_nodes(n, n);
	}
	void chain_pending_nodes(node* first, node* last)
	{
		//1 先将last的next节点更新为待删列表的首节点
		last->next = to_be_deleted;
		//2  借循环保证 last->next指向正确
		// 将待删列表的首节点更新为first节点
		while (!to_be_deleted.compare_exchange_weak(
			last->next, first));
	}
	void chain_pending_nodes(node* nodes)
	{
		node* last = nodes;
		//1 沿着next指针前进到链表末端
		while (node* const next = last->next)
		{
			last = next;
		}
		//2 将链表放入待删链表中
		chain_pending_nodes(nodes, last);
	}

public:
	lock_free_stack() {}
	void push(T const& data)
	{
		node* const new_node = new node(data);    
		new_node->next = head.load();    //head初始化为空
		while (!head.compare_exchange_weak(new_node->next, new_node));  //原子更新头结点  先让新建结点指向老的头节点，然后头节点后移动//也可以写do-while形式 
	}
/*之所以采用std::atomic::.compare_exchange_weak,跟新结点只需要最判断是否相等，多线程并发时，肯定有一个是成功操作的，那个成功操作后会更新变量为最新值，其他线程比较失败后，也会将head最新情况复制给next_node->next,确保下一次会更新,即使发生佯败也没关系，始终会跟新为最终结点*/
//故改用weak改小开销
	std::shared_ptr<T> pop() {
		//1 计数器首先自增，然后才执行其他操作
		++threads_in_pop;
		node* old_head = nullptr;
		do {
			//2 加载head节点给旧head存储
			old_head = head.load();
			if (old_head == nullptr) {
				--threads_in_pop;
				return nullptr;  
			}
		} while (!head.compare_exchange_weak(old_head, old_head->next)); // 3    
		//3处 比较更新head为旧head的下一个节点，多个线程并发删除 确保每个线程获得想删除的结点不重复    

		std::shared_ptr<T> res;
		if (old_head)
		{
			// 4 只要有可能，就回收已删除的节点数据
			res.swap(old_head->data);
		}
		// 5 从节点提取数据，而非复制指针
		try_reclaim(old_head);//要删除时，要确保没有其他线程持有该节点
		return res;
	}
};



//上述无锁栈是依靠待删列表进行，只有一个线程使用POP函数，那么就可以删除 下面介绍风险指针-----当有线程在使用它时，就进行标记，标记的指针被称为风险指针，不能删除




const int max_hazard_pointers = 100;
struct hazard_pointer {
	std::atomic<std::thread::id> id;//id是保存调用hp_owner类的线程id,初始化时id调用std::thread::id类的构造函数生成默认初始值
	std::atomic<void*>  pointer;
};


hazard_pointer hazard_pointers[max_hazard_pointers];//风险数组


class hp_owner {
public:
	hp_owner(hp_owner const&) = delete;
	hp_owner operator=(hp_owner const&) = delete;
	hp_owner() :hp(nullptr) {
		for (unsigned i = 0; i < max_hazard_pointers; ++i) {
			std::thread::id  old_id;
			if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id())) {//找到空的风险数组，如果找到了，就拿出去给1111那一步用，然后存入该线程的线程id
				hp = &hazard_pointers[i];
				break;
			}
		}

		if (!hp) {
			throw std::runtime_error("No hazard pointers available");
		}
	}

	std::atomic<void*>& get_pointer() {
		return hp->pointer;
	}

	~hp_owner() {
		hp->pointer.store(nullptr);//析构时储存置为零
		hp->id.store(std::thread::id());//变为默认值
	}
private:
	hazard_pointer* hp;
};

bool outstanding_hazard_pointers_for(void* p)
{
	for (unsigned i = 0; i < max_hazard_pointers; ++i)
	{
		if (hazard_pointers[i].pointer.load() == p)
		{
			return true;//找到风险指针 返回
		}
	}
	return false;
}

//结点回收函数的实现

//1 实现结点类
//待删节点
struct data_to_reclaim {
	void* data;
	std::function<void(void*)> deleter;
	data_to_reclaim* next;
	template<typename T>
	data_to_reclaim(T* p) : data(p), next(0) {}
	~data_to_reclaim() {
		delete(data);
	}
};


std::atomic<data_to_reclaim*> nodes_to_reclaim;//链表头

void add_to_reclaim_list(data_to_reclaim* node) {
	node->next = nodes_to_reclaim.load();
	while (!nodes_to_reclaim.compare_exchange_strong(node->next, node));
}//头插法，以读改写操作进行原子跟新最新的头节点

void delete_nodes_with_no_hazards() {
	data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);//因为是原子操作交换，所以把全局变量的链表结点拿出来后就给本线程使用，其他线程访问时就是空链表
	while (current) {
		data_to_reclaim* const next = current->next;
		if (!outstanding_hazard_pointers_for(current->data)) {//风险数组对全局共享  待删链表种的每一个结点都要去遍历 看是否涉及风险指针。
			delete current;
		}
		else {
			add_to_reclaim_list(current);//在把涉及风险指针的结点拿回去，这里就有涉及到多线程的竞争插入了
		}
		current = next;
	}
}//回收链表的结点删除，发现有风险结点 重新加载到回收链表里面


template <typename T>
class lock_free_stack1 {
private:
	struct node
	{
		std::shared_ptr<T> data;
		node* next;
		node(T const& data_) :
			data(std::make_shared<T>(data_)), next(nullptr)
		{}
	};
	lock_free_stack1(const lock_free_stack1&) = delete;
	lock_free_stack1& operator = (const lock_free_stack1&) = delete;
	std::atomic<node*> head;
	std::atomic<void*>& get_hazard_pointer_for_current_thread() {
		//每个线程都具有自己的风险指针 线程本地变量
		thread_local static hp_owner hazzard;
		return hazzard.get_pointer();
	}

	void reclaim_later(node* old_head) {
		add_to_reclaim_list(new data_to_reclaim(old_head));//创造一个新的待删结点 加入待删列表
	}
public:

	lock_free_stack1(){}
	void push(T const& data)
	{
		node* const new_node = new node(data);
		new_node->next = head.load();    //head初始化为空
		while (!head.compare_exchange_weak(new_node->next, new_node));  //原子更新头结点  先让新建结点指向老的头节点，然后头节点后移动//也可以写do-while形式 
	}

	std::shared_ptr<T> pop()
	{
		//1 从风险列表中获取一个节点给当前线程
		std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
		node* old_head = head.load();
		do
		{
			node* temp;
			do
			{
				temp = old_head;
				hp.store(old_head);//存入，防止被删除
				old_head = head.load();
			}//2 如果old_head和temp不等说明head被其他线程更新了，需重试
			while (old_head != temp);
		}//3 将当前head更新为old_head->next，如不满足则重试
		while (old_head &&
			!head.compare_exchange_strong(old_head, old_head->next));
		// 4一旦更新了head指针，便将风险指针清零
		hp.store(nullptr);
		std::shared_ptr<T> res;
		if (old_head)
		{
			res.swap(old_head->data);
			//5 删除旧有的头节点之前，先核查它是否正被风险指针所指涉
			if (outstanding_hazard_pointers_for(old_head))
			{
				//6 延迟删除
				reclaim_later(old_head);
			}
			else
			{
				//7 删除头部节点
				delete old_head;
			}
			//8 删除没有风险的节点
			delete_nodes_with_no_hazards();
		}
		return res;
	}
	
};

/*
风险数组测试案例

void TestHazardPointer() {
    hazard_pointer_stack<int> hazard_stack;
    std::set<int>  rmv_set;
    std::mutex set_mtx;

    std::thread t1([&]() {
        for (int i = 0; i < 20000; i++) {
            hazard_stack.push(i);
            std::cout << "push data " << i << " success!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        });

    std::thread t2([&]() {
        for (int i = 0; i < 10000;) {
            auto head = hazard_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
        });

    std::thread t3([&]() {
        for (int i = 0; i < 10000;) {
            auto head = hazard_stack.pop();
            if (!head) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            std::lock_guard<std::mutex> lock(set_mtx);
            rmv_set.insert(*head);
            std::cout << "pop data " << *head << " success!" << std::endl;
            i++;
        }
        });

    t1.join();
    t2.join();
    t3.join();

    assert(rmv_set.size() == 20000);
}

*/



/*引用计数去完成无锁并发栈*/
template<typename T>
class single_ref_stack {
public:
    single_ref_stack(){

    }
    ~single_ref_stack() {
        while (pop());
    }

    void push(T const& data) {
        auto new_node = ref_node(data);
        new_node._node_ptr->_next = head.load();
        while (!head.compare_exchange_weak(new_node._node_ptr->_next, new_node));
    }

    std::shared_ptr<T> pop() {
        ref_node old_head = head.load();
        for (;;) {
            //1 只要执行pop就对引用计数+1并更新到head中
            ref_node new_head;

            //
            do {
                new_head = old_head;
                new_head._ref_count += 1;
            } while (!head.compare_exchange_weak(old_head, new_head));//此处能保证线程为最新head指的方向且引用数最新，因为存的是副本(new_head)所以即使判断失败也无关紧要  

            old_head = new_head;//更新计数
            //3  在这之前 所有线程都是指向相同的头节点且ref_count=到这里的所有线程数量+1
            auto* node_ptr = old_head._node_ptr;
            if (node_ptr == nullptr) {
                return  std::shared_ptr<T>();
            }

            //4 比较head和old_head相等则交换否则说明head已经被其他线程更新，失败则for循环重新更新，注意，是head往前移 但是head前面的指针不一定删除，下面判断删除一定能确保能够删除
            if (head.compare_exchange_strong(old_head, node_ptr->_next)) {

                //要返回的值
                std::shared_ptr<T> res;
                //交换智能指针
                res.swap(node_ptr->_data);

                //5  增加的数量  
                int increase_count = old_head._ref_count - 2;//3
                //6  
                if (node_ptr->_dec_count.fetch_add(increase_count) == -increase_count) {
                    delete node_ptr;
                }

                return res;//无论是否删除 都进行返回数据。
            }
            else {
                //7
                if (node_ptr->_dec_count.fetch_sub(1) == 1) {//此判断是当前线程是最后持有该节点的线程，其引用计数必定1。
                    delete node_ptr;//-2
                }
            }
        }
    }/*线程调用pop()函数操作该界定之后 不会再操作该结点，因为head已经被读改写操作更新 这个要注意！！！！*/

private:
    //结点
    struct ref_node;
    struct node {//内层结点
        //1 数据域智能指针
        std::shared_ptr<T>  _data;
        //2  下一个节点
        ref_node _next;
        node(T const& data_) : _data(std::make_shared<T>(data_)) {}

        //减少的数量
        std::atomic<int>  _dec_count;
    };

    struct  ref_node {//外层结点
        // 引用计数
        int _ref_count;//外层计数

        node* _node_ptr;//包裹指针
        ref_node(T const& data_) :_node_ptr(new node(data_)), _ref_count(1) {}

        ref_node() :_node_ptr(nullptr), _ref_count(0) {}
    };
    std::atomic<ref_node> head;//不是指针类
};
//


/*再次回忆,std::memory_order_acquire与std::memory_order_release构成同步，即在release之前的指令，CPU不会将其排列在其后面 同理std::memory_order_acquire后面的指令不会排在其前面*/

/*std::memory_order_relax对同一变量的原子操作，同一单线程是happen_before*/

/*在release和acquire之间 的读该写操作的内存序列可以是任意的，可以构成释放序列*/
/*除了relax顺序对同一原子变量操作后，不同线程之间不一定是可见的，其他的内存顺序对同一原子变量操作后是，不同线程是能可见的

*/


//非std::memory_order_cst内存序列的无锁并发栈

template<typename T>
class lock_free_stack
{
private:
 struct node;
 struct counted_node_ptr {
  int external_count;
  node* ptr;
 };

 struct node {
  std::shared_ptr<T> data;
  std::atomic<int> internal_count;
  counted_node_ptr next;
  node(T const& data_):data(std::make_shared<T>(data_)),internal_count(0){}
 };
 std::atomic<counted_node_ptr> head;

 void increase_head_count(counted_node_ptr& old_counter) {
  counted_node_ptr new_node;
  do {
	  new_node = old_counter;
	  new_node.external_count++;
  } while (!head.compare_exchange_strong(old_counter, new_node,std::memory_order_acquire,std::memory_order_relaxed));
  old_counter.external_count = new_node.external_count;/*注意函数传的是副本的引用，如果比较成功 同时更新副本和真的头结点的引用计数*/
  /*/*更新引用计数和确保头节点最新*/
 }
public:
 ~lock_free_stack() {
  while (pop());
 }
 void push(T const& data) {
  counted_node_ptr new_node;
  new_node.ptr = new node(data);//内外层 外层结点的node是内结点，内结点的next才是指针域
  new_node.external_count = 1;
  new_node.ptr->next = head.load(std::memory_order_relaxed);
  while (!head.compare_exchange_weak(new_node.ptr->next, new_node, std::memory_order_release, std::memory_order_relaxed));//比较成功用release内存序列，失败用relaex
 }
 std::shared_ptr<T> pop() {

  counted_node_ptr old_head = head.load(std::memory_order_relaxed);

  for (;;) {
	  increase_head_count(old_head);//同步更新副本和真的头节点的引用计数,不同线程进来 构成释放序列，多线程并发POP 执行完毕这一步是指向同一个头节点，这个要注意
	  node* const ptr = old_head.ptr;//node里面还存着外界点
	  if (!ptr) {
		  return std::shared_ptr<T>();
	  }

	  if (head.compare_exchange_strong(old_head,ptr->next,std::memory_order_relaxed) ){
		  std::shared_ptr<T> res;//成功进来的线程执行 为顺序序列，所以可以用relaxed
		  res.swap(ptr->data);
		  int const count_increase = old_head.external_count - 2;
		  if (ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) {
			  delete ptr;
		  }
		  return res;
	  }
	  else if (ptr->internal_count.fetch_sub(1,std::memory_order_acquire) == 1) {
		  delete ptr;
	  }
  }
 }

};
/*回忆 release和acquice中，所有操作都是都是读改写且任意内存序列，都能构成同步    还有释放序列的同步关系*/
/*
int main() {
 lock_free_stack<int> stack;
 std::thread m1([&]() {
  for (int i = 0; i < 100; i++) {
	  stack.push(i);
  }
  });
 std::thread m2([&]() {
  for (int i = 0; i < 100; i++) {
	  auto c = stack.pop();
	  std::cout << *(c.get()) << std::endl;
  }
  });
 m1.join();
 m2.join();

}
*/
  



//智能指针不用进行复制为NULL，其会调用构造函数 初始化为NULL。

//无锁并发队列，用uniptr+引用计数模拟智能指针。(过于困难)

template<typename T>
class lock_free_queue
{
private:

    struct node_counter
    {
        unsigned internal_count : 30;
        //⇽--- 2
        unsigned external_counters : 2;
    };

    struct node;

    struct counted_node_ptr
    {
        //存在破坏trivial class 的风险
        /*bool operator == (const counted_node_ptr& cnp) {
            return (external_count == cnp.external_count && ptr == cnp.ptr);
        }*/

        //构造初始化各成员
        counted_node_ptr() :external_count(0), ptr(nullptr) {}
        int external_count;
        node* ptr;
    };

    struct node
    {
        std::atomic<T*> data;
        std::atomic<node_counter> count;
        //⇽---  1
        std::atomic<counted_node_ptr> next;

        node(int external_count = 2)
        {
            node_counter new_count;
            new_count.internal_count = 0;
            //⇽---  4
            new_count.external_counters = external_count;
            count.store(new_count);

            counted_node_ptr node_ptr;
            node_ptr.ptr = nullptr;
            node_ptr.external_count = 0;

            next.store(node_ptr);
        }


        void release_ref()
        {
            std::cout << "call release ref " << std::endl;
            node_counter old_counter =
                count.load(std::memory_order_relaxed);
            node_counter new_counter;
            do
            {
                new_counter = old_counter;
                //1
                --new_counter.internal_count;
            }
            //2
            while (!count.compare_exchange_strong(
                old_counter, new_counter,
                std::memory_order_acquire, std::memory_order_relaxed));
            if (!new_counter.internal_count &&
                !new_counter.external_counters)
            {
                //3
                delete this;
                std::cout << "release_ref delete success" << std::endl;
                destruct_count.fetch_add(1);
            }
        }
    };

    std::atomic<counted_node_ptr> head;
    //⇽--- 1
    std::atomic<counted_node_ptr> tail;

    // ⇽---  1
    void set_new_tail(counted_node_ptr& old_tail,
        counted_node_ptr const& new_tail)
    {
        node* const current_tail_ptr = old_tail.ptr;
        // ⇽---  2  此处仅有一个线程能设置tail为new_tail，失败的会更新old_tail为tail的新值
        //  为防止失败的线程重试导致tail被再次更新所以添加了后面的&&判断
        //如果tail和old_tail不等说明引用计数不同或者tail已经被移动，如果tail已经被移动那么old_tail的ptr和current_tail_ptr不同，则可以直接退出。
        //所以一旦tail被设置为new_tail，那么另一个线程在重试时判断tail和old_tail不等，会修改old_tail, 此时old_tail已经和current_tail不一致了，所以没必要再重试。
       //如不加后续判断， 会造成重复设置newtail，引发多插入节点的问题。
        while (!tail.compare_exchange_weak(old_tail, new_tail) &&
            old_tail.ptr == current_tail_ptr);
        // ⇽---  3
        if (old_tail.ptr == current_tail_ptr)
            //⇽---  4   
            free_external_counter(old_tail);
        else
            //⇽---  5
            current_tail_ptr->release_ref();
    }

    static void free_external_counter(counted_node_ptr& old_node_ptr)
    {
        std::cout << "call  free_external_counter " << std::endl;
        node* const ptr = old_node_ptr.ptr;
        int const count_increase = old_node_ptr.external_count - 2;
        node_counter old_counter =
            ptr->count.load(std::memory_order_relaxed);
        node_counter new_counter;
        do
        {
            new_counter = old_counter;
            //⇽---  1
            --new_counter.external_counters;
            //⇽---  2  
            new_counter.internal_count += count_increase;
        }
        //⇽---  3
        while (!ptr->count.compare_exchange_strong(
            old_counter, new_counter,
            std::memory_order_acquire, std::memory_order_relaxed));
        if (!new_counter.internal_count &&
            !new_counter.external_counters)
        {
            //⇽---  4
            destruct_count.fetch_add(1);
            std::cout << "free_external_counter delete success" << std::endl;
            delete ptr;
        }

    }


    static void increase_external_count(
        std::atomic<counted_node_ptr>& counter,
        counted_node_ptr& old_counter)
    {
        counted_node_ptr new_counter;
        do
        {
            new_counter = old_counter;
            ++new_counter.external_count;
        } while (!counter.compare_exchange_strong(
            old_counter, new_counter,
            std::memory_order_acquire, std::memory_order_relaxed));
        old_counter.external_count = new_counter.external_count;
    }

public:
    lock_free_queue() {

        counted_node_ptr new_next;
        new_next.ptr = new node();
        new_next.external_count = 1;
        tail.store(new_next);
        head.store(new_next);
        std::cout << "new_next.ptr is " << new_next.ptr << std::endl;
    }

    ~lock_free_queue() {
        while (pop());
        auto head_counted_node = head.load();
        delete head_counted_node.ptr;
    }

    void push(T new_value)
    {
        std::unique_ptr<T> new_data(new T(new_value));
        counted_node_ptr new_next;
        new_next.ptr = new node;
        new_next.external_count = 1;
        counted_node_ptr old_tail = tail.load();
        for (;;)
        {
            increase_external_count(tail, old_tail);
            T* old_data = nullptr;
            //⇽---  6
            if (old_tail.ptr->data.compare_exchange_strong(
                old_data, new_data.get()))
            {
                counted_node_ptr old_next;
                counted_node_ptr now_next = old_tail.ptr->next.load();
                //⇽---  7 链接新的节点
                if (!old_tail.ptr->next.compare_exchange_strong(
                    old_next, new_next))
                {
                    //⇽---  8
                    delete new_next.ptr;
                    new_next = old_next;   // ⇽---  9
                }
                set_new_tail(old_tail, new_next);
                new_data.release();
                break;
            }
            else    // ⇽---  10
            {
                counted_node_ptr old_next;
                // ⇽--- 11
                if (old_tail.ptr->next.compare_exchange_strong(
                    old_next, new_next))
                {
                    // ⇽--- 12
                    old_next = new_next;
                    // ⇽---  13
                    new_next.ptr = new node;
                }
                //  ⇽---  14
                set_new_tail(old_tail, old_next);
            }
        }

        construct_count++;
    }


    std::unique_ptr<T> pop()
    {
        counted_node_ptr old_head = head.load(std::memory_order_relaxed);
        for (;;)
        {
            increase_external_count(head, old_head);
            node* const ptr = old_head.ptr;
            if (ptr == tail.load().ptr)
            {
                //头尾相等说明队列为空，要减少内部引用计数
                ptr->release_ref();
                return std::unique_ptr<T>();
            }
            //  ⇽---  2
            counted_node_ptr next = ptr->next.load();
            if (head.compare_exchange_strong(old_head, next))
            {
                T* const res = ptr->data.exchange(nullptr);
                free_external_counter(old_head);
                return std::unique_ptr<T>(res);
            }
            ptr->release_ref();
        }
    }

    static std::atomic<int> destruct_count;
    static std::atomic<int> construct_count;
};

template<typename T>
std::atomic<int> lock_free_queue<T>::destruct_count = 0;

template<typename T>
std::atomic<int> lock_free_queue<T>::construct_count = 0;



/*std::function要求本身所含的函数可以进行拷贝构造，而std::packadged_task<>只允许示例的移动而不可复制，*/
/*线程池*/
 class threadpool{
      private:
       threadpool(const threadpool&)=delete;
       threadpool& operator=(const threadpool&)=delete;
       
       using Task=std::packaged_task<void()>;
       std::atomic<int> thread_num_;//此在最前面
       std::queue<Task> tasks_;
       std::vector<std::thread>  pool_;
       std::atomic<bool> stop_;
       std::mutex cv_mt_;
       std::condition_variable cv_lock_;

   //插入的线程是一个λ表达式，在线程池未停止服务前，一直从队列中窃取任务，无任务则挂起
       void start(){
           for(int i=0;i<thread_num_;i++){
              pool_.emplace_back([this](){
                   while(!this->stop_.load()){
                    Task task;
                    {
                   std::unique_lock<std::mutex> cv_mt(cv_mt_);
                  this->cv_lock_.wait(cv_mt,[this]{
                     return this->stop_.load()||!this->tasks_.empty();//插入任务 当前任务列对为空，线程就挂起，若停止服务 继续往下走
                  });//在并发情况下突然退出，线程是先争夺锁，未取得锁的进行等待，取得锁后，发现退出 然后往下走，依次退出，若有任务未完成，则执行一个任务在进行退出

                  if(this->tasks_.empty())//此情况是 在退出情况下，即stop_=true 停止服务 再次查看是否为空 不为空 把剩下的任务执行完毕
                    return ;
                  task=std::move(this->tasks_.front());
                  this->tasks_.pop();
                    }
                    this->thread_num_--;
                    task();//执行函数
                    this->thread_num_++;
                   }
              });
           }
       }


       void stop(){
        stop_.store(true);
        cv_lock_.notify_all();
        for(auto& td:pool_){
          if(td.joinable()){
            std::cout<<"join thread "<<td.get_id()<<std::endl;
            td.join();
          }
        }
       }
       

      public:
        static threadpool& instance(){
          static threadpool ins;
          return ins;
        }

          threadpool(unsigned int num=std::thread::hardware_concurrency()):stop_(false){
         if(num<=1){
         thread_num_=2;
       }else{
         thread_num_=num;
       }
         start();
       }

          ~threadpool(){
        stop();
      }

      template<class F,class... Args>
       auto commit(F&& f,Args&&... args)->//下面才是函数主题
        std::future<decltype(std::forward<F>(f)(std::forward<Args>(args)...))>{
          using RetType=decltype(std::forward<F>(f)(std::forward<Args>(args)...));//函数和参数都进行原样转发 推断返回类型
          if(stop_.load()){
            return std::future<RetType>{};
          }
          auto task=std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f),std::forward<Args>(args)...));//std::function<RetType()>转换为std::packaged_task<RetType()>对象
            std::future<RetType> ret=task->get_future();
            {
              std::lock_guard<std::mutex> cv_mt(cv_mt_);
             tasks_.emplace([task](){
            (*task());
             });//伪闭包
            }
            cv_lock_.notify_one();
            return ret;
        }
       
    };

/*此版本线程池不能用于执行成员函数*/


//原本的安全队列
template<typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;//这里尾巴不是std::unique_ptr
    std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()   
    {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);//往前移动
        return old_head;
    }
        std::unique_lock<std::mutex> wait_for_data()   
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock,[&] {return head.get() != get_tail(); }); //说明有数据
        return std::move(head_lock);   
    }//因为锁变量是局部的，所以通过移动锁来确保线程之间的锁的一致性
        std::unique_ptr<node> wait_pop_head()
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data()); //获得头锁
                return pop_head();//获得头结点
        }
        std::unique_ptr<node> wait_pop_head(T& value)
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data());  
                value = std::move(*head->data);
            return pop_head();
        }

public:

    threadsafe_queue() :  
        head(new node), tail(head.get())
    {}

    threadsafe_queue(const threadsafe_queue& other) = delete;
    threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

    std::shared_ptr<T> wait_and_pop() 
    {
        std::unique_ptr<node> const old_head = wait_pop_head();
        return old_head->data;
    }

    void wait_and_pop(T& value)  
    {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
    }


    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
         std::unique_ptr<node> const old_head = try_pop_head(value);
         if(old_head){
            return true;
         }else{
            return false;
         }
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }

    void push(T new_value)  
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
            {
        std::unique_ptr<node> p(new node);
        node* const new_tail = p.get();
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
            }
            data_cond.notify_all();
    }

       std::unique_ptr<node> try_pop_head()
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail())
            {    
                return std::unique_ptr<node>();
            }
            return pop_head();
        }
        
        std::unique_ptr<node> try_pop_head(T& value)
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail())
            {
                return std::unique_ptr<node>();
            }
            value = std::move(*head->data);
            return pop_head();
        }
};



/*改装阻塞队列，加上一个exit()函数*/
template<typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node* prev;/*双指针*/
    };

    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;
    std::atomic_bool  bstop;//用于终止

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()   
    {
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        return old_head;
    }

    std::unique_lock<std::mutex> wait_for_data()   
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock,[&] {return head.get() != get_tail() || bstop.load() == true; });//与原来非改装的安全队列相比，队列一个bstop==true(),其作用是在手动停止线程池时，进行安全退出
        return std::move(head_lock);   
    }

        std::unique_ptr<node> wait_pop_head()
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data());  //移动锁 
            if (bstop.load()) {
                return nullptr;/*因为我们想要停止，所以这里返回空*/
            }

                return pop_head();
        }
        std::unique_ptr<node> wait_pop_head(T& value)
        {
            std::unique_lock<std::mutex> head_lock(wait_for_data());  
            if (bstop.load()) {
                return nullptr;/*同理，在手动停止下，唤醒所以挂起的线程，线程都获得nullptr*/
            }
            value = std::move(*head->data);
            return pop_head();
        }

        std::unique_ptr<node> try_pop_head()
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);//有锁 不用转移
            if (head.get() == get_tail())
            {
                return std::unique_ptr<node>();
            }
            return pop_head();
        }
        std::unique_ptr<node> try_pop_head(T& value)
        {
            std::lock_guard<std::mutex> head_lock(head_mutex);
            if (head.get() == get_tail())
            {
                return std::unique_ptr<node>();
            }
            value = std::move(*head->data);
            return pop_head();
        }
public:

    threadsafe_queue() :  // ⇽-- - 1
        head(new node), tail(head.get())
    {}

    ~threadsafe_queue() {
        bstop.store(true);
        data_cond.notify_all();//唤醒所有线程，进行安全退出
    }

    threadsafe_queue(const threadsafe_queue& other) = delete;
    threadsafe_queue& operator=(const threadsafe_queue& other) = delete;

    void Exit() {
        bstop.store(true);
        data_cond.notify_all();
    }//同析构函数

    bool wait_and_pop_timeout(T& value) {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        auto res = data_cond.wait_for(head_lock, std::chrono::milliseconds(100),
                [&] {return head.get() != get_tail() || bstop.load() == true; });/*加上时间限制*/
        if (res == false) {
            return false;
        }

        if (bstop.load()) {
            return false;
        }
   /*这里要进行判断两次*/
        value = std::move(*head->data);    
        head = std::move(head->next);
        return true;
    }

    std::shared_ptr<T> wait_and_pop() //  
    {
        std::unique_ptr<node> const old_head = wait_pop_head();
        if (old_head == nullptr) {
            return nullptr;
        }// bstop=true,退出 给外面返回NULLPTR
        return old_head->data;
    }

    bool  wait_and_pop(T& value)  //  <------4
    {
        std::unique_ptr<node> const old_head = wait_pop_head(value);
        if (old_head == nullptr) {
            return false;
        }
        return true;
    }


    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head = try_pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

    bool try_pop(T& value)
    {
        std::unique_ptr<node> const old_head = try_pop_head(value);
        if (old_head) {
            return true;
        }
        return false;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }

    void push(T new_value) //<------2
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        {
            std::lock_guard<std::mutex> tail_lock(tail_mutex);
            tail->data = new_data;
            node* const new_tail = p.get();
            new_tail->prev = tail;
            tail->next = std::move(p);
            tail = new_tail;//双链表更新
        }

        data_cond.notify_one();
    }


    bool try_steal(T& value) {
        std::unique_lock<std::mutex> tail_lock(tail_mutex,std::defer_lock);
        std::unique_lock<std::mutex>  head_lock(head_mutex, std::defer_lock);
        std::lock(tail_lock, head_lock);
        if (head.get() == tail)
        {
            return false;
        }

        node* prev_node = tail->prev;
        value = std::move(*(prev_node->data));
        tail = prev_node;
        tail->next = nullptr;
        return true;
    }
};




/*future类线程池*/
/*适用于非改装(前面的threadsafe_queue_ht)和改装后的安全队列，也适用于并发成员函数内部*/
class function_war {
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {};
	};
	std::unique_ptr<impl_base> impl;
	template<typename F>
	struct impl_type :impl_base {
		F f;
		impl_type(F&& f_) :f(std::move(f_)) {}//函数转移到此种去进行调用
		void call() {
			f();
		}
	};
public:
	template<typename F>
	function_war(F&& f) :impl(new impl_type<F>(std::move(f))) {}//调用impl_type构造函数
	void operator()() {
		impl->call();//
	}

	function_war() = default;
	function_war(function_war&& other): impl(std::move(other.impl)) {}
	function_war& operator=(function_war&& other){
		impl = std::move(other.impl);
		return *this;
	}

	function_war(function_war const&) = delete;
	function_war& operator=(function_war const&) = delete;

};

class join_threads {
	std::vector<std::thread>& threads;
public:
	explicit join_threads(std::vector<std::thread>& threads_): threads(threads_){}
	~join_threads() {
		for (unsigned long i = 0; i < threads.size(); i++) {
			if (threads[i].joinable()) {
				threads[i].join();
			}
		}
	}

};


class Threadpool {
private:
	std::atomic<bool> done;
	threadsafe_queue<function_war> workqueue;
	std::vector<std::thread> threads;
	join_threads joiner;
	void work_thread() {
		while (!done) {
			function_war task;
			if (workqueue.try_pop_head(task)) {//try_pop_head()是非等待队列 没有任务直接返回空指针（try_pop()），wait_and_pop才是
				task();
			}
			else {
				std::this_thread::yield();
			}
		}
	}

public:

	Threadpool() :done(false), joiner(threads) {
		unsigned const thread_num = std::thread::hardware_concurrency();
		try {
			for (int unsigned i = 0; i < thread_num; i++) {
				threads.push_back(std::thread(&Threadpool::work_thread, this));
			}
		}
		catch (...) {
			done = true;
			throw;
		}
	}

	static Threadpool& instance() {
		static  Threadpool pool;
		return pool;
	}
	~Threadpool()
	{
		done = true;/*调用join_threads的析构函数*/
	}
	/*std::result<functiontype()>在模板会被推到为函数实例f()，不接受参数类型函数，返回值不知道,注意要有括号才能被推到为函数实例 而std::result<function()>::type为其运行结果*/
	/*f()就包装在std::result<functiontype()>中*/
	template<typename FunctionType>//std::result类似于decltype,对类型进行推导，然后存在type内
	std::future<typename std::result_of<FunctionType()>::type>
		submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type;
		std::packaged_task<result_type()> task(std::move(f));   //封装为std::package_task，与std::promise类似 只支持为std::move,
		std::future<result_type> res(task.get_future());
		workqueue.push(std::move(task));//传输task时 调用function_war的构造函数，构造且将函数移动给impl_type中的F f,即std::package_task<result_type()>类(模板推到)，调用function_war a() 重载了括号，其就会进行调用
		return res;//注意队列存储的是function_wrapper，这里是利用task隐式构造了function_wrapper类型的对象。
	}
};



void test(){
  int a=0;
  a++;
  std::cout<<a<<std::endl;
}
/*
int main() {
	auto p=Threadpool::instance().submit(test);
	auto u= Threadpool::instance().submit(test);
	auto g= Threadpool::instance().submit(test); 
}
*/

//可以将返回的get()储存在一个数组中 循环检测 如果满足 则取得其中的值然后pop掉


//最普通的线程池，用于std::function,可以并发类成员函数（std::bind()），改装的安全队列组装
class simple_thread_pool
{
    std::atomic_bool done;//用于控制线程终止，不让他再执行任务
    threadsafe_queue<std::function<void()> > work_queue; 
    std::vector<std::thread> threads; 
    join_threads joiner;    
    void worker_thread()
    {
        while (!done)    
        {
            std::function<void()> task;
           
            if (work_queue.try_pop(task))//非等待队列，无任务直接返回
            {
                task();    
            }
            else
            {
                std::this_thread::yield();    
            }
        }
    }

    simple_thread_pool() :
        done(false), joiner(threads)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&simple_thread_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }
public:
    static simple_thread_pool& instance() {
       static  simple_thread_pool pool;
       return pool;
    }
    ~simple_thread_pool()
    {
        done = true;     
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }
    }
    template<typename FunctionType>
    void submit(FunctionType f)
    {   
        work_queue.push(std::function<void()>(f));//不论是函数还是可调用对象 都封装到std:：function<>中    
    }
};


//std::queue进行封装的普通线程池
class Threadpool {
private:
	Threadpool(const Threadpool&) = delete;
	Threadpool& operator=(const Threadpool&) = delete;
    std::queue<std::function<void()>> tasks;
	std::atomic<int> thread_num_;
	std::vector<std::thread>  pool_;
	std::atomic<bool> stop_;
	std::mutex cv_mt_;
	std::condition_variable cv_lock_;

	void start() {
		for (int i = 0; i < thread_num_; i++) {
			pool_.emplace_back([this]() {
				while (!this->stop_.load()) {
						std::unique_lock<std::mutex> cv_mt(cv_mt_);
						this->cv_lock_.wait(cv_mt, [this] {
							return this->stop_.load() || !this->tasks.empty();
							});//无任务线程挂起

						if (this->tasks.empty())
							return;
						auto task = std::move(this->tasks.front());
						this->tasks.pop();
					 cv_mt.unlock();
					this->thread_num_--;
					task();
					this->thread_num_++;
				}
				});
		}
	}

	void stop() {
		stop_.store(true);
		cv_lock_.notify_all();//全部唤醒，最后执行，没任务直接return,有任务执行完再说
		for (auto& td : pool_) {
			if (td.joinable()) {
				std::cout << "join thread " << td.get_id() << std::endl;
				td.join();
			}
		}
	}


public:
	static Threadpool& instance() {
		static Threadpool ins;
		return ins;
	}

	Threadpool(unsigned int num = std::thread::hardware_concurrency()) :stop_(false) {
		if (num <= 1) {
			thread_num_ = 2;
		}
		else {
			thread_num_ = num;
		}
		start();
	}

	~Threadpool() {
		stop();
	}
	
  template<class F>
    void AddTask(F&& task) {
        {   
            std::lock_guard<std::mutex> locker(cv_mt_);
            tasks.emplace(std::forward<F>(task));
        }
           cv_lock_.notify_one();
    }

};


//条件等待线程池
class notify_thread_pool
{
private:
    void worker_thread()
    {
        while (!done)//done=true,线程池的所有线程停止工作，并且消费队列中所有等待都唤醒获得空指针，然后join()达到安全目的
        {

            auto task_ptr = work_queue.wait_and_pop();//若无数据，则会挂起，而不是立刻返回空指针，利用修改后的队列，只有两种情况返回 要摸有数据，要摸主动退出
            if (task_ptr == nullptr) {
                continue;
            }

            (*task_ptr)();
        }
    }
public:

    static notify_thread_pool& instance() {
        static  notify_thread_pool pool;
        return pool;
    }
    ~notify_thread_pool()
    {
       
        done = true;
        work_queue.Exit();//先在退出时唤醒所有挂起的线程，最后都置为NULL
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            threads[i].join();
        }//安全退出
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>   
        submit(FunctionType f)
    {
        typedef typename std::result_of<FunctionType()>::type result_type;   
            std::packaged_task<result_type()> task(std::move(f));   
            std::future<result_type> res(task.get_future());    
            work_queue.push(std::move(task));/*push时调用构造函数*/    
            return res;   
    }

private:
    notify_thread_pool() :
        done(false), joiner(threads)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try
        {
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&notify_thread_pool::worker_thread, this));
            }
        }
        catch (...)
        {
            done = true;
            work_queue.Exit();//这里唤醒消费队列中所有的挂起线程 都获得nullptr
            throw;
        }
    }

    std::atomic_bool done;
   
    threadsafe_queue<function_war> work_queue;
    
    std::vector<std::thread> threads;
    
    join_threads joiner;
};

//std::function支持可存储、复制和传递，一般用法进行std::bind()绑定后托付给std::function进行管理 搭配使用，std::package只适合移动还有std::promise,std::future,
//std::packaged_task可以包裹在std::function




/*给每一个线程池都分享一个专门的任务队列，而不是都公用一个*/
class parrallen_thread_pool
{
private:

    void worker_thread(int index)
    {
        while (!done)//没任务就进行挂起 有任务就一直轮训抽取任务
        {

            auto task_ptr = thread_work_ques[index].wait_and_pop();//每个线程处理自己的任务
            if (task_ptr == nullptr) {
                continue;
            }

            (*task_ptr)();
        }
    }
public:

    static parrallen_thread_pool& instance() {
        static  parrallen_thread_pool pool;
        return pool;
    }
    ~parrallen_thread_pool()
    {
        //⇽-- - 11
        done = true;
        for (unsigned i = 0; i < thread_work_ques.size(); i++) {
            thread_work_ques[i].Exit();
        }
//这里 唤醒每一个线程对应的消费队列的所有挂起的任务/消费程序（这里用线程有点不恰当）
        for (unsigned i = 0; i < threads.size(); ++i)
        {
            //⇽-- - 9
            threads[i].join();
        }
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
        submit(FunctionType f)
    {
        int index = (atm_index.load() + 1) % thread_work_ques.size();//投递任务时，进行循环投递到每一个任务队列中
        atm_index.store(index);//更新下一次投递下标
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));//外面已经std::bind()了
        std::future<result_type> res(task.get_future());
        thread_work_ques[index].push(std::move(task));//对应INDEX及进行任务投递
        return res;
    }

private:
    parrallen_thread_pool() :
        done(false), joiner(threads), atm_index(0)
    {
        //⇽--- 8
        unsigned const thread_count = std::thread::hardware_concurrency();
        try
        {
            thread_work_ques = std::vector < threadsafe_queue<function_wrapper>>(thread_count);

            for (unsigned i = 0; i < thread_count; ++i)
            {
                //⇽-- - 9
                threads.push_back(std::thread(&parrallen_thread_pool::worker_thread, this, i));//每个线程分别处理自己的任务队列
            }
        }
        catch (...)
        {
            //⇽-- - 10
            done = true;
            for (int i = 0; i < thread_work_ques.size(); i++) {
                thread_work_ques[i].Exit();
            }
            throw;
        }
    }

    std::atomic_bool done;
    //全局队列
    std::vector<threadsafe_queue<function_wrapper>> thread_work_ques;//给每一个线程都分配一个消费队列
    //⇽-- - 2
    std::vector<std::thread> threads;//消费队列数和线程数量一样
    //⇽-- - 3
    join_threads joiner;
    std::atomic<int>  atm_index;//此原子变量是用来轮训给每一个消费队列投递任务的
};


/*当某一个线程消费队列为空时，其会挂起，为了防止挂起，则其可以偷任务*/

class steal_thread_pool
{
private:

    void worker_thread(int index)
    {
        while (!done)
        {
            function_wrapper wrapper;
            bool pop_res = thread_work_ques[index].try_pop(wrapper);
            if (pop_res) {
                wrapper();
                continue;
            }
          //发现任务队列为空 则依次循环进行偷任务，偷了一个就进行停止
            bool steal_res = false;
            for (int i = 0; i < thread_work_ques.size(); i++) {
                if (i == index) {
                    continue;
                }

                steal_res  = thread_work_ques[i].try_pop(wrapper);
                if (steal_res) {
                    wrapper();
                    break;
                }

            }

            if (steal_res) {
                continue;
            }

            std::this_thread::yield();
        }
    }
public:

    static steal_thread_pool& instance() {
        static  steal_thread_pool pool;
        return pool;
    }
    ~steal_thread_pool()
    {
        //⇽-- - 11
        done = true;
        for (unsigned i = 0; i < thread_work_ques.size(); i++) {
            thread_work_ques[i].Exit();
        }

        for (unsigned i = 0; i < threads.size(); ++i)
        {
            //⇽-- - 9
            threads[i].join();//
        }
    }

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
        submit(FunctionType f)
    {
        int index = (atm_index.load() + 1) % thread_work_ques.size();
        atm_index.store(index);
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        thread_work_ques[index].push(std::move(task));
        return res;
    }

private:
    steal_thread_pool() :
        done(false), joiner(threads), atm_index(0)
    {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try
        {
            thread_work_ques = std::vector < threadsafe_queue<function_wrapper>>(thread_count);

            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&steal_thread_pool::worker_thread, this, i));
            }
        }
        catch (...)
        {
            done = true;
            for (int i = 0; i < thread_work_ques.size(); i++) {
                thread_work_ques[i].Exit();
            }
            throw;
        }
    }

    std::atomic_bool done;
    std::vector<threadsafe_queue<function_wrapper>> thread_work_ques;
    std::vector<std::thread> threads;
    join_threads joiner;
    std::atomic<int>  atm_index;
};


//注意拉姆达表达式以特定变量捕获是变量进行拷贝复制进行捕获的，对于删除拷贝复制（=）的类变量不能捕获特定变量，比如原子变量，但是this指针除外
//经常用std::function<>和std::bind()
//注意done的使用，是不在让每个线程抽取任务执行，再加上EXIT函数，让所有挂起的任务都安全结束，当然不要忘了join()线程


//可中断的线程
class interrupt_flag;
thread_local interrupt_flag this_thread_interrupt_flag;

class interrupt_flag
{


	std::atomic<bool> flag;
	std::condition_variable* thread_cond;//注意条件变量是指针类，这个只能是std::unique_lock
	std::condition_variable_any* thread_cond_any;//ANY可以配合各种类型的锁
	std::mutex set_clear_mutex;
public:
	interrupt_flag() :
		thread_cond(0), thread_cond_any(0)
	{}
	void set()
	{
		flag.store(true, std::memory_order_relaxed);
		std::lock_guard<std::mutex> lk(set_clear_mutex);//这是lock_guard,自动析构解锁，所以设置停止标记为true后解锁。
		if (thread_cond)
		{
			thread_cond->notify_all();
		}
		else if (thread_cond_any) {
			thread_cond_any->notify_all();
		}
	}//将进行关联的都进行全部唤醒，且一般只用一个
	bool is_set() const
	{
		return flag.load(std::memory_order_relaxed);
	}
	void set_condition_variable(std::condition_variable& cv)
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);//这里上锁就是为了进行条件变量的关联
		thread_cond = &cv;
	}
	void clear_condition_variable()
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		thread_cond = 0;//解除条件变量
	}


	template<typename Lockable>
	void wait(std::condition_variable_any& cv, Lockable& lk) {//支持任意锁
		struct custom_lock {
			interrupt_flag* self;
			Lockable& lk;/*类成员是引用，传入的时候也要是引用*/
			custom_lock(interrupt_flag* self_, std::condition_variable_any& cond, Lockable& lk_) ://传入的是中断标记
				self(self_), lk(lk_) {//构造函数传来的LK已经是被加锁的了,这样不用担心在检查中断时的条件竞争
				self->set_clear_mutex.lock();
				self->thread_cond_any = &cond;
			}
                //关联条件变量了，所以要加锁，加的是self里面的锁，而不是lk。LK的作用和self里面的锁一起同步，构造函数只对SELF加锁.
                //注意不是lockguard,要自己unlock()才能对互斥量解锁，而解锁发生在析构函数中

			void unlock() {
				lk.unlock();
				self->set_clear_mutex.unlock();
			}/*交给条件变量来用的*/

			void lock() {
				std::lock(self->set_clear_mutex, lk);
			}/*交给条件变量来用的*/

			~custom_lock() {
				self->thread_cond_any = 0;
				self->set_clear_mutex.unlock();
			}//析构进行解锁和解绑
		};

		/*下面四行的作用域中，self的锁一直保持锁的状态*/
        custom_lock cl(this, cv, lk);//------------------------- 1
        interruption_point();//-------------------------2
        cv.wait(cl);//------------------------3 这里的wait函数是自动调用cl的lock(),unlock()函数，自定义的函数，若在此处挂起，lk,self的自己锁被结构体的unlock()都解锁，然后挂起
        //如果接着又唤醒往下走，调用cl的lock（）函数都上锁往下走，析构的时候只解锁self的锁，lk由外部解锁
        interruption_point();//------------------4 
    }
};
//两种情况 第一种是在1之前就发送了notiy信号，即调用了set函数，这是停止标记记为true，执行2处解绑条件变量，然后析构解锁set_clear_mutex
//第二种情况 执行到三处 释放锁进行挂起状态，这是再次等待notiy信号进行后加锁继续往下执行，执行到4处解绑条件变量，然后析构解锁set_clear_mutex
struct clear_cv_on_destruct{
  ~clear_cv_on_destruct();
};//用于解除关联



void interruptible_wait(std::condition_variable& cv,std::unique_lock<std::mutex>&lk){//线程的回调函数去调用
    interruption_point();//先判断是否中断，没有中断进行关联
    this_thread_interrupt_flag.set_condition_variable(cv);
    clear_cv_on_destruct guards;//用于此函数结束时，进行析构解开关联。
    interruption_point();
    cv.wait_for(lk,std::chrono::milliseconds(1));//当被挂起时，如果线程调用interrupt()函数,那么中断标记flag就被记为TRUE，然后唤醒继续往下走,注意这里是有时间限制的
    /*防止死等*/
    interruption_point();
}
/*一般来说，wait和notify操作都是互斥的，且一定有先后顺序且是基于同一个锁的操作*/


template<typename T>
void interruptible_wait(std::future<T>& uf)
{
    while (!this_thread_interrupt_flag.is_set())
    {
        if (uf.wait_for(std::chrono::milliseconds(1)) ==
            std::future_status::ready)
            break;//阻塞1毫秒等待是否完成，完成返回ready,未完成返回timeout
    }
    interruption_point();
}



void interruption_point(){//中断标记被设置为TRUE，直接抛出异常
    if(this_thread_interrupt_flag.is_set()){
        throw thread_interrupted();
    }
}

clear_cv_on_destruct::~clear_cv_on_destruct(){
this_thread_interrupt_flag.clear_condition_variable();//当前线程进行清除关联
}


class thread_interrupted:public std::exception
{
    public:
    thread_interrupted():message("thread interrupted.") {}
    ~thread_interrupted() throw(){
    }
        virtual const char* what() const throw(){
            return message.c_str();
        }
    private:
    std::string message;
};



class interruptible_thread
{
	std::thread internal_thread;//内部封装一个线程，用于可中断
	interrupt_flag* flag;//此标记是用来表示线程可中断
public:
	template<typename FunctionType>
	interruptible_thread(FunctionType f)//可调用对象
	{
		//⇽-- - 2
		std::promise<interrupt_flag*> p;
		//⇽-- - 3
		internal_thread = std::thread([f, &p] {//引用捕获p,std::promise不可拷贝
			p.set_value(&this_thread_interrupt_flag);
			//⇽-- - 4
			f();
			});
		//⇽-- - 5
		flag = p.get_future().get();//获得时，说明线程内部已经获得好了，线程已经启动了    
	}

	void join() {
		internal_thread.join();
	}
	void interrupt()
	{
		if (flag)//判断是Falg不为空
		{
			flag->set();//进行唤醒，置为true
		}
	}
};


/*上述代码要执行 要将interruption_point()分文件分开存放*/

//测试案例


#include <iostream>
std::vector<interruptible_thread> background_threads;
std::mutex mtx1;
std::mutex mtx2;
std::condition_variable cv1;
std::condition_variable_any cv2;
void start_background_processing() {
    background_threads.push_back([]() {
        try {
            std::unique_lock<std::mutex> lock(mtx1);//外部上锁 
            interruptible_wait(cv1, lock);//此函数里面有本地线程，然后进行关联
        }
        catch (std::exception& ex) {
            std::cout << "catch exception is " << ex.what() << std::endl;
        }

    });

    background_threads.push_back([]() {
        try {
            std::unique_lock<std::mutex> lock(mtx2);
            this_thread_interrupt_flag.wait(cv2, mtx2);
        }
        catch (std::exception& ex) {
            std::cout << "catch exception is " << ex.what() << std::endl;
        }

    });
}

int main()
{
    start_background_processing();
    for (unsigned i = 0; i < background_threads.size(); i++) {
        background_threads[i].interrupt();
    }

    for (unsigned i = 0; i < background_threads.size(); i++) {
        background_threads[i].join();
    }
}



//线程池 要么自己封装一个阻塞队列塞进去任务，如果不塞，则涉及线程池的时候要有锁的条件变量

