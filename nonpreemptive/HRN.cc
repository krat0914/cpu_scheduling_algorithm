#include <iostream>

#include "../proc.h"

/**
 * HRN
 *
 * SJF의 기아 현상을 해결 (Aging 기법)
 * 대기 시간을 포함한 우선 순위로 판단 (대기 시간 + 실행 시간) / 실행 시간)
 */

namespace krt {

// 순위의 값이 가장 작은 작업의 인덱스를 구함
int GetMinimumPriorityIndex(const std::vector<krt::Task*>& tasks, const std::vector<int>& wait_time) {
  int minimum_index = 0;
  float minimum_priority = 1000000;
  for (int i = 0; i < tasks.size(); ++i) {
	float current_priority = (wait_time[i] + tasks[i]->burst_time) / static_cast<float>(tasks[i]->burst_time);
	if (current_priority < minimum_priority) {
	  minimum_priority = current_priority;
	  minimum_index = i;
	}
  }

  return minimum_index;
}

void HRN(krt::TaskSet<krt::Task>* task_plan) {
  std::vector<krt::Task*> tasks;
  std::vector<int> wait_time;

  tasks.reserve(krt::dummy_task_size);
  wait_time.reserve(krt::dummy_task_size);

  int task_index = 0;

  int start = 0;
  int end = 0;

  int running_task_index = -1;
  krt::Task* running = nullptr;
  for (int i = 1; i < 1000000; ++i) {
    // 작업 생성 시간에 맞게 리스트에 저장
    while (task_index < krt::dummy_task_size && task_plan->At(task_index)->gen_time <= i) {
      tasks.push_back(task_plan->At(task_index++));
	  wait_time.push_back(0);
    }


    // 실행 중인 작업이 없다면 새로 할당
    if (running == nullptr) {
      if (tasks.empty()) {
        // 리스트가 비었고 모든 작업도 끝났다면 종료
        if (task_index >= krt::dummy_task_size) {
            std::cout << "All tasks done!...\n";
            break;
        } else continue;  // 힙은 비었지만 남은 작업이 있다면 기다리기
	  }
	  running_task_index = GetMinimumPriorityIndex(tasks, wait_time);
      running = tasks[running_task_index];
      start = i;
	}

    running->burst_time--;
	
	// 대기 시간 증가
	for (int i = 0; i < wait_time.size(); ++i) {
	  if (i != running_task_index) wait_time[i]++;
	}

    // 작업 완료
    if (running->burst_time <= 0) {
      end = i;
      std::cout << "[INFO]--------\n  pid : " << running->id << "\n  cost : " 
                << running->total_burst_time << "\n  left time : "
                << running->burst_time << "\n  gen time : "
                << running->gen_time << "\n  start, end : " 
                << start << ", " << end << "\n  terminated\n-------------" << std::endl;
      running = nullptr;
      tasks.erase(tasks.begin() + running_task_index);
	  wait_time.erase(wait_time.begin() + running_task_index);
    }
  }
}

}  // namespace krt

int main() {
  krt::TaskSet<krt::Task> task_plan;
  task_plan.GenerateDummyTasks(krt::dummy_task_size);

  krt::HRN(&task_plan);
  return 0;
}