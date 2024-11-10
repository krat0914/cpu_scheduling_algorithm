#include <iostream>

#include "../proc.h"

/**
 * SJF
 *
 * 대기 큐에서 실행 시간(burst time)이 가장 작은 작업부터 실행
 * - Starvation(기아 현상) 발생 가능
 * - 실행 시간을 측정하기 어려움
 */

namespace krt {

/**
 * 실행 시간에 따라 정렬하는 힙
 */
class SJFHeap {
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
    while (LeftChild(current) <= size && tasks[LeftChild(current)]->burst_time < tasks[current]->burst_time) {
      int new_current = GetSmallestChild(current);
      SwapTask(current, GetSmallestChild(current));
      current = new_current;
    }

    return top;
  }

  int Empty() const { return size == 0; }

 private:
  inline int GetSmallestChild(int index) { 
    int smallest_child = LeftChild(index);
    if (size >= smallest_child + 1 && 
      tasks[smallest_child]->burst_time > tasks[smallest_child + 1]->burst_time) {
      smallest_child++;
    }
    return smallest_child;
  }
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

void SJF(krt::TaskSet<krt::Task>* task_plan) {
  krt::SJFHeap heap;

  int task_index = 0;
  krt::Task* running = nullptr;       // 실행 중인 작업에 대한 포인터

  int start = 0;
  int end = 0;

  for (int i = 1; i < 1000000; ++i) {
    // 작업 생성 시간에 맞게 힙에 저장
    while (task_index < krt::dummy_task_size && task_plan->At(task_index)->gen_time <= i) {
      heap.Push(task_plan->At(task_index++));
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

  krt::SJF(&task_plan);
  return 0;
}

