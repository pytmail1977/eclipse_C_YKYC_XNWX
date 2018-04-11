################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/YC.cpp \
../src/YK.cpp \
../src/YKYC.cpp \
../src/YKonXNWX.cpp \
../src/commonTool.cpp \
../src/global.cpp 

OBJS += \
./src/YC.o \
./src/YK.o \
./src/YKYC.o \
./src/YKonXNWX.o \
./src/commonTool.o \
./src/global.o 

CPP_DEPS += \
./src/YC.d \
./src/YK.d \
./src/YKYC.d \
./src/YKonXNWX.d \
./src/commonTool.d \
./src/global.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/Cellar/mysql/5.7.18_1/include/mysql -I/home/pyt64/mysql-connector-c-6.1.5-src/include -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


