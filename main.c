#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RR_QUANTUM 2
#define CNTXT_SWITCH 1
#define LINESIZE 250

typedef struct ProcessInfo {
    int pid;
    int arrTime;
    int execTime;
    int priority;
} processInfo_t;

typedef struct ProcessExec {
    int pid;
    int turnAroundTime;
    int waitTime;
    int nTimes;
} processExec_t;

typedef struct ProcessPerf {
    int total_time;
    int total_nr_ctxt_switch;
    int total_time_ctx_switch;
} processPerf_t;

// Linked list struct
struct Data {
};

// Fist come first serve algorithm
processExec_t fCFS (processInfo_t process, int completionTime) {
    int turnAroundTime = completionTime - process.arrTime;
    int waitingTime = turnAroundTime - process.execTime;
    processExec_t process_exec = {process.pid,turnAroundTime,waitingTime,0};
    return process_exec;
}

void simulate(char filepath[], int algo) {
    // Declare variables
    char* line[LINESIZE];
    char *elem1, *elem2, *elem3, *elem4;
    int completionTime = 0;
    int turnAroundTime,waitingTime,executionTime,priority;
    processExec_t process_out = {0,0,0,0};
    processInfo_t process_info = {0,0,0,0};

    // File streams
    FILE *file = fopen(filepath, "r");
    FILE *myStream = fopen("C:\\Users\\alexi\\CLionProjects\\Scheduler\\execution.csv","w");

    // Reads file line by line
    while (fgets(line, sizeof(line), file)) {
        // Fill out the process structure
        elem1 = strtok(line, " ");
        elem2 = strtok(NULL, " ");
        elem3 = strtok(NULL, " ");
        elem4 = strtok(NULL, " ");
        processInfo_t process_info = {atoi(elem1),atoi(elem2),atoi(elem3),atoi(elem4)};
        printf("PID: %d\n", process_info.pid);
        completionTime += process_info.execTime;

        // Execute algorithm
        switch (algo) {
            // First come, first served algorithm
            case 1:
                process_out = fCFS(process_info,completionTime);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            default:
                break;
        }
        printf("Output Algo1: PID: %d, Turnaround time: %d, Waiting time: %d, Preempted times: %d \n",
            process_out.pid,process_out.turnAroundTime,process_out.waitTime,process_out.nTimes);

        // Write result into output file
        fprintf(myStream, "%d %d %d %d\n",
            process_out.pid,process_out.turnAroundTime,process_out.waitTime,process_out.nTimes);
    }
    fclose(file);
    printf("\n");
}

int main() {
    printf("Hello World!\n");
    simulate("C:\\Users\\alexi\\CLionProjects\\Scheduler\\tasks.csv",1);
    printf("End Of Program P1\n");
    return 0;
}