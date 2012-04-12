/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define PROT_READ 0x1  
#define PROT_WRITE 0x2  
#define PROT_EXEC 0x4  
#define PROT_SEM 0x8  
#define PROT_NONE 0x0  
#define PROT_GROWSDOWN 0x01000000  
#define PROT_GROWSUP 0x02000000  

#define MAP_SHARED 0x01  
#define MAP_PRIVATE 0x02  
#define MAP_TYPE 0x0f  
#define MAP_FIXED 0x10  
#define MAP_ANONYMOUS 0x20  

#define MS_ASYNC 1  
#define MS_INVALIDATE 2  
#define MS_SYNC 4  

#define MADV_NORMAL 0  
#define MADV_RANDOM 1  
#define MADV_SEQUENTIAL 2  
#define MADV_WILLNEED 3  
#define MADV_DONTNEED 4  

#define MADV_REMOVE 9  
#define MADV_DONTFORK 10  
#define MADV_DOFORK 11  

#define MAP_ANON MAP_ANONYMOUS
#define MAP_FILE 0


struct output_file;

struct output_file *open_output_file(const char *filename, int gz, int sparse, unsigned long partstart, unsigned long blocksize);
void write_data_block(struct output_file *out, u64 off, u8 *data, int len);
void write_data_file(struct output_file *out, u64 off, const char *file,
		     off_t offset, int len);
void pad_output_file(struct output_file *out, u64 len);
void close_output_file(struct output_file *out);
