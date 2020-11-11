#include "jit_asm.h"
#include <stdio.h>

/*
	ekknod@2019
	- JIT_ASM was meant to be part of EC_PRO LAN cheat
	- idea:
		executing assembly code from text section, with nearly native code performance

	- this project did never finished, because i did not write all needed paylods stubs for JIT_ASM.


	future plans were:
		- make instruction indexes randomized by time ( instruction_list[index] )
		- implement it to EC_PRO LAN server packet_handler
*/


struct tcp_header {
	uint16_t size;
	uint8_t  retn, status;
} ;

struct tcp_entry {
	uint16_t next;
	uint16_t id;
} ;

struct tcp_entry_address {
	struct tcp_entry ehdr;
	uint64_t address;
};

struct tcp_entry_device {
	struct tcp_entry entry;
#if INTPTR_MAX == INT32_MAX
	uint32_t wow64fix;
#endif
	unsigned short Length;
	unsigned short MaximumLength;
	unsigned short Offset;
	short Buffer[60];
};

#define STACK_ADD_DEVICE(e,i,n,b,o) { \
	(e)->id = (i); \
	(e)->next = (n); \
	((struct tcp_entry_device *)e)->Offset = o; \
	((struct tcp_entry_device *)e)->Length = sizeof(b) - 2; \
	((struct tcp_entry_device *)e)->MaximumLength = sizeof(b); \
	memcpy(((struct tcp_entry_device *)e)->Buffer, b, sizeof(b)); \
	e = (struct tcp_entry *)((struct tcp_entry_device*)e + 1); \
}

#define STACK_CLEAR_FUNCTIONS(e, n) { \
	(e)->id = (2); \
	(e)->next = n; \
	e = (struct tcp_entry *)(e + 1); \
}

#define STACK_EMPTY_FUNCTION(e,n,i) { \
	(e)->id = (i); \
	(e)->next = (n); \
	e = (struct tcp_entry *)(e + 1); \
}

#define STACK_ADD_FUNCTION(e,d) { \
	(e)->id = 1; \
	(e)->next = 4 + sizeof(d); \
	memcpy(((char *)e + 4), d, sizeof(d)); \
	e = (struct tcp_entry *)(e + 1); \
	e = (struct tcp_entry *)((char *)e + sizeof(d)); \
}

#define STACK_EXECUTE(h, e) { \
	(h)->size = 4; \
	(h)->retn = 1; \
	(h)->status = 1; \
	e = (struct tcp_entry*)(h + 1); \
}


#define STACK_END_FUNCTION STACK_EMPTY_FUNCTION
#define STACK_GET_HEAP STACK_EMPTY_FUNCTION
#define STACK_ADD_GET_SYSTEM_PROCESS STACK_EMPTY_FUNCTION
#define STACK_ADD_PML4 STACK_EMPTY_FUNCTION



#include <string.h>
static char heap[8192];
static void *function_list[32];
static void *function_data[8192];

static int  function_count, function_size;

static void function_address(struct tcp_header *hdr, struct tcp_entry *ehdr)
{
	struct tcp_entry_address *entry = (struct tcp_entry_address*)ehdr;
	*(uint64_t*)((char *)hdr + hdr->size) = (uint64_t)function_list[entry->address];
	hdr->size += sizeof(uint64_t);
}

static void function_add(struct tcp_header *hdr, struct tcp_entry *ehdr)
{
	int len = ehdr->next - 4;

	if (len == -4)
		return;

	for (int i = 0; i < len; i++) {
		*(char*)((char *)function_data + function_size + i) =
			*(char*)((char *)ehdr + sizeof(struct tcp_entry) + i);
	}

	function_list[function_count++] = (void*)((char *)function_data + function_size);
	function_size += len;
}


static void function_clear(struct tcp_header *hdr, struct tcp_entry *ehdr)
{
	for (int i = 0; i < function_size; i++)
		((char *)function_data)[i] = 0;

	function_count = 5;
	function_size  = 0;
}

