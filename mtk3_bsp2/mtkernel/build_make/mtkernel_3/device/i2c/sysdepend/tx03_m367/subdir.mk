################################################################################
# micro T-Kernel 3.00.03  makefile
################################################################################

OBJS += \
./mtkernel_3/device/i2c/sysdepend/tx03_m367/i2c_m367.o 

C_DEPS += \
./mtkernel_3/device/i2c/sysdepend/tx03_m367/i2c_m367.d 


# Each subdirectory must supply rules for building sources it contributes
mtkernel_3/device/i2c/sysdepend/tx03_m367/%.o: ../device/i2c/sysdepend/tx03_m367/%.c
	@echo 'Building file: $<'
	$(GCC) $(CFLAGS) -D$(TARGET) $(INCPATH) -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
