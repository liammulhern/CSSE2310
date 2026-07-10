/*
 * ============================================================================
 *
 *       Filename:  run.h
 *
 *    Description:  Header file for run.c
 *
 *        Version:  1.0
 *        Created:  29/09/23 22:34:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Liam Mulhern (s4742874), 
 *   Organization:  CSSE2310 A3
 *
 * ============================================================================
 */
#ifndef RUN_H
#define RUN_H

#define RUN_CMP "cmp"
#define RUN_EXPECTED "good-uqwordladder"
#define PID_COUNT 3
#define PIPE_COUNT 2
#define RUN_EXEC_FAIL 99
#define RUN_CMP_BUFFER_SIZE 50
#define READ 0
#define WRITE 1
#define RUN_CMP_SUCCESS 0
#define RUN_TIMEOUT_SEC 1
#define RUN_TIMEOUT_NSEC 500000000
#define MAX_NSEC 1000000000
#define RUN_CMP_OUTPUT "/dev/null"

#include <stdbool.h>
#include <signal.h>

#include "arguments.h"
#include "hashtable.h"
#include "job.h"

/* RunPipeType
 * ---------------
 * Enum associating pipe identifier with pipeFds index.
 * 
 * Parameters:
 *  TEST_STDOUT_PIPE: Index of test program's stdout pipe
 *  TEST_STDERR_PIPE: Index of test program's stderr pipe
 */
typedef enum {
    TEST_STDOUT_PIPE = 0,
    TEST_STDERR_PIPE = 1,
} RunPipeType;

/* RunPidType
 * ---------------
 * Enum associating pid identifier with pids index.
 * 
 * Parameters:
 *  TEST_PROGRAM: Index of test program pid
 *  TEST_STDOUT_CMP: Index of stdout cmp pid
 *  TEST_STDERR_CMP: Index of stderr cmp pid
 */
typedef enum {
    TEST_PROGRAM = 0,
    TEST_STDOUT_CMP = 1,
    TEST_STDERR_CMP = 2,
} RunPidType;

/* RunTestOutcome
 * ---------------
 * Represents the outcome for a given test.
 * 
 * Parameters:
 *  jobTest: The job associated with the test outcome
 *  stdoutExitStatus: The exit status of stdout cmp
 *  stderrExitStatus: The exit status of stderr cmp
 *  testExitStatus: The exit status of th etest program
 */
typedef struct RunTestOutcome {
    JobTest* jobTest;
    int stdoutExitStatus;
    int stderrExitStatus;
    int testExitStatus;
    bool stderrExited;
    bool stdoutExited;
    bool testExited;
} RunTestOutcome;

/* RunOutcome
 * ---------------
 * Represents the total of all outcomes of all tests executed.
 * 
 * Parameters:
 *  totalTests: The total number of tests completed.
 *  successfulTests: The number of successful tests completed.
 */
typedef struct RunOutcome {
    int totalTests;
    int successfulTests;
} RunOutcome;

/* run_tests()
 * ---------------
 * Runs all tests required by program and saves outcome data for printing.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTests: The hash table of parsed jobs to test.
 *
 * Returns:
 *  Exit status of test. If all tests pass, ERR_TEST_ALL_PASS, if one
 *  or more fail ERR_TEST_FAIL, if interupted with SIGINT ERR_TEST_INTERUPTED.
 * 
 * Errors:
 *  None
 */
int run_tests(Arguments* arguments, HashTable* jobTests);

/* run_create_child_signal()
 * ---------------
 * Creates the child signal handler for SIGCHLD and attaches handler to 
 * run_child_signal_handler().
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_create_child_signal();

/* run_create_interupt_signal()
 * ---------------
 * Creates the interup signal handler for SIGINT and attaches handler to
 * run_interupt_signal_handler().
 * 
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_create_interupt_singal();

/* run_child_signal_handler()
 * ---------------
 * The signal handler for SIGCHLD signal.
 * 
 * Parameters:
 *  signal: The signal sent to the handler.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_child_signal_handler(int signal);

/* run_interupt_signal_handler()
 * ---------------
 * The signal handler for SIGINT signal. 
 * 
 * Parameters:
 *  signal: The signal sent to the handler.
 *
 * Returns:
 *  None
 *
 * Global variables modified:
 *  testInterupted: Sets test interupted flag high when signal is received.
 * 
 * Errors:
 *  None
 */
