initbuilder:
;MOV X0, #64
ADD X0, X0, X4
builder:
CMP X0, #0
B.EQ builddone
LDUR X1,[X3,#0]
LSL X2,X0,#3
ADD X2,X1,X2
STUR X2,[X2,#0]
SUBI X0,X0,#1
BR LR

swap: ; let's say that i is X20 and j is X19, let''s place array in X0. X9 is a temp'.
; need to change X20 and X19 to parameter registers X0...X7, because they are being passed
; into the function.
LSL X10, X5, #3
ADD X10, X1, X10
LSL X12, X6, #3
ADD X12, X1, X12

LDUR X9, [X10, #0] ;saves i to X9
LDUR X13, [X12, #0] ; saves j to X13
STUR X13, [X10, #0] ; loads X9 to X13
STUR X9, [X12, #0] ; loads X13 to X9

;
;
;
;

BR LR

smallest:
; return via X0, establish the length and starting index in parameter registers.
; assume X1 and X2, and X3 for i respectively for both, for now.
SUBI SP, SP, #48
STUR X30,[SP,#40];LR
STUR X23,[SP, #32] ;array

STUR X22,[SP,#24]; small, possibly get rid of

STUR X21,[SP,#16] ;current position
STUR X20,[SP,#8] ;n
STUR X19,[SP,#0] ;length

MOV X23, X0 ;array

MOV X21, X2 ; temp loop j
MOV X20, X2
MOV X19, X1

LSL X22, X2, #3
ADD X22, X0, X22; X10,22 is small.
ADDI X12, X10, #0; X12 is a[j]

;B forsmallreplace



forsmallreplace: SUBS X9, X19, X21
B.LE exitsmall
ADDI X21, X21, #1
LSL X12, X21, #3
ADD X12, X0, X12

ifsmall: SUBS X9, X22, X12
B.LE forsmallreplace
ADDI X22, X12, #0
ADDI X20, X21, #0
B forsmallreplace

exitsmall:
LDUR X19,[SP,#0]
LDUR X20,[SP,#8]
LDUR X21,[SP,#16]
LDUR X22,[SP,#24]
LDUR X23,[SP,#32]
LDUR X30,[SP,#40]
ADDI SP, SP, #48

BR LR

sort: 





iterate:
;assuming array,l,r,m
;as X0, X1, X2, X3
SUBI SP, SP, 40
STUR X30,[SP,#32]
STUR X22,[SP,#24]
STUR X21,[SP,#16]
STUR X20,[SP,#8]
STUR X19,[SP,#0]

MOV X19, X0
MOV X20, X1
MOV X21, X2
MOV X22, X3

B loopiter

loopiter:
SUBS X9, X20, X21
B.LT exititer1
SUB X10, X21,X20
ADDI X11, X11, #2
SDIV X10, X10, X11
ADD X10, X10, X20 ;X10 is mid

LSL X12, X10, #3 ;X12 is a[mid]
ADD X12, X0, X12
SUB X9, X12, X22
CBZ X9, exititer2
SUBS X9, X12, X22
B.LT exititer3
SUBI X21, X10, #1
BL loopiter

exititer1: ADDI X22, XZR, #0
ADDI X3, X22, #0
LDUR X19,[SP,#0]
LDUR X20,[SP,#8]
LDUR X21,[SP,#16]
LDUR X22,[SP,#24]
LDUR X30,[SP,#32]
ADDI SP, SP, #40
BR LR

exititer2:
ADDI X3, X22, #0
LDUR X19,[SP,#0]
LDUR X20,[SP,#8]
LDUR X21,[SP,#16]
LDUR X22,[SP,#24]
LDUR X30,[SP,#32]
ADDI SP, SP, #40
BR LR



exititer3:
ADDI X20, X10, #1
B loopiter

recurse: ;assume X0,1,2,3 as array,l,r,m
SUBS X9, X2, X1
B.LT done

done:
BR LR

body:
SUBI SP, SP, #32
STUR X21,[SP,#0]
STUR X20,[SP,#8]
STUR X19,[SP,#16]
STUR X30,[SP,#24]

ADD X19,X1,XZR
ADD X20,X2,XZR
ADD X21,X3,XZR

SUB X10, X21,X20
ADDI X11, X11, #2
SDIV X10, X10, X11
ADD X10, X10, X20 ;X10 is mid

LSL X12, X10, #3 ;X12 is a[mid]
ADD X12, X0, X12

SUB X13, X21,X12
CBZ X13, recursi
ADD X4, X10, XZR
BL body

SUBS X13, X21,X12



main:
MOV X4, #64
MOV X3, 
