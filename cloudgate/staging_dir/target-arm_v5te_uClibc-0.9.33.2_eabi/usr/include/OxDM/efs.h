#ifndef OXDM_EFS_H
#define OXDM_EFS_H

//oflags
#define OXDM_EFS_O_RDONLY 00
#define OXDM_EFS_O_WRONLY 01
#define OXDM_EFS_O_RDWR   02
#define OXDM_EFS_O_CREAT  0100
#define OXDM_EFS_O_TRUNC  01000

/* efs_file_t is used as input/output of (some of) the EFS rpcs */
typedef struct{
	uint8_t file_exists; //output
	uint32_t oflag; //input
	uint32_t size; //input/output (optional on input)
	char source_path[256]; //input
	char target_path[256];
}efs_file_t;

#endif
