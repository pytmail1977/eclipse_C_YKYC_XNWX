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
	/media/sda1/771/opt/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-fsl-linux-gnueabi-g++ -I/home/pyt64/mysql-build/libmysqlclient-deb/include/mysql -O0 -g3 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


