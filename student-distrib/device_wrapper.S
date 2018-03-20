# device_wrapper.S - Set up the keyboard and rtc handler

.text
.globl keyboard_wrapper
.globl rtc_wrapper

# the keyboard and rtc wrapper that will call their related handler

keyboard_wrapper:
    pushal
    call keyboard_interrupt
    popal
    iret

rtc_wrapper:
    pushal
    call rtc_interrupt
    popal
    iret
