#include <pthread.h>
#include <stdio.h>
#include <type_traits>
/**
 * 尽可能模拟书上的接口, 但是condition始终与一个mutex绑定, 也就是说,
 * 如果要像书上那样实现condition x, y,
 * 则condition类型必须和一个monitor实例关联,这在C++里几乎是不可能的就算是Qt也没有这么骚气的语法,
 * 而是要每声明一个控件都要设置父指针才可以和一个对象关联。
 * 1. 如果condition是public类,
 * 那么当我在类外Monitor::Condition声明一个实例的时候,这个实例并不知道应该绑定到哪个monitor实例上。
 * 2. 如果condition是private类。 我们希望有这个语法的目的,
 * 就是可以继承Monitor然后用condition a,
 * b,这样简洁的声明。然而如果condition是private了,那我继承的时候自然也没法使用condition了。
 * 所以唯一的办法就是动态绑定,
 * 为Condition提供一个可以用Condition(this)的接口已经是很漂亮了(和Qt一样)
 * 函数也是一样,自己定义的函数不能保证进来之后就加锁解锁,
 * 除非把函数当成一个对象, 继承自一个monitor提供的所有函数的对象。
 *
 * 我已经写傻了,
 * 发现除非改编译器，否则完全没法为了实现这个简单的功能而提供接口,即不能限制程序员继承了monitor之后不加锁的行为。唯一一个限制的方法是:提供run接口,要求程序员必须使用run接口来调用函数。
 */

class Monitor {
 private:
  pthread_mutex_t mutex;
  friend class Condition;  //懒得封装这么多get set了, 直接声明Condition为友元,
                           //且设置为private，继承后的就不能随便乱动了
 public:
  Monitor() { pthread_mutex_init(&mutex, NULL); }
  virtual ~Monitor() { pthread_mutex_destroy(&mutex); }
  template <typename T>
  auto run(T func) -> decltype(func()) {
    pthread_mutex_lock(&mutex);
    if (std::is_void<decltype(func())>::value) {  // todo
      func();
      pthread_mutex_unlock(&mutex);
    } else {
      auto ret = func();
      pthread_mutex_unlock(&mutex);
      return ret;
    }
  }
};

class Condition {
 private:
  pthread_cond_t _cond;
  pthread_mutex_t* _mutex;  //传入monitor的全局锁或其他锁
                            //使用前必须加锁, 可以配合monitor使用
 public:
  Condition(pthread_mutex_t* mutex) : _mutex(mutex) {
    pthread_cond_init(&_cond, NULL);
  }
  Condition(Monitor* monitor) : _mutex(&monitor->mutex) {
    pthread_cond_init(&_cond, NULL);
  }
  virtual ~Condition() { pthread_cond_destroy(&_cond); }
  void wait() { pthread_cond_wait(&_cond, _mutex); }
  void signal() { pthread_cond_signal(&_cond); }
};

int main() {

}
