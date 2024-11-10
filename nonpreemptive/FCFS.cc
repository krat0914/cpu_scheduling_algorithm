#include <iostream>
#include <queue>

#include "../proc.h"

/**
 * FCFS
 *
 * 먼저 들어온 작업들을 순차적으로 처리 (FIFO)
 */

namespace krt {

void FCFS(krt::TaskSet<krt::Task>* ready_queue) {
  std::queue<krt::Task*> q;
  for (int i = 0; i < 10; ++i) {
     q.push(ready_queue->At(i));
  }

  while (!q.empty()) {
    krt::Task* current = q.front();
	q.pop();

	current->Process();
	std::cout << "[INFO] pid : " << current->id << ", cost : " 
	          << current->burst_time << "  terminated" << std::endl;
  }
}

}  // namespace krt

int main() {
  krt::TaskSet<krt::Task> ready_queue;
  ready_queue.GenerateDummyTasks(10);

  krt::FCFS(&ready_queue);
  return 0;
}