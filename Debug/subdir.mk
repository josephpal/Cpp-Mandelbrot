################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HelperFunctions.cpp \
../Mandelbrot.cpp \
../PPMImage.cpp \
../main.cpp 

OBJS += \
./HelperFunctions.o \
./Mandelbrot.o \
./PPMImage.o \
./main.o 

CPP_DEPS += \
./HelperFunctions.d \
./Mandelbrot.d \
./PPMImage.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


