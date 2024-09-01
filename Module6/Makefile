all:
	gcc -Wall -pedantic test.c
	
asm:
	gcc -O0 -S test.c
	
arm:
	arm-none-eabi-gcc -O0 -Wall -mcpu=cortex-m4 --specs=nosys.specs -mfloat-abi=soft -mthumb -fverbose-asm -S test.c