
.global reset
reset:
    ldr  sp, =0x0011000
    bl main
stop: b stop
