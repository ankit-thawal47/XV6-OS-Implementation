#include "printing.hpp"

#define LEVEL_1_IMPLEMENTED 1
#define LEVEL_2_IMPLEMENTED 1
#define LEVEL_3_IMPLEMENTED 1
#define LEVEL_4_IMPLEMENTED 1

#if LEVEL_2_IMPLEMENTED == 1
// TEST 1 (task 2, C API threads and synchronous context switching)
#include "Threads_C_API_test.hpp"
// TEST 2 (task 2, CPP API thread and synchronous context change)
#include "Threads_CPP_API_test.hpp"
// TEST 7 (task 2, testing whether user code is executed in user mode)
#include "System_Mode_test.hpp"
#endif

#if LEVEL_3_IMPLEMENTED == 1
// TEST 3 (task 3., complete C API with semaphores, synchronous change of context)
#include "ConsumerProducer_C_API_test.hpp"
// TEST 4 (task 3, complete CPP API with traffic lights, synchronous context change)
#include "ConsumerProducer_CPP_Sync_API_test.hpp"
#endif

#if LEVEL_4_IMPLEMENTED == 1
// TEST 5 (task 4., thread_sleep test C API)
#include "ThreadSleep_C_API_test.hpp"
// TEST 6 (task 4. CPP API and asynchronous context change)
#include "ConsumerProducer_CPP_API_test.hpp"
#include "System_Mode_test.hpp"

#endif

void userMain() {
    printString("Enter test number? [1-7]\n");
    int test = getc() - '0';
    getc(); // Enter posle broja

    if ((test >= 1 && test <= 2) || test == 7) {
        if (LEVEL_2_IMPLEMENTED == 0) {
            printString("Task 2 is not stated to have been implemented\n");
            return;
        }
    }

    if (test >= 3 && test <= 4) {
        if (LEVEL_3_IMPLEMENTED == 0) {
            printString("Task 3 is not stated to have been implemented\n");
            return;
        }
    }

    if (test >= 5 && test <= 6) {
        if (LEVEL_4_IMPLEMENTED == 0) {
            printString("Task 4 is not stated to have been implemented\n");
            return;
        }
    }

    switch (test) {
        case 1:
#if LEVEL_2_IMPLEMENTED == 1
            Threads_C_API_test();
            printString("TEST 1 (task 2, C API threads and synchronous context switching)\n");
#endif
            break;
        case 2:
#if LEVEL_2_IMPLEMENTED == 1
            Threads_CPP_API_test();
            printString("TEST 2 (task 2, CPP API thread and synchronous context change)\n");
#endif
            break;
        case 3:
#if LEVEL_3_IMPLEMENTED == 1
            producerConsumer_C_API();
            printString("TEST 3 (task 3., complete C API with semaphores, synchronous change of context)\n");
#endif
            break;
        case 4:
#if LEVEL_3_IMPLEMENTED == 1
            producerConsumer_CPP_Sync_API();
            printString("TEST 4 (task 3, complete CPP API with traffic lights, synchronous context change)\n");
#endif
            break;
        case 5:
#if LEVEL_4_IMPLEMENTED == 1
            testSleeping();
            printString("TEST 5 (task 4., thread_sleep test C API)\n");
#endif
            break;
        case 6:
#if LEVEL_4_IMPLEMENTED == 1
            testConsumerProducer();
            printString("TEST 6 (task 4., thread_sleep test C API)\n");
#endif
            break;
        case 7:
#if LEVEL_2_IMPLEMENTED == 1
            System_Mode_test();
            printString("The test did not complete successfully\n");
            printString("TEST 7 (task 2, testing whether user code is executed in user mode)\n");
#endif
            break;
        default:
            printString("You have not entered the correct number for the test\n");
    }
}