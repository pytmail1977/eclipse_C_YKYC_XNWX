################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/YC.cpp \
../src/YK.cpp \
../src/YKYC.cpp \
../src/commonTool.cpp \
../src/global.cpp 

OBJS += \
./src/YC.o \
./src/YK.o \
./src/YKYC.o \
./src/commonTool.o \
./src/global.o 

CPP_DEPS += \
./src/YC.d \
./src/YK.d \
./src/YKYC.d \
./src/commonTool.d \
./src/global.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/home/pyt64/timesys/i_MX6QSABRELite/toolchain/bin/armv7l-timesys-linux-gnueabi-g++ -I/home/pyt64/mysql-build/libmysqlclient-deb/include/mysql -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


