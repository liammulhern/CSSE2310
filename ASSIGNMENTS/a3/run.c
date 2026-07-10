/*
 * ============================================================================
 *
 *       Filename:  run.c
 *
 *    Description:  Run child processes used to compare stdout and stderr
 *                  of test program.
 *
 *        Version:  1.0
 *        Created:  29/09/23 22:14:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include <errno.h>

#include "run.h"
#include "error.h"
#include "validity.h"

// Global used in SIGINT signal handler
bool testInterupted = false;

int run_tests(Arguments* arguments, HashTable* jobTests) {
    // Initialise run outcome with no successful tests
    RunOutcome runOutcome = {0, 0};

    size_t index = 0;
    HashTableNode* currentNode = NULL; 

    // Iterate over HashTable and create output files
    while ((currentNode = hash_table_iterate(jobTests,
            currentNode, &index)) != NULL 
            && !testInterupted) {
        // Get the jopTest struct from the hashtable node
        JobTest* jobTest = (JobTest*) currentNode->object;

        run_print_start(jobTest->testId);
        pid_t* pids = run_child_processes(arguments, jobTest);

        RunTestOutcome* runTestOutcome = run_exit_timeout(pids, jobTest);

        if (testInterupted) {
            run_destory_outcome(runTestOutcome);
            break;
        }

        runOutcome.totalTests++;

        if (run_check_outcome_fail(runTestOutcome)) {
            run_print_fail(jobTest->testId);
            run_destory_outcome(runTestOutcome);
            continue;
        }

        if (run_check_outcome_pass(runTestOutcome)) {
            runOutcome.successfulTests++;
        }

        run_print_outcome_all(runTestOutcome);
        run_destory_outcome(runTestOutcome);
    }

    return run_print_total_outcome(&runOutcome);
}

void run_create_child_signal() {
    struct sigaction saChild;
    memset(&saChild, 0, sizeof(saChild));

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);

    saChild.sa_handler = run_child_signal_handler;
    saChild.sa_mask = sigset;
    saChild.sa_flags = 0;

    sigaction(SIGCHLD, &saChild, NULL);
}

void run_create_interupt_singal() {
    struct sigaction saInterupt;
    memset(&saInterupt, 0, sizeof(saInterupt));

    saInterupt.sa_handler = run_interupt_signal_handler;
    saInterupt.sa_flags = SA_RESTART;

    sigaction(SIGINT, &saInterupt, 0);
}

void run_child_signal_handler(int signal) {
    return;
}

void run_interupt_signal_handler(int signal) {
    testInterupted = true;
}

int** run_open_pipes() {
    // Allocate memory for all pipes
    int** pipeFds = (int**) malloc(sizeof(int*) * PIPE_COUNT);
    
    for (int i = 0; i < PIPE_COUNT; i++) {
        // Allocate memory for both read and write file descriptor 
        int* pipeFd = (int*) malloc(sizeof(int) * 2);

        pipeFds[i] = pipeFd;
    }

    // stdout pipe to cmp 
    pipe(pipeFds[TEST_STDOUT_PIPE]);
    // stderr pipe to cmp
    pipe(pipeFds[TEST_STDERR_PIPE]);

    return pipeFds;
}

void run_close_pipes(int** pipeFds) {
    close(pipeFds[TEST_STDOUT_PIPE][READ]);
    close(pipeFds[TEST_STDOUT_PIPE][WRITE]);
    close(pipeFds[TEST_STDERR_PIPE][READ]);
    close(pipeFds[TEST_STDERR_PIPE][WRITE]);

    for (int i = 0; i < PIPE_COUNT; i++) {
        free(pipeFds[i]);
    }

    free(pipeFds);
}

pid_t* run_child_processes(
        Arguments* arguments,
        JobTest* jobTest) {

    pid_t* pids = (pid_t*) malloc(sizeof(pid_t) * PID_COUNT);
    int** pipeFds = run_open_pipes();

    pids[TEST_STDOUT_CMP] = run_child_cmp(arguments, jobTest, pipeFds, 
            jobTest->testStdoutPath, TEST_STDOUT_CMP);
    pids[TEST_STDERR_CMP] = run_child_cmp(arguments, jobTest, pipeFds, 
            jobTest->testStderrPath, TEST_STDERR_CMP);
    pids[TEST_PROGRAM] = run_child_program(arguments, jobTest, pipeFds);

    run_close_pipes(pipeFds);

    return pids;
}

pid_t run_child_program(
        Arguments* arguments,
        JobTest* jobTest,
        int** pipeFds) {
    pid_t testForkId = fork();

    if (testForkId == 0) {
        // Child process
        int inputFd = open(jobTest->testInputFileName, O_RDONLY, S_IRUSR);

        close(pipeFds[TEST_STDOUT_PIPE][READ]);
        close(pipeFds[TEST_STDERR_PIPE][READ]);

        // Redirect test stdin to input file 
        dup2(inputFd, STDIN_FILENO);
        // Redirect test stdout to TEST_STDOUT_PIPE write
        dup2(pipeFds[TEST_STDOUT_PIPE][WRITE], STDOUT_FILENO);
        // Redirect test stderr to TEST_STDERR_PIPE write
        dup2(pipeFds[TEST_STDERR_PIPE][WRITE], STDERR_FILENO);
        
        close(inputFd);
        close(pipeFds[TEST_STDOUT_PIPE][WRITE]);
        close(pipeFds[TEST_STDERR_PIPE][WRITE]);

        // Create test uqwordladder args 
        jobTest->testInputArguments[0] = arguments->testProgramPath;
        execvp(arguments->testProgramPath, jobTest->testInputArguments);

        exit(RUN_EXEC_FAIL);
    }

    return testForkId;
}

pid_t run_child_cmp(
        Arguments* arguments,
        JobTest* jobTest,
        int** pipeFds,
        char* compareFilePath,
        RunPidType runPidType) {
    pid_t cmpForkId = fork();

    if (cmpForkId == 0) {
        // Child process
        switch (runPidType) {
            case TEST_STDOUT_CMP:
                close(pipeFds[TEST_STDOUT_PIPE][WRITE]);
                close(pipeFds[TEST_STDERR_PIPE][READ]);

                // Redirect test stdout to stdin of cmp
                dup2(pipeFds[TEST_STDOUT_PIPE][READ], STDIN_FILENO);

                close(pipeFds[TEST_STDOUT_PIPE][READ]);
                break;
            case TEST_STDERR_CMP:
                close(pipeFds[TEST_STDOUT_PIPE][READ]);
                close(pipeFds[TEST_STDOUT_PIPE][WRITE]);

                // Redirect test stderr to stdin of cmp
                dup2(pipeFds[TEST_STDERR_PIPE][READ], STDIN_FILENO);

                close(pipeFds[TEST_STDERR_PIPE][READ]);
                break;
            default:
                break;
        }

        close(pipeFds[TEST_STDERR_PIPE][WRITE]);
        int compareOutputFd = open(RUN_CMP_OUTPUT, O_WRONLY, S_IWUSR);

        // Redirect CMP stderr and stdout to /dev/null
        dup2(compareOutputFd, STDOUT_FILENO);
        dup2(compareOutputFd, STDERR_FILENO);

        // Create cmp args to compare stdin with stdoutpath
        char* compareArgs[] = {RUN_CMP, compareFilePath, NULL};
        execvp(RUN_CMP, compareArgs);

        exit(RUN_EXEC_FAIL);
    }

    return cmpForkId;
}

void run_kill_child_processes(pid_t* pids) {
    for (int i = 0; i < PID_COUNT; i++) {
        kill(pids[i], SIGKILL);
        waitpid(pids[i], NULL, 0);
    }
}

RunTestOutcome* run_exit_timeout(pid_t* pids, JobTest* jobTest) {
    RunTestOutcome* runTestOutcome = run_initialise_outcome(jobTest);
    struct timespec* timeout = run_get_timeout();
    int sleepWaitStatus = 0;

    while (!runTestOutcome->testExited 
            || !runTestOutcome->stdoutExited
            || !runTestOutcome->stderrExited) {

        // Sleep using absolute time to 1.5 seconds from process execution.
        sleepWaitStatus = clock_nanosleep(CLOCK_MONOTONIC, 
                TIMER_ABSTIME, timeout, NULL);

        // Check if timer has ended 
        if (sleepWaitStatus == 0) {
            run_kill_child_processes(pids);
            run_wait(pids, runTestOutcome);
            break;
        }

        run_wait(pids, runTestOutcome);
    }

    free(timeout);
    free(pids);
    
    return runTestOutcome;
}

void run_wait(pid_t* pids, RunTestOutcome* runTestOutcome) {
    siginfo_t waitInfo;

    // Attempt to wait on test program if it has not yet exited.
    if (!runTestOutcome->testExited) {
        waitid(P_PID, pids[TEST_PROGRAM], &waitInfo, WEXITED);
        run_exit_status(pids, waitInfo.si_pid, &waitInfo, runTestOutcome);
    }

    // Attempt to wait on cmp stdout program if it has not yet exited.
    if (!runTestOutcome->stdoutExited) {
        waitid(P_PID, pids[TEST_STDOUT_CMP], &waitInfo, WEXITED);
        run_exit_status(pids, waitInfo.si_pid, &waitInfo, runTestOutcome);
    }

    // Attempt to wait on cmp stderr program if it has not yet exited.
    if (!runTestOutcome->stderrExited) {
        waitid(P_PID, pids[TEST_STDERR_CMP], &waitInfo, WEXITED);
        run_exit_status(pids, waitInfo.si_pid, &waitInfo, runTestOutcome);
    }
}

void run_exit_status(
        pid_t* pids,
        pid_t pid, 
        siginfo_t* waitInfo,
        RunTestOutcome* runTestOutcome) {
    int exitStatus = RUN_EXEC_FAIL;
    
    // Check if child exited successfully
    if (waitInfo->si_code == CLD_EXITED) {
        exitStatus = waitInfo->si_status;
    }

    // Set test outcome for each child process
    if (pid == pids[TEST_PROGRAM]) {
        runTestOutcome->testExitStatus = exitStatus;
        runTestOutcome->testExited = true;
    } else if (pid == pids[TEST_STDOUT_CMP]) {
        runTestOutcome->stdoutExitStatus = exitStatus;
        runTestOutcome->stdoutExited = true;
    } else if (pid == pids[TEST_STDERR_CMP]) {
        runTestOutcome->stderrExitStatus = exitStatus;
        runTestOutcome->stderrExited = true;
    }
}

struct timespec* run_get_timeout() {
    struct timespec* timeout = (struct timespec*) malloc(sizeof(
            struct timespec));
    
    clock_gettime(CLOCK_MONOTONIC, timeout);

    // Add timeout to current time 
    timeout->tv_sec += RUN_TIMEOUT_SEC;
    timeout->tv_nsec += RUN_TIMEOUT_NSEC;

    if (timeout->tv_nsec > MAX_NSEC) {
        timeout->tv_nsec -= MAX_NSEC;
        timeout->tv_sec++;
    }

    return timeout;
}

RunTestOutcome* run_initialise_outcome(JobTest* jobTest) {
    RunTestOutcome* runTestOutcome = (RunTestOutcome*) malloc(
            sizeof(RunTestOutcome));

    runTestOutcome->jobTest = jobTest;

    // Set default exit status to RUN_EXEC_FAIL as fail safe
    runTestOutcome->stdoutExitStatus = RUN_EXEC_FAIL;
    runTestOutcome->stderrExitStatus = RUN_EXEC_FAIL;
    runTestOutcome->testExitStatus = RUN_EXEC_FAIL;
    runTestOutcome->stdoutExited = false;
    runTestOutcome->stderrExited = false;
    runTestOutcome->testExited = false;

    return runTestOutcome;
}

bool run_check_outcome_fail(RunTestOutcome* runTestOutcome) {
    return runTestOutcome->stdoutExitStatus == RUN_EXEC_FAIL 
            || runTestOutcome->stderrExitStatus == RUN_EXEC_FAIL
            || runTestOutcome->testExitStatus == RUN_EXEC_FAIL;
}

bool run_check_outcome_pass(RunTestOutcome* runTestOutcome) {
    return !run_check_cmp_status_differs(runTestOutcome->stdoutExitStatus)
        && !run_check_cmp_status_differs(runTestOutcome->stderrExitStatus)
        && !run_check_program_status_differs(runTestOutcome);
}

void run_print_start(char* testId) {
    printf("Running job: %s\n", testId);
    fflush(stdout);
}

void run_print_fail(char* testId) {
    printf("Unable to execute job %s\n", testId);
    fflush(stdout);
}

void run_print_outcome_all(RunTestOutcome* runTestOutcome) {
    char* testId = runTestOutcome->jobTest->testId;

    run_print_outcome(testId, "Stdout", 
            run_check_cmp_status_differs(runTestOutcome->stdoutExitStatus));
    run_print_outcome(testId, "Stderr", 
            run_check_cmp_status_differs(runTestOutcome->stderrExitStatus));
    run_print_outcome(testId, "Exit status", 
            run_check_program_status_differs(runTestOutcome));
}

void run_print_outcome(char* testId, char* testType, bool differs) {
    printf("Job %s: %s ", testId, testType);

    if (differs) {
        printf("differs\n");
    } else {
        printf("matches\n");
    }
}

int run_print_total_outcome(RunOutcome* runOutcome) {
    if (testInterupted && runOutcome->totalTests == 0) {
        printf("testuqwordladder: No tests completed\n");

        return ERR_TEST_INTERUPTED;
    } 

    printf("testuqwordladder: %d out of %d tests passed\n", 
            runOutcome->successfulTests, runOutcome->totalTests);

    // Check if tests have been completed and successful 
    if (runOutcome->totalTests > 0
            && runOutcome->totalTests == runOutcome->successfulTests) {
        return ERR_TEST_ALL_PASS;
    } else {
        return ERR_TEST_FAIL;
    }
}

bool run_check_cmp_status_differs(int exitStatus) {
    return exitStatus != RUN_CMP_SUCCESS;
}

bool run_check_program_status_differs(RunTestOutcome* runTestOutcome) {
    FILE* expectedExitStatusFile = fopen(
            runTestOutcome->jobTest->testExitStatusPath, "r");
    int expectedExitStatus;

    // Read exit status from file
    fscanf(expectedExitStatusFile, "%d\n", &expectedExitStatus);
    fclose(expectedExitStatusFile); 

    return expectedExitStatus != runTestOutcome->testExitStatus;
}

void run_destory_outcome(void* object) {
    RunTestOutcome* runTestOutcome = (RunTestOutcome*) object;

    free(runTestOutcome);
}
