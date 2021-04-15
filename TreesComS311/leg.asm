/*store sample five elements in the main memory*/ 
       ADDI X0,XZR,#5   //initialize x0 to 5
Loop:STUR X0, [X1, #0] //stor that value to memeory location
      ADDI X1,X1,#8    //increase x1 by 8 to get the next value
      SUBI X0,X0,#1   //decerement x0 by 1
      CBZ X1,XZR     //if x1 is zero branch loop and exit 
B Loop
Exit:
//DUMP
swap: CBZ X0, NullPointer  //X0 =0 go to error taken from the text book
	LDUR X10, [X0,-8] //set X10 to the length of the array
	CMP X1, XZR          //if i <0 got to error
	B.LT IndexOutOfBounds 
	CMP X1, X10
	B.GT IndexOutOfBounds 
	ADDI X9, X1, 1
	CMP X9, XZR
	B.LT IndexOutOfBounds 
	CMP X9, X10
	B.GT IndexOutOfBounds  
	LSL X10, X1,#3    
	ADD X10, X0,X10   //compute address of arr[i]
	LDUR X9, [X10,#0] //get arr[i]
	LDUR X11,[X10,#8]  //get arr[i+1] 
	STUR X11,[X10,#0] //save new vlaue arr[i]
	STUR X9, [X10,#8] //save new value to arr[i+1]
 	BR X10             //return from this function
smallinde: MOV X10,XZR //create space for the smallest value of the array
	   MOV X19,XZR
     Loop: CMP X19,X1 //compare i to the length of array
           B.GE exit //go to exit if the value array size
	   ADDI X19,X19,#1 //i=i+1
	   STUR X21,[X19,#0] //x21 =arr[i]
	   CMP X21,X10
	   CBNZ X21 //if x21 is greater then check the next
	   ELSE:
	    LDUR X19,[X21,#0]
	    CBZ X19, X10 //set the index value of the current array vlaue
	    BR LR 
	   	      
sort:   MOV X9,XZR   //i=0 
	CMP X19,X1  //compare if the value of i greater than arr.len
	ADD X11, X0, X10  //X11 =address of arr[i]
	 B.GT exit   //if it is true exit
	BL samllinde     //other wise find the smallest value's index
	MOV X0, X21 //initializing 
	MOV X1, X20 //initializing 
	BL swap       //call swap
	SUBI X20, X20, #1
quickR:  SUBI SP,SP,#16 //save a return address
	 SUBI SP,SP,#16 //save a return address for the right of the array 
	 STUR LR,[SP,#8]
 	 STUR X0,[SP,#0]
	 SUBI SP,SP,#16 //save a return address for the left part of the array
	 STUR LR,[SP,#8]
 	 STUR X1,[SP,#0]
	 STUR LR,[SP,#8]
	 STUR X2,[SP,#0]
	 SUB XZR,X0,X1   //compare the right and the left
	 B.GE L1          //if true go to L1
	L1:
	   SUB X9,X0,X1  // right minus left
	   ADD X20,X1,X9  //left plus right minus left
	   DIV X21,X20,#2    // devide by 2
 	ELSE: 
	B L2
	L2: ADD X16,X0,X1
	  STUR X21,[X16,#0]  //set register 21 to the mid value 
	 BR LR          //return 
	 ELSE:
	  B L3
	  L3:LDUR X21,(SP,#16)
	   SUBI X3,x1,#1  
             
     	    BR LR
	    //DUMP
 //not done yet


	  	      
	  


		   
