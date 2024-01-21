//
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"


thread_t TCB::running = nullptr;
thread_t TCB::output = nullptr;
thread_t TCB::main = nullptr;
thread_t TCB::idle = nullptr;



uint64 TCB::timeSliceCounter = 0;

thread_t TCB::createThread( Body body, void *arg, uint64 *stack, bool runAtCreation)
{
    return new TCB(body, arg, stack, runAtCreation);
}

void TCB::outputThreadBody(void *) {

    while(true){
        while((*((char*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)){
            char c = Riscv::buffOUT->take();
            *((char*)CONSOLE_TX_DATA) = c;
        }
    }
}

void TCB::idleThreadBody(void *) {

    while(true){
        thread_dispatch();
    }
}

thread_t TCB::createOutputThread()
{
    if(!output)
    {
        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        output = createThread(outputThreadBody, nullptr, stack, true);
        output->sysThread = true;

    }
    return output;
}

thread_t TCB::createIdleThread()
{
    if(!idle)
    {
        uint64 *stack = (uint64*)__mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
        idle = createThread(idleThreadBody, nullptr, stack, true);
        idle->sysThread = true;
        idle->thread_status = IDLE;

    }
    return idle;
}

thread_t TCB::createMainThread()
{
    if(!main)
    {
        main = createThread(nullptr, nullptr, nullptr, false);
        main->sysThread = true;
        running = main;
    }
    return main;
}

void TCB::yield()
{
    __asm__ volatile ("mv a0, %0" : : "r" (THREAD_DISPATCH));
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    timeSliceCounter = 0;
    TCB *old = running;

    if (old->thread_status == RUNNING) {
        old->thread_status = READY;
        Scheduler::put(old);

    }
    running = Scheduler::get();
    if (running)
        running->thread_status = RUNNING;
    else
        running=idle;

    //

    Riscv::setPriviledge();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setThreadStatus(FINISHED);
    TCB::yield();
}

int TCB::start()
{
    if (this->getThreadStatus() != CREATED) {
        return -1;
    }
    this->thread_status = READY;
    Scheduler::put(this);
    return 0;
}

int TCB::exit()
{
    if (running->getThreadStatus() != RUNNING) {
        return -1;
    }
    running->thread_status = FINISHED;
    dispatch();
    return 0;
}

int TCB::sleep(time_t timeout)
{
    if(running->thread_status != RUNNING)
       return -1;


    running->thread_status=SLEEPING;
    SleepingThreads::insert(running, timeout);
    dispatch();

    return 0;
}

int TCB::wake()
{
    if(thread_status != SLEEPING)
        return -1;

    thread_status = READY;
    Scheduler::put(this);
    return 0;

}

void TCB::join(thread_t *handle)
{
    while((*handle)->getThreadStatus() != FINISHED)
        dispatch();
}




