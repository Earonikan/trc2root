#include <read_waveform.h>

void ReadString(char str1[],char *Addr) {
	
	for (int i=0;i<16;i++) str1[i] = *(Addr + i);
	
}

void ReadWord(int8_t *ans, char *Addr) {
	
	*ans = *(int8_t*)(Addr);
		
}

void ReadWord(int16_t *ans, char *Addr) {
	
	*ans = *(int16_t*)(Addr);
		
}

void ReadLong(int32_t *ans, char *Addr) {
	
	*ans = *(int32_t*)(Addr);
		
}

void ReadFloat(float_t *ans, char *Addr) {
	
	*ans = *(float_t*)(Addr);
		
}

void ReadDouble(double_t *ans, char *Addr) {
	
	*ans = *(double_t*)(Addr);
		
}

int readwfm_b(int n_ch, int chans, Float_t *wave_x, Float_t *wave_y, Float_t *points, char filename[]) {
	
	FILE *fid;
	char *buffer,*WAVEDESC;
	long lSize;
	size_t result;
	
	int32_t WAVE_DESCRIPTOR,USER_TEXT,WAVE_ARRAY_1;
	int32_t WAVE_ARRAY_COUNT,TRIGTIME_ARRAY;
	int16_t WAVE_SOURCE,COMM_TYPE;
	float_t VERTICAL_GAIN,VERTICAL_OFFSET,HORIZ_INTERVAL;
	double_t HORIZ_OFFSET;
	
	// open file
	fid = fopen(filename,"rb");
	if (fid==NULL) {fputs("ERROR: file not found!",stderr); return 1;}
	
	// obtain file size:
	fseek(fid,0,SEEK_END);
	lSize = ftell(fid);
	rewind(fid);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); return 1;}

	// copy the file into the buffer:
	result = fread(buffer,1,lSize,fid);
	if (result != lSize) {fputs ("Reading error",stderr); return 1;}


	// find and access to data
	WAVEDESC = strstr(buffer,"WAVEDESC");
	char *aCOMM_TYPE			= WAVEDESC	+ 32;
	char *aWAVE_DESCRIPTOR		= WAVEDESC	+ 36;
	char *aUSER_TEXT			= WAVEDESC	+ 40;
	char *aTRIGTIME_ARRAY     	= WAVEDESC	+ 48;
	char *aWAVE_ARRAY_1			= WAVEDESC	+ 60;
	char *aWAVE_ARRAY_COUNT		= WAVEDESC	+ 116;
	char *aVERTICAL_GAIN		= WAVEDESC	+ 156;
	char *aVERTICAL_OFFSET		= WAVEDESC	+ 160;
	char *aHORIZ_INTERVAL     	= WAVEDESC	+ 176;
	char *aHORIZ_OFFSET			= WAVEDESC	+ 180;
	char *aWAVE_SOURCE			= WAVEDESC	+ 344;

	
	ReadWord(&WAVE_SOURCE,aWAVE_SOURCE);
	ReadFloat(&VERTICAL_GAIN,aVERTICAL_GAIN);
	ReadFloat(&VERTICAL_OFFSET,aVERTICAL_OFFSET);
	ReadFloat(&HORIZ_INTERVAL,aHORIZ_INTERVAL);
	ReadDouble(&HORIZ_OFFSET,aHORIZ_OFFSET);
	ReadWord(&COMM_TYPE, aCOMM_TYPE);
	ReadLong(&WAVE_DESCRIPTOR, aWAVE_DESCRIPTOR);
	ReadLong(&USER_TEXT, aUSER_TEXT);
	ReadLong(&WAVE_ARRAY_1, aWAVE_ARRAY_1);
	ReadLong(&WAVE_ARRAY_COUNT, aWAVE_ARRAY_COUNT);
	ReadLong(&TRIGTIME_ARRAY, aTRIGTIME_ARRAY);

	rewind(fid);
	fseek(fid,(WAVEDESC-buffer) + WAVE_DESCRIPTOR + USER_TEXT + TRIGTIME_ARRAY,SEEK_SET);
	
	int16_t wave_y0[WAVE_ARRAY_COUNT];
		
	if (COMM_TYPE == 0) fread(wave_y0,1,WAVE_ARRAY_COUNT,fid);
	else fread(wave_y0,2,WAVE_ARRAY_COUNT,fid);
	
	for (int i=0;i<WAVE_ARRAY_COUNT;i++) {
		
		*(wave_y+n_ch+chans*i) = VERTICAL_GAIN*wave_y0[i]-VERTICAL_OFFSET;
		*(wave_x+n_ch+chans*i) = i*HORIZ_INTERVAL+HORIZ_OFFSET;
		*(points+n_ch+chans*i) = i;
		//printf("\npoint number is %d, channel is %d",i,n_ch);
		
	}
		
	// terminate
	fclose(fid);
	free(buffer);
	
	return 0;

}

int32_t readinfo_b(char filename[]) {
	
	FILE *fid;
	char *buffer,*WAVEDESC;
	long lSize;
	size_t result;
	int32_t WAVE_ARRAY_COUNT;
	
	// open file
	fid = fopen(filename,"rb");
	if (fid==NULL) {fputs("ERROR: file not found! \n",stderr); exit(1);}
	
	// obtain file size
	fseek(fid,0,SEEK_END);
	lSize = ftell(fid);
	rewind(fid);

	// allocate memory to contain the whole file
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit(1);}

	// copy the file into the buffer
	result = fread(buffer,1,lSize,fid);
	if (result != lSize) {fputs ("Reading error",stderr); exit(1);}

	// find and access to data
	WAVEDESC = strstr(buffer,"WAVEDESC");
	char *aWAVE_ARRAY_COUNT	= WAVEDESC	+ 116;
	ReadLong(&WAVE_ARRAY_COUNT, aWAVE_ARRAY_COUNT);

	// terminate
	fclose(fid);
	free(buffer);

	return WAVE_ARRAY_COUNT;

}
