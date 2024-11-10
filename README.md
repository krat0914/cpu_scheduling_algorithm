# cpu_scheduling_algorithm

cpu 스케줄링 알고리즘 구현

<br><br>

### 비선점형 스케줄링 알고리즘 (Non-preemptive Scheduling Algorithm)
---
> 하나의 프로세스가 작업을 종료하고 cpu를 반환해야 다음 프로세스가 cpu 점유 가능
- *`Convoy Effects`* : cpu 사용 시간이 짧은 프로세스가 cpu 사용 시간이 긴 프로세스를 기다리는 현상
- *`Starvation`* : cpu 사용 시간이 긴 프로세스가 우선 순위가 뒤로 밀려 계속 실행되지 못하는 현상

<br>

| 이름 | 소스코드 |
|---|---|
| FCFS (First Come First Served) | [FCFS](./nonpreemptive/FCFS.cc) |
| SJF (Shortest Job First) | [SJF](./nonpreemptive/SJF.cc) |
| HRN (Highest Response Ratio Next) | [HRN](./nonpreemptive/HRN.cc) |

<br><br>

### 선점형 스케줄링 알고리즘 (Preemptive Scheduling Agorithm)
---
> 하나의 프로세스가 작업 중이어도 다른 프로세스가 강제로 cpu 점유하는 것이 가능

<br>

| 이름 | 소스코드 |
|---|---|
| RR (Round Robin) | [RR](./preemptive/RR.cc) |
| SRT (Shortest Remaining Time First) | [SRT](./preemptive/SRT.cc) |
| MLQ (Multilevel Queue) | |
| MFQ (Multilevel Feedback Queue) | |