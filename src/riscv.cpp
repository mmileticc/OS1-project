//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"
#include "../h/ABIWrappers.hpp"


void Riscv::sysCallWraper(...){
    __asm__ volatile("ecall");
}

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::popA0(){
    uint64 scause = r_scause();
    if (!(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)){
        __asm__ volatile ("ld x10, 10 * 8(s0)"); //returning value of a0 if it dont contain return value
    }
}

void Riscv::handleSupervisorTrap()
{
    // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

    volatile uint64 scause = r_scause();
    volatile uint64 a0 = r_a0();  //sys code
    volatile uint64 a1 = r_a1();  //1. arg
    volatile uint64 a2 = r_a2();  //2. arg
    volatile uint64 a3 = r_a3();  //3. arg
    volatile uint64 a4 = r_a4();  //4. arg
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        volatile bool dispatch = true;
        switch (a0) {
            case memAlloc:
                ABI::memAllocWrapper(a1);
                dispatch = false;
                break;
            case memFree:
                ABI::memFreeWrapper(a1);
                dispatch = false;
                break;
            case threadCreate:
                ABI::threadCreateWrapper(a1, a2, a3, a4);
                dispatch = false;
                break;
            case threadExit:
                ABI::threadExitWrapper();
                dispatch = false;
                break;
            case threadDispatch:
                //__asm__ volatile ("ld x10, 10 * 8(s0)");
                break;
            case semOpen:
                ABI::semOpenWrapper(a1,a2);
                dispatch = false;
                break;
            case semClose:
                ABI::semCloseWrapper(a1); dispatch = false;
                break;
            case semWait:
                ABI::semWaitWrapper(a1); dispatch = false;
                break;
            case semSignal:
                ABI::semSignalWrapper(a1); dispatch = false;
                break;
            case semTrywait:
                ABI::semTryWaitWrapper(a1); dispatch = false;
                break;
            case putC:
                __putc((char)a1);
                //ABI::putcWrapper(a1);
                dispatch = false;
                break;
            case getC:
                //dispatch = false;
                __getc();
                //ABI::getcWrapper();
                break;

        }


        if(dispatch){
            //saving a0
            __asm__ volatile ("addi sp, sp, -16");
            __asm__ volatile ("sd x10, (sp)");
            TCB::dispatch();
            //returning a0
            __asm__ volatile ("ld x10, (sp)");
            __asm__ volatile ("addi sp, sp, 16");
        }else{
            __asm__ volatile("sd x10, 80(fp)");
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);  //saying that interrupt is processed

    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard).
        console_handler();
    }
    else
    {
        // unexpected trap cause
        uint64 volatile sepc = r_sepc();
        printString("scause: ");
        printInt(scause,2,0);
        printString("\n");

        printString("sepc: ");
        printInt(sepc,2,0);
        printString("\n");
    }
}