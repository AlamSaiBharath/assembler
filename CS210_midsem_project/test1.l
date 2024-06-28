00000000 		  ; Code to find max from first n elements of memory array
00000000 		  ;
00000000 		  ; Writing the memory
00000000 00000000 ldc 0
00000001 00000000 ldc 0
00000002 		  ; A = 0, B = 0
00000002 00000600 ldc 6
00000003 00000000 ldc 0
00000004 00000005 stnl 0
00000005 00000700 ldc 7
00000006 00000000 ldc 0
00000007 00000105 stnl 1
00000008 00000400 ldc 4
00000009 00000000 ldc 0
0000000a 00000205 stnl 2
0000000b 00000800 ldc 8
0000000c 00000000 ldc 0
0000000d 00000305 stnl 3
0000000e 		  ;
0000000e 		  ; 9998 = loop, 9997 = neg, 9996 = max, 9995 = ind, 9994 = tmp
0000000e ffffff00 ldc -1
0000000f fffffc03 stl -4 			; memory[ind] = -1
00000010 00000000 ldc 0
00000011 00000000 ldc 0
00000012 00000004 ldnl 0 			; A = memory[0]
00000013 fffffd03 stl -3 			; memory[max] = memory[0], A = 0, B = 0
00000014 		  ;
00000014 		  ; Starting the program
00000014 fffffc00 ldc -4
00000015 		  Loop:
00000015 00000101 adc 1
00000016 0000050d call Compare	; B = neg, A = loop, PC = Compare
00000017 fffffd10 brlz Loop
00000018 fffffd02 ldl -3 			; A = memory[max]
00000019 00000000 ldc 0 			; A = 0, B = max
0000001a 00000005 stnl 0 			; memory[0] = max
0000001b 00000012 HALT
0000001c 		  ;
0000001c 		  ;	A = loop, B = neg
0000001c 		  Compare:
0000001c ffffff03 stl -1 		; 	memory[loop] = A, A = neg, B = neg
0000001d fffffe03 stl -2		;	memory[neg] = A, A = neg, B = neg
0000001e fffffd02 ldl -3		; 	A = memory[max], B = neg
0000001f fffffc02 ldl -4		; 	A = memory[ind]+1, B = max
00000020 00000101 adc 1
00000021 fffffc03 stl -4		;	memory[ind] = A = currInd, A = max, B = max
00000022 fffffc02 ldl -4		; 	A = memory[ind], B = max
00000023 00000004 ldnl 0 		;   A = memory[i], B = earlier max
00000024 00000007 sub			; 	A = B - A
00000025 00000310 brlz Store	; 	if (A < 0) Store
00000026 fffffe02 ldl -2		;	A = memory[neg], A = neg, B = earlier max
00000027 ffffff02 ldl -1		;	A = memory[loop], A = loop, B = B - A
00000028 0000000e return		;   A = neg, B = neg
00000029 		  ;
00000029 		  ; A = B - A, B = earlier max
00000029 		  Store:
00000029 fffffc02 ldl -4		;	A = memory[ind], A = earlier max, B = earlier max
0000002a 00000004 ldnl 0		;	A = memory[i], B = earlier max
0000002b fffffd03 stl -3		;	memory[max] = A, A = earlier max, B = earlier max
0000002c fffffe02 ldl -2		;	A = memory[neg], B = max
0000002d ffffff02 ldl -1		;	A = memory[loop], A = loop, B = neg
0000002e 0000000e return		;   A = neg, B = neg
