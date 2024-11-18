reset
		;Do the reset sequence and initialize the global data
		MOV		R1, #5
		;Jump to the main
		BL		main
		
		
main
		BL		subtract
		BL		terminate
		
subtract
		sub		R1, R1, #1
		CMP		R1, #0
		BNE		subtract
		mov		PC, LR
		
		
terminate
