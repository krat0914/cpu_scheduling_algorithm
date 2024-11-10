#include <iostream>
#include <queue>

#include "../proc.h"

void FCFS(krt::TaskSet* tasks) {
  std::queue<krt::Task*> q;
  for (int i = 0; i < 10; ++i) {
     q.push(tasks->At(i));
  }

  while (!q.empty()) {
    krt::Task* current = q.front();
	q.pop();

	current->Process();
	std::cout << "[INFO] process(" << current->id << "), cost : " 
	          << current->burst_time << "   terminated" << std::endl;
  }
}

int main() {
  krt::TaskSet tasks;
  tasks.GenerateDummyTasks(10);

  FCFS(&tasks);
  return 0;
}