void run_interupt_signal_handler(int signal);

/* run_open_pipes()
 * ---------------
 * Allocates memory and opens the desired number of pipes 
 * specified in PIPE_COUNT.
 *
 * Returns:
 *  Two dimensional array of file descriptors of size PIPE_COUNT.
 * 
 * Errors:
 *  Assumes that pipe will not fail.
 */
int** run_open_pipes();

/* run_close_pipes()
 * ---------------
 * Closes and frees allocated memory for pipes in two dimensional array.
 * 
 * Parameters:
 *  pipeFds: The two dimensional array of file descriptors to be closed.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_close_pipes(int** pipeFds);

/* run_child_processes()
 * ---------------
 * Executes and redirects pipes for forked test and compare program before
 * closing all remaining file descriptors.
 * 
 * Parameters:
 *  pipeFds: The two dimensional array of file descriptors to be redirected
 *      by child programs.
 *
 * Returns:
 *  Pointer to array of child pids of size PID_COUNT.
 * 
 * Errors:
 *  Exits status of child processes set to RUN_EXEC_FAIL if exec fails.
 */
pid_t* run_child_processes(
        Arguments* arguments,
        JobTest* jobTest);

/* run_child_program()
 * ---------------
 * Execute test program and redirect stdout and stderr to the write ends of 
 * TEST_STDOUT_PIPE and TEST_STDERR_PIPE respectively.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTest: The hash table element of single parsed job to test.
 *  pipeFds: The two dimensional array of file descriptors to be redirected
 *      by child programs.
 *
 * Returns:
 *  The pid of the forked child.
 * 
 * Errors:
 *  Exits status of child processes is set to RUN_EXEC_FAIL if exec fails.
 */
pid_t run_child_program(
        Arguments* arguments,
        JobTest* jobTest,
        int** pipeFds);

/* run_child_cmp()
 * ---------------
 * Execute UNIX cmp program and redirect stdin to the read ends of 
 * TEST_STDOUT_PIPE and TEST_STDERR_PIPE depending on runPidType.
 * 
 * Parameters:
 *  arguments: The command line arguments given to the program.
 *  jobTest: The hash table element of single parsed job to test.
 *  pipeFds: The two dimensional array of file descriptors to be redirected
 *      by child programs.
 *  compareFilePath: The string file path to compare stdin to.
 *  runPidType: Dictates the arguments to exec cmp with.
 *
 * Returns:
 *  The pid of the forked child.
 * 
 * Errors:
 *  Exits status of child processes is set to RUN_EXEC_FAIL if exec fails.
 */
pid_t run_child_cmp(
        Arguments* arguments,
        JobTest* jobTest,
        int** pipeFds,
        char* compareFilePath,
        RunPidType runPidType);

/* run_kill_child_processes()
 * ---------------
 * Send SIGKILL to all processes in the array of pids.
 * 
 * Parameters:
 *  pids: The array of pids of the forked child.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_kill_child_processes(pid_t* pids);

/* run_exit_timeout()
 * ---------------
 * Handles the child process timeout of 1.5s or until all child processes
 * have finished.
 * 
 * Parameters:
 *  pids: The array of pids of the forked child.
 *  jobTest: The hash table element of single parsed job to test.
 * 
 * Returns:
 *  RunTestOutcome containing the exit status of the processes. 
 * 
 * Errors:
 *  None
 */
RunTestOutcome* run_exit_timeout(pid_t* pids, JobTest* jobTest);

/* run_wait()
 * ---------------
 * Wait for each child process to reap defunct 
 * child and get exit status.
 * 
 * Parameters:
 *  pids: The array of pids of the forked child.
 *  runTestOutcome: Struct containing the exit status of the processes. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_wait(
        pid_t* pids,
        RunTestOutcome* runTestOutcome);

/* run_exit_status()
 * ---------------
 * Get the exit status of a child using siginfo struct
 * 
 * Parameters:
 *  pids: The array of pids of the forked child.
 *  pid: The pid of the exited child.
 *  waitInfo: The wait info of the exited child.
 *  runTestOutcome: Struct containing the exit status of the processes. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_exit_status(
        pid_t* pids,
        pid_t pid, 
        siginfo_t* waitInfo,
        RunTestOutcome* runTestOutcome);

/* run_get_timeout()
 * ---------------
 * Allocates and creates timespec struct with current time + 1.5s 
 * for absolute wait.
 *
 * Returns:
 *  Timespec struct 1.5s in the future.
 * 
 * Errors:
 *  None
 */
