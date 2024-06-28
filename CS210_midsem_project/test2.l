00000000 		  ;   sum of first 5 numbers and storing that to memory[0]
00000000 		  ; 	9998 = loop
00000000 		  ; 	9997 = neg, value of A
00000000 		  ; 	9996 = new
00000000 		  ; 	9995 = sum
00000000 fffffc03 stl -4 		;   Memory[sum] = 0
00000001 00000100 ldc 1 		;   A = 1, B = 0
00000002 fffffd03 stl -3 		; 	Memory[new] = 1
00000003 		  ; 	Starting the program
00000003 fffffb00 ldc -5
00000004 		  loop:
00000004 00000101 adc 1
00000005 0000050d call sum 	; B = neg, A = loop, PC = sumLabel
00000006 fffffd10 brlz loop
00000007 fffffc02 ldl -4		; A = memory[sum], B = neg
00000008 00000000 ldc 0 		; A = 0, B = sum
00000009 00000005 stnl 0 		; memory[0] = sum
0000000a 00000012 HALT
0000000b 		  ; 	A = loop, B = neg here due to call
0000000b 		  sum:
0000000b ffffff03 stl -1 		;	memory[loop] = A, A = neg, B = neg
0000000c fffffe03 stl -2 		;	memory[neg] = A, A = neg, B = neg
0000000d fffffd02 ldl -3 		; 	A = memory[new], B = neg
0000000e fffffc02 ldl -4 		;	A = memory[sum], B = new
0000000f 00000006 add 		; 	A = A + B, A = new sum
00000010 fffffc03 stl -4 		; 	memory[sum] = A, A = new, B = new
00000011 00000101 adc 1		; 	A = new
00000012 fffffd03 stl -3 		;	memory[new] = A, A = new, B = new
00000013 fffffe02 ldl -2 		;	A = memory[neg], B = new
00000014 ffffff02 ldl -1		; 	A = memory[loop], B = neg
00000015 0000000e return 		;	A = neg, B = neg
