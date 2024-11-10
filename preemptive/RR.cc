#include <iostream>
#include <queue>

#include "../proc.h"

/**
 * RR
 *
 * 작업들을 일정 시간동안 돌아가며 실행
 */

namespace krt {

void RR(krt::TaskSet<krt::Task>* task_plan) {
  std::queue<krt::Task*> q;
  int task_index = 0;

  const int time_quantum = 20;
  int elapsed_time = 0;

  krt::Task* running = nullptr;
  for (int i = 1; i < 1000000; ++i) {
    // 작업 생성 시간에 맞게 큐에 저장
    while (task_index < krt::dummy_task_size && task_plan->At(task_index)->gen_time <= i) {
      q.push(task_plan->At(task_index++));
    }

    // 실행 중인 작업이 없다면 새로 할당
    if (running == nullptr) {
      if (q.empty()) {
          // 큐가 비었고 모든 작업도 끝났다면 종료
          if (task_index >= krt::dummy_task_size && running == nullptr) {
            std::cout << "All tasks done!...\n";
            break;
        } else continue;  // 큐가 비었지만 남은 작업이 있다면 기다리기
      }
      running = q.front();
      q.pop();
      std::cout << "[INFO] New running process : " << running->id << '\n';

      elapsed_time = 0;
    }

    // 경과 시간이 정해진 시간 단위를 넘었다면 새로운 작업으로 교체
    if (elapsed_time >= time_quantum) {
      if (q.empty()) {
          // 큐가 비었고 모든 작업도 끝났다면 종료
          if (task_index >= krt::dummy_task_size && running == nullptr) {
            std::cout << "All tasks done!...\n";
            break;
        } else continue;  // 큐가 비었지만 남은 작업이 있다면 기다리기
      }
        
      std::cout << "[INFO] Switching from ";
      std::cout <<  running->id;

      q.push(running);
      running = q.front();
      q.pop();
      std::cout << " to " << running->id << '\n';

      elapsed_time = 0;
    }

    running->burst_time--;
    elapsed_time++;
    
    // 작업 완료
    if (running->burst_time <= 0) {
      // 경과 시간을 늘려 강제로 실행 작업을 바꾸기
      elapsed_time = 4;

      std::cout << "[INFO]--------\n  pid : " << running->id << "\n  cost : " 
                << running->total_burst_time << "\n  left time : "
                << running->burst_time << "\n  gen time : "
                << running->gen_time << "\n  terminated\n-------------" << std::endl;
      running = nullptr;
    }
  }
}

}  // namespace krt

int main() {
  krt::TaskSet<krt::Task> task_plan;
  task_plan.GenerateDummyTasks(krt::dummy_task_size);

  krt::RR(&task_plan);
  return 0;
}