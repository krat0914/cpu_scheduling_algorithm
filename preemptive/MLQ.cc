#include <iostream>
#include <queue>

#include "../proc.h"

/**
 * MLQ
 *
 * 우선 순위에 따라 계층별 준비 큐를 생성 후 우선 순위에 따라 일을 처리.
 * 우선 순위에 따라 준비 큐가 따로 존재하고 우선 순위가 낮은 큐가 처리 중일 때 
 * 우선 순위가 높은 큐에 작업이 할당되면 기존 작업을 멈추고 우선 순위가 높은
 * 작업을 우선 처리함.
 * 각 계층별 준비 큐는 RR(Round Robin)이나 FCFS를 따라 스케줄링을 함
 */

namespace krt {

class ReadyQueue {
 public:
  // 1 클록 실행
  virtual void RunTick() = 0;
  // 새로운 작업 추가
  virtual void Push(krt::Task* task) = 0;
  // 큐가 비었는지 확인
  virtual int Empty() const = 0;
};

// FCFS를 이용한 큐
class FCFSReadyQueue : public ReadyQueue {
 public:
  FCFSReadyQueue() : running(nullptr){}

  // q가 비어있지 않다는 전제 하에 1 클록 실행
  virtual void RunTick() override {
    // 실행할 작업이 없다면 새롭게 할당
    if (running == nullptr) {
      running = q.front();
      q.pop();
    }

    std::cout << "[INFO] process : " << running->id           \
              << "   priority : " << running->priority        \
              << "    Running\n";                             
    running->burst_time--;

    // 작업 완료
    if (running->burst_time <= 0) {
      std::cout << "[INFO] process : " << running->id          \
                << "   priority : " << running->priority       \
                << "    Terminated\n";                         
      running = nullptr;
    }
  };

  virtual void Push(krt::Task* task) override {
    q.push(task);
  }

  virtual int Empty() const { return q.empty() && running == nullptr; }

 private:
  krt::Task* running;
  std::queue<krt::Task*> q;
};

// RR을 이용한 큐
class RRReadyQueue : public ReadyQueue {
 public:
  RRReadyQueue(int time_quantum) 
   : elapsed_time(0), time_quantum(time_quantum), running(nullptr) {}

  // q가 비어있지 않다는 전제 하에 1 클록 실행
  virtual void RunTick() override {
    // 실행할 작업이 없다면 새롭게 할당
    if (running == nullptr) {
      running = q.front();
      q.pop();
      elapsed_time = 0;

      std::cout << "[INFO] New running process : " << running->id  \
                << "   priority : " << running->priority           \
                << "    Running\n";
    }

    // 경과 시간이 정해진 시간 단위를 넘었다면 새로운 작업으로 교체
    if (elapsed_time >= time_quantum) {
      q.push(running);
      std::cout << "[INFO] Switich from " << running->id;

      running = q.front();
      q.pop();
      elapsed_time = 0;

      std::cout << " to " << running->id                           \
                << "   priority : " << running->priority << '\n';
    }

    running->burst_time--;
    elapsed_time++;

    // 작업 완료
    if (running->burst_time <= 0) {
      std::cout << "[INFO] process : " << running->id          \
                << "   priority : " << running->priority       \
                << "    Terminated\n";   
      running = nullptr;
    }
  };

  virtual void Push(krt::Task* task) override {
    q.push(task);
  }

  virtual int Empty() const { return q.empty() && running == nullptr; }

 private:
  int elapsed_time = 0;
  int time_quantum;
  krt::Task* running = nullptr;
  std::queue<krt::Task*> q;
};

class MLQScheduler : public ReadyQueue  {
 public:
  void CreateQueues() {
    rq[0] = new krt::RRReadyQueue(2);
    rq[1] = new krt::RRReadyQueue(5);
    rq[2] = new krt::RRReadyQueue(5);
    rq[3] = new krt::FCFSReadyQueue();
  }

  void ReleaseQueues() {
    for (int i = 3; i >= 0; --i) {
      if (rq[i] != nullptr) {
        delete rq[i];
        rq[i] = nullptr;  // 해제 후 nullptr로 설정
      }
    }
  }

  virtual void Push(krt::Task* task) override {
    rq[task->priority - 1]->Push(task);

    // 현재 실행 중인 작업 없다면 새로운 작업을 할당
    if (running_level == -1) {
      running_level = task->priority - 1;
    }

    // 우선 순위가 더 높은 작업이 들어오면 그것으로 실행하는 큐를 변경
    if (running_level < task->priority - 1) {
      running_level = task->priority - 1;
    }
  }

  virtual void RunTick() override {
    // 만약 실행 중인 작업이 없거나 현재 큐가 비었다면 하나 씩 내려가며 작업을 검색
    if (running_level == -1 || rq[running_level]->Empty()) {
      for (int i = 3; i >= 0; --i) {
        if (!rq[i]->Empty()) {
          running_level = i;
          break;
        }
      }
    } 

    // 모든 큐가 빈 것이 아니라면 작업 실행
    if (running_level != -1) {
      rq[running_level]->RunTick();
      // 실행한 작업이 있는 큐가 비었을 경우 다른 큐를 찾기 위해 실행 레벨을 -1로 지정
      if (rq[running_level]->Empty()) running_level = -1;
    }
  }

  virtual int Empty() const { 
    return rq[0]->Empty() &&
           rq[1]->Empty() &&
           rq[2]->Empty() &&
           rq[3]->Empty();
  }

 private:
  int running_level = -1;
  krt::ReadyQueue* rq[4];
};

void MLQ(krt::TaskSet<krt::Task>* task_plan) {
  krt::MLQScheduler mlq;
  mlq.CreateQueues();

  int task_index = 0;
  for (int i = 1; i < 1000000; ++i) {
    // 작업 생성 시간에 맞게 힙에 저장
    while (task_index < krt::dummy_task_size && task_plan->At(task_index)->gen_time <= i) {
      std::cout << "[INFO] New process pushed : " << task_plan->At(task_index)->id 
                << "   priority : " << task_plan->At(task_index)->priority << std::endl;
      mlq.Push(task_plan->At(task_index++));
    }

    // 실행 중인 작업이 없다면 새로 할당
    // 힙이 비었고 모든 작업도 끝났다면 종료
    if (mlq.Empty()) {
      if (task_index >= krt::dummy_task_size) {
        std::cout << "All tasks done!...\n";
        break;
      } else continue;  // 힙은 비었지만 남은 작업이 있다면 기다리기
    }

    mlq.RunTick();
  }

  mlq.ReleaseQueues();
}

}  // namespace krt

int main() {
  krt::TaskSet<krt::Task> task_plan;
  task_plan.GenerateDummyTasks(krt::dummy_task_size);

  krt::MLQ(&task_plan);
  return 0;
}