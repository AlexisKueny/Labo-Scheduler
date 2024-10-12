// Imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros
#define RR_QUANTUM 2
#define CNTXT_SWITCH 1
#define LINESIZE 250
#define INPATH "C:\\Users\\alexi\\CLionProjects\\Scheduler\\tasks.csv"
#define EXECOUTPATH "C:\\Users\\alexi\\CLionProjects\\Scheduler\\execution.csv"
#define PERFOUTPATH "C:\\Users\\alexi\\CLionProjects\\Scheduler\\performance.csv"

// Process data structures
typedef struct ProcessInfo {
    int pid;
    int arrTime;
    int startTime;
    int execTime;
    int priority;
    int timeLeft;
    int completionTime;
} processInfo_t;

void printStruct(processInfo_t value) {
    printf("PID:%d,ArrTime:%d,ExecTime:%d,Priority:%d,TimeLeft:%d,CompletionTime:%d\n",
        value.pid,value.arrTime,value.execTime,value.priority,value.timeLeft,value.completionTime);
}

// Define the structure of a node in the linked list
typedef struct processList{
 processInfo_t p_e;
 struct processList *next;
}processL_t;

// ----------------------------------------------------------------------------------------------------------------- //
//                                                LINKED LISTS                                                       //
// ------------------------------------------------------------------------------------------------------------------//

// Function prototypes
processL_t* createNode(processInfo_t data);
void insertAtHead(processL_t** head, processInfo_t data);
void insertAtEnd(processL_t** head, processInfo_t data);
void printList(processL_t* head);
void deleteList(processL_t** head);

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
 * Returns length of linked list
 * @param head head of linked list
 * @return length of linked lists
 */
int findLength(processL_t* head) {
    int count = 0;
    processL_t* p_current = head;
    while (p_current != NULL) {
        count++;
        p_current = p_current->next;
    }
    return count;
}

/**
 * Return the first process with the highest priority.
 * And that can be run.
 * @param head head of list
 * @param currentTime time when the scheduler request the process
 */
processL_t* selectHighestPriorityProcess(processL_t* head, int currentTime) {
    processL_t* highestPriority = NULL;
    processL_t* temp = head;

    while (temp != NULL) {
        if (temp->p_e.arrTime <= currentTime) {  // Only consider processes that have arrived
            if (highestPriority == NULL || temp->p_e.priority < highestPriority->p_e.priority) {
                highestPriority = temp;
            }
        }
        temp = temp->next;
    }

    return highestPriority;
}

// -------------------------------------------------------------------------------------------------------------------//
//                                             SCHEDULER ALGORITHMS                                                   //
// -------------------------------------------------------------------------------------------------------------------//

/**
 * Executes first come, first served algorithm, which handles processes by order of arrival.
 * Writes into execution.csv
 * @param head Head of linked lists containing incoming process information
 */
void firstComeFirstServed(processL_t* head) {
    // Variable declarations
    int completionTime = 0;
    int totalTime = 0;
    int turnAroundTime,waitingTime;
    processL_t *p_current = head;
    int timeFlow = 0;

    // Output stream
    FILE *outStream = fopen(EXECOUTPATH,"w");

    // Iterate through list and write into file
    while(p_current != NULL) {
        if (p_current->p_e.arrTime <= timeFlow) {
            timeFlow += p_current->p_e.execTime;
            p_current->p_e.completionTime = timeFlow;
            turnAroundTime = p_current->p_e.completionTime - p_current->p_e.arrTime;
            waitingTime = turnAroundTime - p_current->p_e.execTime;
            fprintf(outStream,"%d %d %d %d\n",p_current->p_e.pid,turnAroundTime,waitingTime,0);
            p_current = p_current->next;
        }
        else {
            timeFlow ++;
        }
    }
    fclose(outStream);
}

/**
 * Round-robin algorithm for process management.
 * Writes output into "exec.csv"
 * @param head of data list
 */
void roundRobin(processL_t* head) {
    int timeFlow = 0;
    int turnAround,waitingTime;
    int completionCount = 1;
    processL_t *p_current = head;
    int listLength = findLength(head);
    printf("Length of list is: %d\n",findLength(head));
    printList(head);
    printf("\n");

    // Request queue list
    processL_t *queue_head = NULL;
    processL_t *queue_current = queue_head;

    // File output
    FILE *outStream = fopen(EXECOUTPATH,"w");

    while (completionCount < listLength + 1) {
        while (p_current != NULL) {
            if (p_current->p_e.arrTime <= timeFlow && p_current->p_e.timeLeft > 0) {
                printStruct(p_current->p_e);
                p_current->p_e.timeLeft -= RR_QUANTUM;
            }
            else if (p_current->p_e.timeLeft == 0 && p_current->p_e.pid == completionCount) {
                p_current->p_e.completionTime = timeFlow;
                printf("============\n");
                printStruct(p_current->p_e);
                printf("============\n");
                turnAround = p_current->p_e.completionTime - p_current->p_e.arrTime;
                waitingTime = turnAround - p_current->p_e.execTime;
                fprintf(outStream,"%d %d %d %d\n",p_current->p_e.pid,turnAround,waitingTime,0);
                completionCount++;
            }
            p_current = p_current->next;
        }
        timeFlow += RR_QUANTUM;
        p_current = head;
    }
    fclose(outStream);
}

