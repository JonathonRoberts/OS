//struct {
///* General Volume Boot Record */
//unsigned short bytes_per_sector;
//unsigned char sectors_per_cluster;
//unsigned short reserved_sector_count;
//unsigned char number_of_fats;
//unsigned short max_root_dir_entries;
//unsigned short sectors_per_fat;
//unsigned int root_dir_sector;
//unsigned char fat_type[8];
//unsigned short signature;
//unsigned int fat_base;
//// What sector does the FATs start at
//unsigned int data_start_addr;
//// What address does the data regions start at
//} fat12;
//
//char buffer[512];
//unsigned char partition1_type;
//unsigned int partition1_begin_sector;
//unsigned int global_offset;
//
///* read MBR */
//seek_from_start(0);
//read_from_io(buffer, 1, 512);
//partition1_begin_sector = load_int((buffer+446+8));
//global_offset = partition1_begin_sector*512;
//
///* read VBR into buffer */
//seek_from_start(global_offset);
//read_from_io(buffer, 1, 512);
//
///* VBR: volume boot record */
//fat12.bytes_per_sector = load_short(buffer+0x0b);
//fat12.sectors_per_cluster = buffer[0x0d];
//fat12.reserved_sector_count = load_short(buffer+0x0e);
//fat12.number_of_fats = buffer[0x10];
//fat12.max_root_dir_entries = load_short(buffer+0x11);
//fat12.sectors_per_fat = load_short(buffer+0x16);
//fat12.signature = load_short(buffer+0x1fe);
///* At what sector does the fat table begin? */
//fat12.fat_base = partition1_begin_sector + fat12.reserved_sector_count;
///* At what sector does the root directory start at? */
//fat12.root_dir_sector = fat12.fat_base + fat12.sectors_per_fat*fat12.number_of_fats;
///* At what address does the data region start at? */
//fat12.data_start_addr = fat12.root_dir_sector*fat12.bytes_per_sector +
//fat12.max_root_dir_entries*32;


typedef struct fat16_vbr
{
	uint8_t 	bootjmp[3];
	uint8_t 	OEMIdentifier[8];
	uint16_t        BytesPerSector;
	uint8_t		SectorsPerCluster;
	uint16_t	ReservedSectorCount;
	uint8_t		TableCount;
	uint16_t	RootEntryCount;
	uint16_t	TotalSectors16;
	uint8_t		MediaDescriptor;
	uint16_t	SectorsPerFAT16;
	uint16_t	SectorsPerTrack;
	uint16_t	NumberofHeads;
	uint32_t	HiddenSectors;
	uint32_t	LargeTotalSectors32;
	//extended fat12 and fat16 stuff
	uint8_t		DriveNumber;
	uint8_t		Reserved1;
	uint8_t		ExtendedBootSig;
	uint32_t	OSSectors;
	uint8_t		SystemIdentifier[8];

}__attribute__((packed)) fat16_vbr_t;

typedef struct fat16_mbr
{
	uint8_t 	Status;
	uint8_t 	CHSFirstHead;
	uint8_t		CHSFirstSector;
	uint8_t		CHSFirstCylinder;
	uint8_t		PartitionType;
	uint8_t 	CHSLastHead;
	uint8_t		CHSLastSector;
	uint8_t		CHSLastCylinder;
	uint32_t	LBAofLastSector;
	uint32_t	TotalBlocksinPartition;

}__attribute__((packed)) fat16_mbr_t;

typedef struct fat16_dir
{
	uint8_t 	Filename[8];
	uint8_t 	Extention[3];
	uint8_t 	Attributes;
	uint8_t		RESERVED;
	uint8_t		CreateTimeTenth;
	uint16_t	CreateTime;
	uint16_t	CreateDate;
	uint16_t	LastAccDate;
	uint16_t	StartingCluster;
	uint16_t	LastWrtTime;
	uint16_t	LastWrtDate;
	uint16_t	FirstCluster;
	uint32_t 	Filesize;
}__attribute__((packed)) fat16_dir_t;

struct fat16_vbr VBR;
struct fat16_mbr Partition1;
struct fat16_dir RootFiletree[200];
//struct fat16_dir BootFiletree[30];

