;------------------------------------------------------------------------------
; this code is written based upon bandwidth 0.32
; written by Zack T Smith
;------------------------------------------------------------------------------

; target platform is 32bit
bits 32
; IA64 CPU(om x86 mode) instruction set
cpu ia64

global  memory_read_128bit
global  _memory_read_128bit
;------------------------------------------------------------------------------
; Name:    memory_read_128bit
; Purpose: Reads 128-bit values sequentially from an area of memory.
; Params:  [esp+4] = ptr to memory area
;          [esp+8] = length in bytes
;          [esp+12] = loops
;------------------------------------------------------------------------------
memory_read_128bit:
_memory_read_128bit:
push  ebx
push  ecx

; eax is the pointer to the start of the chunk,
; ebx is the pointer to the end of chunk
; ecx is counter
mov  eax, [esp+4+8]
mov  ebx, eax
add  ebx, [esp+8+8]
mov  ecx, [esp+12+8]

; L1 loop repeat reading memory
.L1:
mov  eax, [esp+4+8]

; L2 loop repeat the whole chunk of memory
.L2:
; Read aligned @ 16-byte boundary with stride of 64bytes
; movntdqa: nt means non-temporal, a hint
; that tells cpu not to use cache. However,
; in practise, CPU did use cache actually.
movntdqa  xmm0, [eax]
;movntdqa  xmm0, [16+eax]
;movntdqa  xmm0, [32+eax]
;movntdqa  xmm0, [48+eax]
movntdqa  xmm0, [64+eax]
;movntdqa  xmm0, [80+eax]
;movntdqa  xmm0, [96+eax]
;movntdqa  xmm0, [112+eax]

add  eax, 128
cmp  eax, ebx
jb  .L2; jump if eax < ebx

sub  ecx, 1
jnz  .L1; jump if ecx != 0

pop  ecx
pop  ebx
ret

global  mem_read_word_stride
global  _mem_read_word_stride
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
mem_read_word_stride:
_mem_read_word_stride:
push  ebx
push  ecx
push  edx

; eax is the pointer to the start of the chunk,
; ebx is the pointer to the end of chunk
; ecx is stride
mov  eax, [esp+4+12]
mov  ebx, eax
add  ebx, [esp+8+12]
mov  ecx, [esp+12+12]

.L3:
mov edx, [eax]
add eax, ecx
cmp eax, ebx
jb  .L3

pop  edx
pop  ecx
pop  ebx
ret

global  memory_read_byte
global  _memory_read_byte
;------------------------------------------------------------------------------
; Name:    memory_read_byte
; Purpose: Reads one byte from memory.
; Params:  [esp+4] = ptr to memory
;------------------------------------------------------------------------------
memory_read_byte:
_memory_read_byte:
push  ebx
push  ecx

; eax is the pointer to the start of the chunk,
; ebx is the pointer to the end of chunk
; ecx is counter
mov  eax, [esp+4+8]
mov  ebx, eax
mov  eax, [ebx]

pop  ecx
pop  ebx
ret

global  memory_write_128bit
global  _memory_write_128bit
;------------------------------------------------------------------------------
; Name:    memory_write_128bit
; Purpose: writes 128-bit values sequentially from an area of memory.
; Params:  [esp+4] = ptr to memory area
;          [esp+8] = length in bytes
;          [esp+12] = loops
;------------------------------------------------------------------------------
memory_write_128bit:
_memory_write_128bit:
push  ebx
push  ecx

; eax is the pointer to the start of the chunk,
; ebx is the pointer to the end of chunk
; ecx is counter
mov  eax, [esp+4+8]
mov  ebx, eax
add  ebx, [esp+8+8]
mov  ecx, [esp+12+8]

; L1 loop repeat writeing memory
.L1:
mov  eax, [esp+4+8]

; L2 loop repeat the whole chunk of memory
.L2:

mov [eax], ecx
mov [eax+64], ecx

add  eax, 128
cmp  eax, ebx
jb  .L2; jump if eax < ebx

sub  ecx, 1
jnz  .L1; jump if ecx != 0

pop  ecx
pop  ebx
ret

