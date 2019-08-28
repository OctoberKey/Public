#ifndef MY_TIMER_HPP_
#define MY_TIMER_HPP_

#include<chrono>

using namespace std;
using namespace std::chrono;

namespace assistant {

class Timer
{
public:
  Timer() : m_begin(high_resolution_clock::now()) {}
  void reset() { m_begin = high_resolution_clock::now(); }

  //纳秒
  int64_t elapsed_nano() const
  {
    return duration_cast<chrono::nanoseconds>(high_resolution_clock::now() - m_begin).count();
  }

  //微秒
  int64_t elapsed_micro() const
  {
    return duration_cast<chrono::microseconds>(high_resolution_clock::now() - m_begin).count();
  }

  //毫秒
  int64_t elapsed_milli() const
  {
    return duration_cast<chrono::milliseconds>(high_resolution_clock::now() - m_begin).count();
  }

  //秒
  double elapsed_second() const
  {
    return duration_cast<duration<double>>(high_resolution_clock::now() - m_begin).count();
  }


  //分
  int64_t elapsed_minutes() const
  {
    return duration_cast<chrono::minutes>(high_resolution_clock::now() - m_begin).count();
  }

  //时
  int64_t elapsed_hours() const
  {
    return duration_cast<chrono::hours>(high_resolution_clock::now() - m_begin).count();
  }

private:
  time_point<high_resolution_clock> m_begin;
};

} // assistant

#endif // MY_TIMER_HPP_