void priority_algo(processL_t** head) {
    processL_t* runningProcess = NULL;  // The process currently being executed
    processL_t* prev = NULL;            // Pointer to track the previous process in the list
    processL_t* temp = NULL;            // Temporary pointer for removing nodes
    int time = 0;                       // Global time
    int contextSwitches = 0;            // Number of context switches

    // Output file stream to write execution results
    FILE *outStream = fopen(EXECOUTPATH, "w");
    if (outStream == NULL) {
        printf("Error opening output file\n");
        return;
    }

    // Priority-based preemptive scheduling simulation
    while (*head || runningProcess) {
        // Select the highest priority process that has arrived
        processL_t* selected = selectHighestPriorityProcess(*head, time);

        // Preemption: If a higher-priority process is available, preempt the current one
        if (runningProcess && selected && selected->p_e.priority < runningProcess->p_e.priority) {
            printf("Time %d: Preempting process PID: %d for higher-priority process PID: %d\n",
                   time, runningProcess->p_e.pid, selected->p_e.pid);
            time += CNTXT_SWITCH;
            contextSwitches++;
            runningProcess = NULL;
        }

        // If the running process has finished execution
        if (runningProcess && runningProcess->p_e.timeLeft == 0) {
            printf("Time %d: Process PID: %d finished execution\n", time, runningProcess->p_e.pid);
            int turnaroundTime = time - runningProcess->p_e.arrTime;
            int waitTime = turnaroundTime - runningProcess->p_e.execTime;

            // Write process execution details to file
            fprintf(outStream, "%d %d %d 1\n", runningProcess->p_e.pid, turnaroundTime, waitTime);

            // Remove the completed process from the list
            processL_t* curr = *head;
            prev = NULL;
            while (curr != NULL) {
                if (curr == runningProcess) {  // Found the process to remove
                    if (prev == NULL) {
                        // Removing the head node
                        *head = curr->next;
                    } else {
                        // Bypass the node in the list
                        prev->next = curr->next;
                    }
                    free(curr);  // Free the node
                    break;
                }
                prev = curr;
                curr = curr->next;
            }

            runningProcess = NULL;  // No process is running now
            time += CNTXT_SWITCH;
            contextSwitches++;
        }

        // Start a new process if no process is running
        if (runningProcess == NULL && selected != NULL) {
            runningProcess = selected;
            if (runningProcess->p_e.startTime == -1) {  // If it's the first time the process runs
                runningProcess->p_e.startTime = time;
            }
            printf("Time %d: Running process PID: %d, Priority: %d\n", time, runningProcess->p_e.pid, runningProcess->p_e.priority);
            runningProcess->p_e.timeLeft--;  // Decrement remaining time
            time++;  // Increment time
        }

        // If no process is available, increment time (idle time)
        if (runningProcess == NULL && selected == NULL) {
            printf("Time %d: No process available, idle...\n", time);
            time++;
        }
    }

    printf("Context switches: %d\n", contextSwitches);
    fclose(outStream);  // Close output file
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

    // File streams
    FILE *file = fopen(filepath, "r");
    FILE *myStream = fopen("C:\\Users\\alexi\\CLionProjects\\Scheduler\\execution.csv","w");

    /*
     * PART I: READ FILE LINE BY LINE AND INSERT IN LINKED LIST
     */
    while (fgets(line, sizeof(line), file)) {
        // Fill out the process structure
        elem1 = strtok(line, " ");
        elem2 = strtok(NULL, " ");
        elem3 = strtok(NULL, " ");
        elem4 = strtok(NULL, " ");
        processInfo_t process_info = {atoi(elem1),atoi(elem2),atoi(elem3),atoi(elem4),atoi(elem3)}; // All necessary process info
        insertAtEnd(&head, process_info);
    }
    fclose(file);
    //printList(head);
    printf("\n");

    /*
     * PART II: CALL REQUESTED ALGORITHM AND EXECUTE IT
     */
    switch (algo) {
        case 1:
            firstComeFirstServed(head);
            break;
        case 2:
            roundRobin(head);
            break;
        case 3:
            priority_algo(&head);
            break;
        default:
            printf("Invalid input\n");
            exit(1);
    }
}

/**
 * Main function to execute program
 * @return 0
 */
int main() {
    printf("Hello World!\n");
    simulate(INPATH,3);
    printf("End Of Program P1\n");
    return 0;
}