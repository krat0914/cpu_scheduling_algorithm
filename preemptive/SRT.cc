#include <iostream>

#include "../proc.h"

/**
 * SRT
 *
 * SJF의 선점형 버전
 * 기존 SJF는 실행 중인 프로세스가 완료된 후 다음 프로세스가 실행됐지만 
 * SRT는 실행 중인 프로세스도 포함해 실행 시간(burst time)을 비교한 후 실행 시간이 가장 짧은 
 * 프로세스가 cpu에 할당된다.
 */

namespace krt {

/**
 * 실행 시간에 따라 정렬하는 힙 (SJF의 힙과 동일)
 */
class SRTHeap {
 public:
  void Push(krt::Task* task) {
    tasks[++size] = task;

    int current = size;
    while (current > 1 && tasks[Parent(current)]->burst_time > tasks[current]->burst_time) {
      SwapTask(Parent(current), current);
      current = Parent(current);
    }
  }

  krt::Task* Pop() {
    krt::Task* top = tasks[1];
    tasks[1] = tasks[size--];

    int current = 1;
    while (LeftChild(current) <= size) {
      int new_current = LeftChild(current);

      if (RightChild(current) <= size &&
          tasks[LeftChild(current)]->burst_time > tasks[RightChild(current)]->burst_time) {
        new_current = RightChild(current);
      }
      if (new_current > size || tasks[current]->burst_time < tasks[new_current]->burst_time) break;

      SwapTask(current, new_current);
      current = new_current;
    }

    return top;
  }

  int Empty() const { return size == 0; }

 private:
  inline void SwapTask(int index1, int index2) {
    krt::Task* temp = tasks[index1];
    tasks[index1] = tasks[index2];
    tasks[index2] = temp;
  }
  inline int Parent(int index) { return index / 2; }
  inline int RightChild(int index) { return index * 2 + 1; }
  inline int LeftChild(int index) { return index * 2; }

  krt::Task* tasks[100];
  size_t size = 0;
};

void SRT(krt::TaskSet<krt::Task>* task_plan) {
  krt::SRTHeap heap;

  int task_index = 0;
  krt::Task* running = nullptr;       // 실행 중인 작업에 대한 포인터

  int start = 0;
  int end = 0;

  for (int i = 1; i < 1000000; ++i) {
    // 작업 생성 시간에 맞게 힙에 저장
    while (task_index < krt::dummy_task_size && task_plan->At(task_index)->gen_time <= i) {
      // 새로운 작업이 생길 때마다 실행 시간아 가장 적은 작업을 판단한다.
      // 이전에 이미 판단된 작업들은 현재 실행 중인 작업보다 실행 시간이 길기에 
      // 지금 실행할 작업을 판단하기 위해선 새로운 작업과 실행 중인 작업만 비교하면 된다.
      
      // 실행 중인 작업이 있고 새로운 작업이 실행 시간이 더 적다면 교체
      if (running != nullptr && task_plan->At(task_index)->burst_time < running->burst_time) {
        heap.Push(running);
        std::cout << "[INFO] Switching from " << running->id;
        running = task_plan->At(task_index++);
        std::cout << " to " << running->id << '\n';
      } else {
        heap.Push(task_plan->At(task_index++));
      }
    }

    // 실행 중인 작업이 없다면 새로 할당
    if (running == nullptr) {
      if (heap.Empty()) {
        // 힙이 비었고 모든 작업도 끝났다면 종료
        if (task_index >= krt::dummy_task_size && running == nullptr) {
            std::cout << "All tasks done!...\n";
            break;
        } else continue;  // 힙은 비었지만 남은 작업이 있다면 기다리기
      }

      // 새로운 작업 할당
      running = heap.Pop();
      start = i;
    }

    running->burst_time--;

    // 작업 완료
    if (running->burst_time <= 0) {
      end = i;
      std::cout << "[INFO]--------\n  pid : " << running->id << "\n  cost : " 
                << running->total_burst_time << "\n  left time : "
                << running->burst_time << "\n  gen time : "
                << running->gen_time << "\n  start, end : " 
                << start << ", " << end << "\n  terminated\n-------------" << std::endl;
      running = nullptr;
    }
  }
}

}  // namespace krt

int main() {
  krt::TaskSet<krt::Task> task_plan;
  task_plan.GenerateDummyTasks(krt::dummy_task_size);

  krt::SRT(&task_plan);
  return 0;
}