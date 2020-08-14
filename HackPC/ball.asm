@27
D=A
@posX 
M=D
@46
D=A
@posY
M=D

@1
D=A
@velX
M=D
@15
D=A
@velY 
M=D

(mainLoop)	
	@posX 
	D=M 
	@invertX
	D;JLE

	@31 
	D=A
	@posX
	D=D-M
	@invertX
	D;JLE
	
	@notInvertX
	0;JMP
	(invertX)
	@velX
	M=-M
	(notInvertX)
	
	@posY
	D=M 
	@invertY
	D;JLE

	@240
	D=A
	@posY
	D=D-M
	@invertY
	D;JLE
	
	@notInvertY
	0;JMP
	(invertY)
	@velY
	M=-M
	(notInvertY)
	
	@posX
	D=M
	@drawSquareX
	M=D
	@posY
	D=M
	@drawSquareY
	M=D
	@drawSquareCol
	M=0
	
	@returnFromDraw0
	D=A 
	@drawSquareReturn
	M=D
	
	@drawSquare
	0;JMP
	(returnFromDraw0)

	@velX 
	D=M
	@posX 
	M=M+D
	@velY
	D=M
	@posY
	M=M+D
	
	@posX
	D=M
	@drawSquareX
	M=D
	@posY
	D=M
	@drawSquareY
	M=D
	@drawSquareCol
	M=-1
	
	@returnFromDraw1
	D=A 
	@drawSquareReturn
	M=D
	
	@drawSquare
	0;JMP
	(returnFromDraw1)
	
	@waitI
	M=0
	
	(waitLoop)
		@32767
		D=A 
		@waitI
		D=M-D 
		@mainLoop
		D;JGE
		
		@waitI
		M=M+1
		
		@waitLoop
		0;JMP



(drawSquare) // drawSquareX(0-31), drawSquareY(0-240), drawSquareCol(0, -1), drawSquareReturn (addr)
	
	@posX 
	D=M 
	@31 
	D=D-A
	@drawSquareReturn
	A=M
	D;JGT
	
	@posX 
	D=M 
	@drawSquareReturn
	A=M
	D;JLT
	
	@posY
	D=M 
	@drawSquareReturn
	A=M
	D;JLT
	
	@posY
	D=M 
	@240
	D=D-A
	@drawSquareReturn
	A=M
	D;JGT
	
	// pos = posX + posY*32
	@drawSquareY
	D=M
	@drawSquarePos
	M=D
	M=M+D
	D=M
	M=M+D
	D=M
	M=M+D
	D=M
	M=M+D
	D=M
	M=M+D
	@drawSquareX
	D=M
	@drawSquarePos
	MD=M+D

	@SCREEN
	D=A+D
	@drawSquarePos
	M=D
	
	@drawSquareIterator
	M=0 
	
	(drawSquareLoop)
	@drawSquareIterator
	D=M 
	@16 
	D=A-D 
	@drawSquareReturn
	A=M
	D;JEQ
	
	@drawSquareCol
	D=M 
	@drawSquarePos
	A=M
	M=D
	
	@32 
	D=A
	@drawSquarePos
	M=M+D
	
	@drawSquareIterator
	M=M+1 
	@drawSquareLoop
	0;JMP
	