struct timespec* run_get_timeout();

/* run_initialise_outcome()
 * ---------------
 * Allocate and create RunTestOutcome with default exit status 
 * of RUN_EXEC_FAIL for all child processes.
 * 
 * Parameters:
 *  jobTest: The hash table element of single parsed job to test.
 *
 * Returns:
 *  RunTestOutcome containing the default exit status of the processes. 
 * 
 * Errors:
 *  None
 */
RunTestOutcome* run_initialise_outcome(JobTest* jobTest);

/* run_check_outcome_fail()
 * ---------------
 * Check if the test has any failed processes i.e exit status 
 * of RUN_EXEC_FAIL.
 * 
 * Parameters:
 *  RunTestOutcome: Struct containing the exit status of the processes. 
 *
 * Returns:
 *  True if any test has failed, false otherwise.
 * 
 * Errors:
 *  None
 */
bool run_check_outcome_fail(RunTestOutcome* runTestOutcome);

/* run_check_outcome_pass()
 * ---------------
 * Check if the test has no differences on stdout, stderr, and exit status.
 * 
 * Parameters:
 *  RunTestOutcome: Struct containing the exit status of the processes. 
 *
 * Returns:
 *  True if there are no differences, false otherwise.
 * 
 * Errors:
 *  None
 */
bool run_check_outcome_pass(RunTestOutcome* runTestOutcome);

/* run_print_start()
 * ---------------
 * Print and flush the starting prompt to stdout.
 * 
 * Parameters:
 *  testId: The test ID for the running test.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_print_start(char* testId);

/* run_print_fail()
 * ---------------
 * Print and flush the fail prompt to stdout.
 * 
 * Parameters:
 *  testId: The test ID for the running test.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_print_fail(char* testId);

/* run_print_outcome_all()
 * ---------------
 * Print test outcome prompt to stdout for all comparisons.
 * 
 * Parameters:
 *  RunTestOutcome: Struct containing the exit status of the processes. 
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_print_outcome_all(RunTestOutcome* runTestOutcome);

/* run_print_outcome()
 * ---------------
 * Print a single test's outcome to stdout indicating its differences.
 * 
 * Parameters:
 *  testId: The test ID for the running test.
 *  testType: The type of test that was completed.
 *  differs: Whether the comparison between expected and test differs or not.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_print_outcome(char* testId, char* testType, bool differs);

/* run_print_total_outcome()
 * ---------------
 * Print the outcome of all completed tests to stdout.
 * 
 * Parameters:
 *  runOutcome: The outcome of all completed tests.
 *
 * Returns:
 *  Exit status depending on test completion.
 * 
 * Errors:
 *  None
 */
int run_print_total_outcome(RunOutcome* runOutcome);

/* run_check_cmp_status_differs()
 * ---------------
 * Check if the exit status of cmp indicates matching files.
 * 
 * Parameters:
 *  exitStatus: The exit status of the cmp process.
 *
 * Returns:
 *  True if cmp differs, false otherwise.
 * 
 * Errors:
 *  None
 */
bool run_check_cmp_status_differs(int exitStatus);

/* run_check_program_status_differs()
 * ---------------
 * Check if the exit status of the test program differs from expected.
 * 
 * Parameters:
 *  runTestOutcome: The outcome of a given test.
 *
 * Returns:
 *  True if the exit status differs, false otherwise.
 * 
 * Errors:
 *  None
 */
bool run_check_program_status_differs(RunTestOutcome* runTestOutcome);

/* run_destroy_outcome()
 * ---------------
 * Frees memory allocated to RunTestOutcome struct.
 * 
 * Parameters:
 *  object: Generic pointer to RunTestOutcome struct.
 *
 * Returns:
 *  None
 * 
 * Errors:
 *  None
 */
void run_destory_outcome(void* object);

#endif
