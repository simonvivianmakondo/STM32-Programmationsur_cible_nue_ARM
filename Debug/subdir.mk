################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test1.c \
../test2.c 

S_UPPER_SRCS += \
../boot.S 

C_DEPS += \
./test1.d \
./test2.d 

OBJS += \
./boot.o \
./test1.o \
./test2.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.S subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-gcc -g -c -mcpu=cortex-m7 -mfloat-abi=softfp -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -O0 -g3 -Wall -c -fmessage-length=0 -ffreestanding -mcpu=cortex-m7 -mfloat-abi=softfp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./boot.o ./test1.d ./test1.o ./test2.d ./test2.o

.PHONY: clean--2e-

