typedef struct
{
	uint8_t		pwmValues[3];
	uint32_t	freqValues[3];

	uint8_t		digitOutValues[16];

	int16_t		tempValues[8];

	uint16_t	dacChannelValues[8];
}kTester_type;


extern kTester_type allData;