static void function_heap(struct tcp_header *hdr, struct tcp_entry *ehdr)
{
	*(uint64_t*)((char *)hdr + hdr->size) = (uint64_t)&heap;
	hdr->size += sizeof(uint64_t);
}

static void function_call(struct tcp_header *hdr, struct tcp_entry *ehdr)
{
	REG[RCX] = (uint64_t)hdr;
	REG[RDX] = (uint64_t)ehdr;
	asmjit((unsigned char *)function_list[ehdr->id]);
}

static void packet_routine(char *data)
{
	struct tcp_header *hdr = (struct tcp_header *)data;
	struct tcp_entry *ehdr = (struct tcp_entry *)(data + sizeof(struct tcp_header));
	while (1) {
		uint16_t next = ehdr->next;

		/* this could be done in better way, just had no time to implement it */
		if (ehdr->id < 4)
			((void (*)(struct tcp_header *, struct tcp_entry *))function_list[ehdr->id])(hdr, ehdr);
		else
			((void (*)(struct tcp_header *, struct tcp_entry *))function_list[4])(hdr, ehdr);

		if (next == 0)
			break;

		if (hdr->status == 0)
			break;

		ehdr = (struct tcp_entry *)((char *)ehdr + next);
	}
	if (hdr->retn) {
		/* simulating server */
		printf("[+] sending data back to client\nsize: %d, result: %d\n", hdr->size, hdr->retn);
	}
}

uint32_t asm_syscall(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,
	uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t);

#define EXEC_SYSCALL(eax, rcx, rdx, r8, r9, rsp) asm_syscall(rcx, rdx, r8, r9, eax, 0, \
	*(uint64_t*)(rsp), *(uint64_t*)(rsp + 8), *(uint64_t*)(rsp + 16), *(uint64_t*)(rsp + 24), *(uint64_t*)(rsp + 32), \
	*(uint64_t*)(rsp + 40), *(uint64_t*)(rsp +48))

static void syscall(void)
{
	REG[RAX] = EXEC_SYSCALL((uint32_t)REG[EAX], REG[RCX], REG[RDX], REG[R8Q], REG[R9Q], REG[RSP]);
}

int main(void)
{

	memset(function_list, 0, sizeof(function_list));
	function_list[function_count++] = function_address;
	function_list[function_count++] = function_add;
	function_list[function_count++] = function_clear;
	function_list[function_count++] = function_heap;
	function_list[function_count++] = function_call;


	unsigned char b_get_system_process[] = {
		68-2, 0,
		MOVR,       R10Q, RCX,
		MOVB,       EAX,  0x36,
		MOVB,       ECX,  0x10,
		MOVR,       RDX,  RSP,
		MOVB,       R8Q,  0x20,
		MOVR,       R9Q,  RDX,
		SYSCALL,
		MOVD,       R8D, 0x04, 0x00, 0x00, 0xC0,
		JNE,        EAX, R8D, 69-2, 0x00,
		MOVR_D,     R8D, RSP,
		MOVW,       EAX, 0x00, 0x20,
		ADDR,       R8D, EAX,
		MOVB,       EAX, 0x36,
		SYSCALL,
		JNZ,        EAX, 69-2, 0x00,
		MOVR_QO,    R9Q, R9Q, 0x10, 0x00,
		MOVR_W,     R8Q,  R10Q,
		BADD_B,     R10Q, 8,
		ADDR,       R10Q, R8Q,
		PMOVR,      R10Q, R9Q,
		CLR_RSP,
	} ;



	/* simulating opengl32 server */

	char payload[1400]={0};	
	struct tcp_header *hdr  = (struct tcp_header *)&payload;
	struct tcp_entry  *ehdr = (struct tcp_entry *)(hdr + 1);


	STACK_ADD_FUNCTION(ehdr, b_get_system_process);
	STACK_ADD_GET_SYSTEM_PROCESS(ehdr, 0, 5);
	STACK_CLEAR_FUNCTIONS(ehdr, 0);
	STACK_EXECUTE(hdr, ehdr);


	packet_routine(payload);
	printf("PsInitialSystemProcess %llx\n", *(uint64_t*)((char*)hdr + 4));


	getchar();
}

