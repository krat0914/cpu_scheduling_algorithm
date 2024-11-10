# cpu_scheduling_algorithm

cpu 스케줄링 알고리즘 구현

### **CPU 스케줄링**
- CPU에 새로운 작업을 할당할 때 사용
- *`Convoy Effects`* : cpu 사용 시간이 짧은 프로세스가 cpu 사용 시간이 긴 프로세스를 기다리는 현상
- *`Starvation`* : cpu 사용 시간이 긴 프로세스가 우선 순위가 뒤로 밀려 계속 실행되지 못하는 현상

<br>

### **Burst**
- 작업을 처리하는 시간
- `CPU Burst` : CPU로 명령어를 처리하는 시간 (연산, I/O 요청)
- `IO Burst` : 입출력을 실행하는 시간
- `CPU Bound` : `CPU Burst`가 큰 작업
- `IO Bound` : `IO Burst`가 큰 작업
- 일반적으로 입출력이 오래 걸리기에 `IO Burst`가 더 큼

<br>

### **발생 상황**
1. `Running` -> `Waiting`
2. `Running` -> `Ready`
3. `Waiting` -> `Ready`
4. `Terminated`

> 1, 4 : **Non-preemptive**<br>
> 2, 3 : **Non-preemptive** or **Preemptive**

<br>

### **CPU Schduler & Dispatcher**
- `CPU Schduler` : CPU를 점유할 작업을 선정
- `Dispatcher` : 선정된 작업을 CPU에 할당 (`Context Switching`)
- `Dispatcher Latency` : `Dispatch`를 할 때 소모되는 시간

<br>

### **선택 기준**
- `CPU 사용률(CPU Utilization)` : CPU가 안 쉬고 일하는 시간
- `처리량(Thoughput)` : 단위 시간 내 마치는 작업의 수
- `대기 시간(Waiting Time)` : 작업가 생성되고 대기하는 시간
- `응답 시간(Response Time)` : 작업이 CPU를 할당받기까지 최초로 걸린 시간(ex UI)
- `반환 시간(Turnaround Time)` : 작업가 종료될 때까지 걸리는 시간
- `평균 대기 시간` : 작업들의 대기 시간의 평균

<br><br>

### 비선점형 스케줄링 알고리즘 (Non-preemptive Scheduling Algorithm)
> 하나의 프로세스가 작업을 종료하고 cpu를 반환해야 다음 프로세스가 cpu 점유 가능

<br>

| 이름 | 소스코드 |
|---|---|
| FCFS (First Come First Served) | [FCFS](./nonpreemptive/FCFS.cc) |
| SJF (Shortest Job First) | [SJF](./nonpreemptive/SJF.cc) |
| HRN (Highest Response Ratio Next) | [HRN](./nonpreemptive/HRN.cc) |

<br><br>

### 선점형 스케줄링 알고리즘 (Preemptive Scheduling Agorithm)
> 하나의 프로세스가 작업 중이어도 다른 프로세스가 강제로 cpu 점유하는 것이 가능

<br>

| 이름 | 소스코드 |
|---|---|
| RR (Round Robin) | [RR](./preemptive/RR.cc) |
| SRT (Shortest Remaining Time First) | [SRT](./preemptive/SRT.cc) |
| MLQ (Multilevel Queue) | [MLQ](./preemptive/MLQ.cc) |
| MFQ (Multilevel Feedback Queue) | |
