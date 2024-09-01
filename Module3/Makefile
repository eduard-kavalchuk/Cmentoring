all:
	gcc -Wall isodd.c
	
asm:
	gcc -S isodd.c
	
arm:
	arm-none-eabi-gcc -Wall -mcpu=cortex-m4 --specs=nosys.specs -mfloat-abi=soft -mthumb -fverbose-asm -S isodd.c