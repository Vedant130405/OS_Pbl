/* kernel.c */
#include "./include/print.h"
#include <stdint.h>
#include "./include/mm.h"            /* provides kheap_init() and kmalloc() */

/* symbol provided by link.ld */
extern uint8_t _kernel_end;

/* TASK + STACK settings */
#define MAX_TASKS 3
#define STACK_SIZE 256        // simulated stack size

/* Heap size passed to heap init (1 MiB) */
#define KHEAP_SIZE (1024 * 1024)

/* ---------------------- PCB STRUCT ---------------------- */
typedef struct {
    unsigned char stack[STACK_SIZE];  // simulated stack
    int sp;                           // stack pointer
    void (*task)();                   // task function
    int ticks_left;                   // remaining CPU time
} PCB;

static PCB tasks[MAX_TASKS];
static int current = -1;

/* ---------------------- DELAY --------------------------- */
/* reduced so switching is visible during test runs */
void delay() {
    volatile unsigned long i;
    for (i = 0; i < 100000000UL; i++);  // ~1 second, adjust if needed
}
/* ---------------------- TASK FUNCTIONS ------------------ */
void task1() { print_msg("Task 1 running\n"); delay(); }
void task2() { print_msg("Task 2 running\n"); delay(); }
void task3() { print_msg("Task 3 running\n"); delay(); }

/* ---------------------- PUSH ---------------------------- */
void push(PCB *p, unsigned char value) {
    if (p->sp <= 0) return;        // avoid underflow
    p->stack[p->sp--] = value;
}

/* ---------------------- POP ----------------------------- */
unsigned char pop(PCB *p) {
    if (p->sp >= STACK_SIZE - 1)
        return 0;                 // avoid overflow
    return p->stack[++p->sp];
}

/* ------------------- INITIALIZE TASKS ------------------- */
void init_tasks() {
    tasks[0].task = task1; tasks[0].ticks_left = 3;
    tasks[1].task = task2; tasks[1].ticks_left = 3;
    tasks[2].task = task3; tasks[2].ticks_left = 3;

    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].sp = STACK_SIZE - 1;

        /* fake return address */
        push(&tasks[i], 0xF0 | i);

        /* fake register initial values */
        push(&tasks[i], 0xAA);  /* EAX */
        push(&tasks[i], 0xBB);  /* EBX */
        push(&tasks[i], 0xCC);  /* ECX */

        /* task ID */
        push(&tasks[i], i + 1);
    }
}

/* ---------------------- CHECK DONE ----------------------- */
int all_done() {
    for (int i = 0; i < MAX_TASKS; i++)
        if (tasks[i].ticks_left > 0)
            return 0;
    return 1;
}

/* ---------------------- SCHEDULER ------------------------ */
void scheduler() {

    do {
        current = (current + 1) % MAX_TASKS;
    } while (tasks[current].ticks_left == 0);

    PCB *p = &tasks[current];

    /* save context (simulated) */
    push(p, 0xDE);
    push(p, 0xAD);
    push(p, 0xBE);
    push(p, 0xEF);

    /* execute 1 tick */
    p->task();
    p->ticks_left--;

    /* restore context (simulated) */
    pop(p); pop(p); pop(p); pop(p);
}

/* ------------------------ KERNEL MAIN -------------------- */
void kernel_main() {
    clear_screen();
    print_msg("TEAM-NOT HUMANS T063\n");
    print_msg("Members: Ansh, Saurav, Vedant\n\n");

    /* STEP 4+: Initialize kernel heap (using mm.h API) */
    kheap_init(&_kernel_end, KHEAP_SIZE);
    print_msg("kheap initialized (1 MiB)\n");

    /* test allocation using external kmalloc() */
    char *msg = (char *)kmalloc(64);
    if (msg) {
        /* write a string into allocated memory and show it */
        const char *hello = "kmalloc working: Hello from heap!";
        int i = 0;
        while (hello[i] != '\0' && i < 63) {
            msg[i] = hello[i];
            i++;
        }
        msg[i] = '\0';

        print_msg("Allocated message: ");
        print_msg(msg);
        print_msg("\n");
    } else {
        print_msg("kmalloc failed: out of memory\n");
    }

    /* initialize tasks and run scheduler loop */
    init_tasks();

    while (!all_done()) {
        scheduler();
    }

    print_msg("\nAll tasks completed!\n");
}