void init_FAT()
{
volatile uint8_t *fspointer8 = (volatile uint8_t*) 0x7c00;
volatile uint16_t *fspointer16;
volatile uint32_t *fspointer32;
	VBR.bootjmp[0]=*fspointer8++;
	VBR.bootjmp[1]=*fspointer8++;
	VBR.bootjmp[2]=*fspointer8++;
	VBR.OEMIdentifier[0]=*fspointer8++;
	VBR.OEMIdentifier[1]=*fspointer8++;
	VBR.OEMIdentifier[2]=*fspointer8++;
	VBR.OEMIdentifier[3]=*fspointer8++;
	VBR.OEMIdentifier[4]=*fspointer8++;
	VBR.OEMIdentifier[5]=*fspointer8++;
	VBR.OEMIdentifier[6]=*fspointer8++;
	VBR.OEMIdentifier[7]=*fspointer8++;
	fspointer16 = (volatile uint16_t*) fspointer8;
	fspointer8+=2;
	VBR.BytesPerSector=*fspointer16;
	VBR.SectorsPerCluster=*fspointer8++;
	fspointer16 = (volatile uint16_t*) fspointer8;
	fspointer8+=2;
	VBR.ReservedSectorCount=*fspointer16++;
	VBR.TableCount=*fspointer8++;
	fspointer16 = (volatile uint16_t*) fspointer8;
	fspointer8+=4;
	VBR.RootEntryCount=*fspointer16++;
	VBR.TotalSectors16=*fspointer16;
	VBR.MediaDescriptor=*fspointer8++;
	fspointer16 = (volatile uint16_t*) fspointer8;
	fspointer8+=6;
	VBR.SectorsPerFAT16=*fspointer16++;
	VBR.SectorsPerTrack=*fspointer16++;
	VBR.NumberofHeads=*fspointer16++;
	fspointer32 = (volatile uint32_t*) fspointer8;
	fspointer8+=8;
	VBR.HiddenSectors=*fspointer32++;
	VBR.LargeTotalSectors32=*fspointer32;
	VBR.DriveNumber=*fspointer8++;
	VBR.Reserved1=*fspointer8++;
	VBR.ExtendedBootSig=*fspointer8++;
	fspointer32 = (volatile uint32_t*) fspointer8;
	fspointer8+=4;
	VBR.OSSectors=*fspointer32;
	VBR.SystemIdentifier[0]=*fspointer8++;
	VBR.SystemIdentifier[1]=*fspointer8++;
	VBR.SystemIdentifier[2]=*fspointer8++;
	VBR.SystemIdentifier[3]=*fspointer8++;
	VBR.SystemIdentifier[4]=*fspointer8++;
	VBR.SystemIdentifier[5]=*fspointer8++;
	VBR.SystemIdentifier[6]=*fspointer8++;
	VBR.SystemIdentifier[7]=*fspointer8++;

	fspointer8=(volatile uint8_t*)0x7dee; /* Location of Final Partition table */

	Partition1.Status = *fspointer8++;
	Partition1.CHSFirstHead = *fspointer8++;
	Partition1.CHSFirstSector = *fspointer8++;
	Partition1.CHSFirstCylinder = *fspointer8++;
	Partition1.PartitionType = *fspointer8++;
	Partition1.CHSLastHead = *fspointer8++;
	Partition1.CHSLastSector = *fspointer8++;
	Partition1.CHSLastCylinder = *fspointer8++;
	fspointer32 = (volatile uint32_t*) fspointer8;
	Partition1.LBAofLastSector = *fspointer32++;
	Partition1.TotalBlocksinPartition = *fspointer32++;

	int i=0;
	fspointer32 = (volatile uint32_t*) 0x100000;
	for (i=0;i<200;i++){
		fspointer8 = (volatile uint8_t*)fspointer32;
		RootFiletree[i].Filename[0] = *fspointer8++;
		RootFiletree[i].Filename[1] = *fspointer8++;
		RootFiletree[i].Filename[2] = *fspointer8++;
		RootFiletree[i].Filename[3] = *fspointer8++;
		RootFiletree[i].Filename[4] = *fspointer8++;
		RootFiletree[i].Filename[5] = *fspointer8++;
		RootFiletree[i].Filename[6] = *fspointer8++;
		RootFiletree[i].Filename[7] = *fspointer8++;
		RootFiletree[i].Extention[0] = *fspointer8++;
		RootFiletree[i].Extention[1] = *fspointer8++;
		RootFiletree[i].Extention[2] = *fspointer8++;
		RootFiletree[i].Attributes = *fspointer8++;
		RootFiletree[i].RESERVED = *fspointer8++;
		RootFiletree[i].CreateTimeTenth = *fspointer8++;
		fspointer16 = (volatile uint16_t*) fspointer8;
		RootFiletree[i].CreateTime = *fspointer16++;
		RootFiletree[i].CreateDate = *fspointer16++;
		RootFiletree[i].LastAccDate = *fspointer16++;
		RootFiletree[i].StartingCluster = *fspointer16++;
		RootFiletree[i].LastWrtTime = *fspointer16++;
		RootFiletree[i].LastWrtDate = *fspointer16++;
		RootFiletree[i].FirstCluster = *fspointer16++;
		fspointer32 = (volatile uint32_t*) fspointer16;
		RootFiletree[i].Filesize = *fspointer32++;
		if(RootFiletree[i].Filename[0] == 'B')
		if(RootFiletree[i].Filename[1] == 'O')
		if(RootFiletree[i].Filename[2] == 'O')
		if(RootFiletree[i].Filename[3] == 'T')
		if(RootFiletree[i].Filename[4] == ' ')
		if(RootFiletree[i].Filename[5] == ' ')
		if(RootFiletree[i].Filename[6] == ' ')
		if(RootFiletree[i].Filename[7] == ' ')
			kputs(0x2, 'Y');

		//kputs(0x2,Filetree[i].Filename[0]);
		//kputs(0x2,Filetree[i].Filename[1]);
		//kputs(0x2,Filetree[i].Filename[2]);
		//kputs(0x2,Filetree[i].Filename[3]);
		//kputs(0x2,Filetree[i].Filename[4]);
		//kputs(0x2,Filetree[i].Filename[5]);
		//kputs(0x2,Filetree[i].Filename[6]);
		//kputs(0x2,Filetree[i].Filename[7]);
		//kputs(0x2,Filetree[i].Extention[0]);
		//kputs(0x2,Filetree[i].Extention[1]);
		//kputs(0x2,Filetree[i].Extention[2]);
		//kputs(0x2,'\n');
//		write_serial(Filetree[i].Filename[0]);
//		write_serial(Filetree[i].Filename[1]);
//		write_serial(Filetree[i].Filename[2]);
//		write_serial(Filetree[i].Filename[3]);
//		write_serial(Filetree[i].Filename[4]);
//		write_serial(Filetree[i].Filename[5]);
//		write_serial(Filetree[i].Filename[6]);
//		write_serial(Filetree[i].Filename[7]);
//		write_serial(Filetree[i].Extention[0]);
//		write_serial(Filetree[i].Extention[1]);
//		write_serial(Filetree[i].Extention[2]);
//		write_serial('\n');
	}
	//}

	return;
}

