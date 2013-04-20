.global _start
.extern __ta_main
_start:
bl __ta_main
# TODO: DO KILL SYSCALL
# loop in case we haven't yet rescheduled
bl .
