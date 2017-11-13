;---
; bootloader.s
;---
	bits 16
	start: jmp boot

;; constant and variable definitions
msg db "Welcommen to mein operating system!", 0
_CurX db 0
_CurY db 0


MovCur:
	; dh = row
	; dl = col
	mov bh, 0 ; page number (0..7)
	mov ah, 2 ; ah = 2 - sets cursor position
	int 10h
	ret

ClrScr:
	mov dl, 0
	mov dh, 0
	call MovCur
	mov al, ' '
	mov bl, 0
	mov cx, 80*25
	call PutChar
	mov dl, 0
	mov dh, 0
	mov [_CurY], dh
	mov [_CurX], dl
	call MovCur
	ret

PutChar:
	; print character
	mov ah, 0ah ; ah = 0ah - write character at cursor position
	int 10h

	; Increment cursor position
	add [_CurX], cx
	mov dh, [_CurY]
	mov dl, [_CurX]
	call MovCur;
	ret

Print:
	call MovCur
	;mov ah, 0x0e	; print char service
	mov cx, 1	; number of times to display each character

.loop:
	lodsb		; AL <- [DS:SI] && SI++
	or	al, al	; end of string?
	jz	.done
	call PutChar;
	jmp 	.loop
.done:
	ret

boot:
	;Protected Mode
	cli; Disable interrupts
	cld; all that we need to init

	;call ClrScr
	;mov si, msg	; SI points to message
	;call Print

	; set video mode
	mov ah, 0x00; teletype output
	mov al, 0x03; vga 3
	mov bh, 0 ; page number (0..7)
	int 10h
	;write test char
	mov al, 0x00
	mov bh, 0 ; page number (0..7)
	int 10h

	;; Load main.c
	mov ax, 0x50

	;; set the buffer
	mov es, ax
	xor bx, bx

	mov al, 2	; read sector 2
	mov ch, 0	; track 0
	mov cl, 2	; sector to read
	mov dh, 0	; head number
	mov dl, 0	; drive number

	mov ah, 0x02	; read sectors from disk
	int 0x13	; call the BIOS routine

	;protected mode
	mov eax, cr0
	or al, 1
	;mov cr0, eax

	jmp [500h + 18h]	; jump and execute the sector

halt:

	hlt; halt the system

; we have to be 512 bytes. Clear the rest of the bytes with 0

	times 510 - ($-$$) db 0
	dw 0xAA55 ; Boot signiture
