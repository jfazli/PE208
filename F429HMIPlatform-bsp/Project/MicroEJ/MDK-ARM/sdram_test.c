uint8_t sdram_test(void){
	uint16_t data, j;
	uint8_t flag = 0;
	
	SDRAM_Init();
	
	data = 0;
	j = 0;
	for(i = 0; i < 0x100000; i += 2)
	{
		SDRAM_WriteBuffer(&data,i,1);
	}

	
	data = 0x5555;
	j = 0;
	for(i = 0; i < 0x100000; i += 2)
	{
		data = j;
		SDRAM_WriteBuffer(&data,i,1);
		j++;
	}
	
	data = 0;
	j = 0;
	for(i = 0; i < 0x100000; i += 2)
	{
		data = 0;
		SDRAM_ReadBuffer(&data,i,1);
		if(data != j)
		{
			flag = 1;
		}
		j++;
	}
	if(flag == 1)
	{
		printf("Test RAM failed\n");
		return flag;
	}
	
	data = 0xFFFF;
	j = 0;
	for(i = 0; i < 0x100000; i += 2)
	{
		SDRAM_WriteBuffer(&data,i,1);
	}
	return flag;
}