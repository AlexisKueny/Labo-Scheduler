// Imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros
#define RR_QUANTUM 2
#define CNTXT_SWITCH 1
#define LINESIZE 250

// Process data structures
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

// Define the structure of a node in the linked list
typedef struct processList{
 processInfo_t p_e;
 struct processList *next;
}processL_t;

// Function prototypes
processL_t* createNode(processInfo_t data);
processExec_t fCFS (processInfo_t process, int completionTime);
void insertAtHead(processL_t** head, processInfo_t data);
void insertAtEnd(processL_t** head, processInfo_t data);
void printList(processL_t* head);
void deleteList(processL_t** head);
void simulate(char filepath[], int algo);

/**
 * Creates new list node
 * @param data Data to insert in list
 * @return Brand-new node
 */
processL_t* createNode(processInfo_t data) {
    processL_t* newNode = (processL_t*)malloc(sizeof(processL_t));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->p_e = data;
    newNode->next = NULL;
    return newNode;
}

/**
 * Inserts a node at the beginning of list
 * @param head head of list
 * @param data data to store
 */
void insertAtHead(processL_t** head, processInfo_t data) {
    processL_t* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

/**
 * Inserts a node at end of list
 * @param head Head of list
 * @param data Data to store
 */
void insertAtEnd(processL_t** head, processInfo_t data) {
    processL_t* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    processL_t* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

/**
 * Prints the linked list
 * @param head head of linked list
 */
void printList(processL_t* head) {
    processL_t* p = head;
    while (p != NULL) {
        printf("Process ID: %d, Arrival Time: %d, Execution Time: %d, Priority: %d\n",
            p->p_e.pid, p->p_e.arrTime,
            p->p_e.execTime, p->p_e.priority);
        p = p->next;
    }
    printf("NULL\n");
}

/**
 * Deletes the linked list
 * @param head head of list
 */
void deleteList(processL_t** head) {
    processL_t* temp = *head;
    processL_t* nextNode;
    while (temp != NULL) {
        nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    *head = NULL;
}

/**
 * Simulates an OS scheduler
 * @param filepath The filepath to the CSV containing the tasks to analyze
 * @param algo The chosen algorithm to handle the tasks
 */
void simulate(char filepath[], int algo) {
    // Declare variables
    char* line[LINESIZE];
    char *elem1, *elem2, *elem3, *elem4;
    int completionTime = 0;
    processL_t* head = NULL;
    processExec_t process_out = {0,0,0,0};

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

/**
 * First come, first served algorithm which executes processes in order of arrival
 * @param process The process in question; comes with PID,arrival time, wait time and priority
 * @param completionTime The time when the process was completed
 * @return Execution data for the process
 */
processExec_t fCFS (processInfo_t process, int completionTime) {
    int turnAroundTime = completionTime - process.arrTime;
    int waitingTime = turnAroundTime - process.execTime;
    processExec_t process_exec = {process.pid,turnAroundTime,waitingTime,0};
    return process_exec;
}

processExec_t roundRobin(processInfo_t process) {
}

/**
 * Main function to execute program
 * @return 0
 */
int main() {
    printf("Hello World!\n");
    simulate("C:\\Users\\alexi\\CLionProjects\\Scheduler\\tasks.csv",1);
    printf("End Of Program P1\n");
    return 0;
}