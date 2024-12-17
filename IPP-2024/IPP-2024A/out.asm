.586
.model flat, stdcall
includelib kernel32.lib
includelib libucrt.lib
includelib ../Debug/IPP-2024L.lib
includelib ../Debug/IPP-2024ASML.lib

ExitProcess proto : dword
SetConsoleTitleA proto: dword
GetStdHandle proto: dword
WriteConsoleA proto: dword, : dword, : dword, : dword, : dword
SetConsoleOutputCP proto : dword
SetConsoleCP proto : dword
overflow_error_message proto
division_by_zero_error_message proto
index_error_message proto
GetStringArrayElement proto
GetIntArrayElement proto
PrintUnsignedInt proto : dword
PrintUnsignedIntNewLine proto : dword
PrintInt proto : dword
PrintIntNewLine proto : dword
PrintBoolean proto : dword
PrintBooleanNewLine proto : dword
PrintConsole proto : dword
PrintConsoleNewLine proto : dword
extrn TIME : proc
extrn DATE : proc 

.stack 4096

.const
consoleTitle byte "IPP-2024", 0
strPause byte "pause", 0
emptyString byte " ", 0
L0 byte "Hello world!", 0
L1 sdword 21
L2 sdword 43
L3 sdword 3
L4 sdword -1
L5 sdword -2
L6 sdword -3
L7 sdword 1
L8 sdword 10
L9 sdword 9
L10 sdword 5
L11 sdword 11
L12 sdword 15
L13 sdword 10
L14 sdword 5
L15 sdword 4
L16 sdword 1
L17 sdword 2
L18 sdword 3
L19 sdword 4
L20 sdword 0
L21 sdword 4
L22 byte " - ", 0
L23 sdword 1

.data
res_mul dword 0
str_main dword offset emptyString
num1_main sdword 0
num2_main sdword 0
nums1_main sdword  -1, -2, -3
num40_main dword 0
num3_main dword 0
num4_main dword 0
nums_main sdword  1, 2, 3, 4
num5_main dword 0
cnt_main dword 0

.code
mul_global proc, a_mul : dword, b_mul : dword
	xor eax, eax
	mov eax, a_mul
	push eax
	mov eax, b_mul
	pop ebx
	mul ebx
	jo OVERFLOW_ERROR

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov res_mul, eax
	jmp NO_ERRORS

OVERFLOW_ERROR:
	call overflow_error_message

DIVISION_BY_ZERO_ERROR:
	call division_by_zero_error_message

NO_ERRORS:
	mov eax, res_mul
	ret 8
mul_global endp

main proc
	push 1251d
	call SetConsoleOutputCP
	push 1251d
	call SetConsoleCP

	push offset consoleTitle
	call SetConsoleTitleA
	push -11
	call GetStdHandle

	xor eax, eax
	mov eax, offset L0
	mov str_main, eax
	push str_main
	call PrintConsoleNewLine
	xor eax, eax
	mov eax, L1
	mov num1_main, eax
	xor eax, eax
	mov eax, L2
	mov num2_main, eax
	xor eax, eax
	push lengthof nums1_main
	push offset nums1_main
	push L7
	call GetIntArrayElement
	push [eax]
	pop eax

	push eax
	mov eax, L8
	pop ebx
	add eax, ebx
	jo OVERFLOW_ERROR

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov num40_main, eax
	push num40_main
	call PrintUnsignedIntNewLine
	mov eax, num1_main
	cmp eax, num2_main
	jle ELSE_0
	xor eax, eax
	call DATE
	mov str_main, eax
	jmp IF_0
ELSE_0:
	xor eax, eax
	call TIME
	mov str_main, eax
IF_0:
	push str_main
	call PrintConsoleNewLine
	xor eax, eax
	push L10
	push L9
	call mul_global
	push eax
	push L12
	push L11
	call mul_global
	pop ebx
	add eax, ebx
	jo OVERFLOW_ERROR

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov num3_main, eax
	xor eax, eax
	mov eax, L13
	push eax
	mov eax, L14
	pop ebx
	cmp eax, 0
	je DIVISION_BY_ZERO_ERROR
	push eax
	mov eax, ebx
	pop ebx
	xor edx, edx
	cmp eax, 0
	jl OVERFLOW_ERROR
	cmp ebx, 0
	jl OVERFLOW_ERROR
	div ebx
	jo OVERFLOW_ERROR

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov num4_main, eax
	xor eax, eax
	push num2_main
	push num1_main
	call mul_global

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov num5_main, eax
	push num5_main
	call PrintUnsignedIntNewLine
	xor eax, eax
	mov eax, L20
	cmp eax, 0
	jl OVERFLOW_ERROR


	cmp eax, 0
	jl OVERFLOW_ERROR
	mov cnt_main, eax
WHILE_0:
	mov eax, cnt_main
	cmp eax, L21
	jge END_WHILE_0
	push cnt_main
	call PrintUnsignedInt
	push offset L22
	call PrintConsole
	push lengthof nums_main
	push offset nums_main
	push cnt_main
	call GetIntArrayElement
	push [eax]
	call PrintIntNewLine
	xor eax, eax
	mov eax, cnt_main
	push eax
	mov eax, L23
	pop ebx
	add eax, ebx
	jo OVERFLOW_ERROR

	cmp eax, 0
	jl OVERFLOW_ERROR
	mov cnt_main, eax
	jmp WHILE_0
END_WHILE_0:
	jmp NO_ERRORS


OVERFLOW_ERROR:
	call overflow_error_message

DIVISION_BY_ZERO_ERROR:
	call division_by_zero_error_message

NO_ERRORS:
	push 0
	call ExitProcess
main endp

end main