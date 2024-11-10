#ifndef _CPU_SCHEDULING_ALGORITHM_PROC_H_
#define _CPU_SCHEDULING_ALGORITHM_PROC_H_

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <type_traits>

namespace krt {

struct Task {
  Task()
   : id(0), priority(0), gen_time(0), burst_time(0) {}

  Task(int id, int priority, int gen_time, int burst_time) 
   : id(id), priority(priority), gen_time(gen_time), burst_time(burst_time) {}

  virtual void Process() {

    std::cout << "[PROCESSING]-----------\n"                    \
	          << "  pid : " << id	<< '\n'					    \
			  << "  priority : " << priority << '\n'			\
			  << "  gen_time : " << gen_time << '\n'			\
			  << "  burst_time : " << burst_time << '\n'        \
			  << "-----------------------\n";
  }

  int id;
  int priority;
  int gen_time;
  int burst_time;
};

template <typename T, typename = std::enable_if<std::is_base_of<Task, T>::value>>
class TaskSet {
 public:
  TaskSet() {}

  void GenerateDummyTasks(int size) {
	std::srand(static_cast<unsigned int>(std::time(0)));
    tasks.resize(size);
	
	for (int i = 0; i < size; ++i) {
      tasks[i].id = i;
	  tasks[i].priority = std::rand() % 4 + 1;
	  tasks[i].gen_time = std::rand() % 100;
	  tasks[i].burst_time = std::rand() % 100;
	}

	std::sort(tasks.begin(), tasks.end(), TaskSet::compare_gen_time);
  }

  T* At(int index) {
	return &tasks[index];
  }

  std::vector<T>& CloneTasks() {
	return tasks.clone();
  }

 private:
  static int compare_gen_time(const T& a, const T& b) {
    return a.gen_time < b.gen_time;
  }

  std::vector<T> tasks;
};

const int dummy_task_size = 5;


}  // namespace krt

#endif  // _CPU_SCHEDULING_ALGORITHM_PROC_H_
