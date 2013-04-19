
%.o: %.c
	$(arm-cc-command)
	
%.o: %.S
	$(arm-as-command)