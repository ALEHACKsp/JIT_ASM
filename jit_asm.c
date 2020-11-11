#define _CRT_SECURE_NO_WARNINGS
#include "jit_asm.h"
#include <intrin.h>
#include <stdio.h>

static int32_t g_next_index;
static uint8_t stack_rsp[11743120] ;

uint64_t REG[16] = {
	0, 0, 0, 0, (uint64_t)stack_rsp, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
} ;

static void mov_gs(char *data)
{
	REG[*(uint8_t *)(data)] = __readgsqword(*(uint16_t *)(data + 1));
	g_next_index += 4;
}

static void movb(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = r[1];
	g_next_index += 3;
}

static void movw(char *data)
{
	REG[*(uint8_t *)(data)] = *(uint16_t *)(data + 1);
	g_next_index += 4;
}

static void movd(char *data)
{
	REG[*(uint8_t *)(data)] = *(uint32_t *)(data + 1);
	g_next_index += 6;
}

static void movq(char *data)
{
	REG[*(uint8_t *)(data)] = *(uint64_t *)(data + 1);
	g_next_index += 10;
}

static void movr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = REG[r[1]];
	g_next_index += 3;
}

static void movr_bptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)REG[r[1]];
	g_next_index += 3;
}

static void movr_wptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)REG[r[1]];
	g_next_index += 3;
}

static void movr_dptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)REG[r[1]];
	g_next_index += 3;
}

static void movr_qptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)REG[r[1]];
	g_next_index += 3;
}

static void movr_bm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]] * 2);
	g_next_index += 3;
}

static void movr_wm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]] * 2);
	g_next_index += 3;
}

static void movr_dm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]] * 2);
	g_next_index += 3;
}

static void movr_qm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]] * 2);
	g_next_index += 3;
}

static void movr_bm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]] * 4);
	g_next_index += 3;
}

static void movr_wm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]] * 4);
	g_next_index += 3;
}

static void movr_dm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]] * 4);
	g_next_index += 3;
}

static void movr_qm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]] * 4);
	g_next_index += 3;
}

static void movr_bm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]] * 8);
	g_next_index += 3;
}

static void movr_wm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]] * 8);
	g_next_index += 3;
}

static void movr_dm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]] * 8);
	g_next_index += 3;
}

static void movr_qm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]] * 8);
	g_next_index += 3;
}

static void movr_boptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]]  + *(uint16_t *)(data + 2));
	g_next_index += 5;
}

static void movr_woptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + *(uint16_t *)(data + 2));
	g_next_index += 5;
}

static void movr_doptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + *(uint16_t *)(data + 2));
	g_next_index += 5;
}

static void movr_qoptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + *(uint16_t *)(data + 2));
	g_next_index += 5;
}

static void movr_brptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]] + REG[r[2]]);
	g_next_index += 4;
}

static void movr_wrptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + REG[r[2]]);
	g_next_index += 4;
}

static void movr_drptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + REG[r[2]]);
	g_next_index += 4;
}

static void movr_qrptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + REG[r[2]]);
	g_next_index += 4;
}

static void movr_broptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]]  + REG[r[2]] + *(uint16_t *)(data + 3));
	g_next_index += 6;
}

static void movr_wroptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + REG[r[2]] + *(uint16_t *)(data + 3));
	g_next_index += 6;
}

static void movr_droptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + REG[r[2]] + *(uint16_t *)(data + 3));
	g_next_index += 6;
}

static void movr_qroptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + REG[r[2]] + *(uint16_t *)(data + 3));
	g_next_index += 6;
}

static void movr_brm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]]  + REG[r[2]] * 2);
	g_next_index += 4;
}

static void movr_wrm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + REG[r[2]] * 2);
	g_next_index += 4;
}

static void movr_drm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + REG[r[2]] * 2);
	g_next_index += 4;
}

static void movr_qrm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + REG[r[2]] * 2);
	g_next_index += 4;
}

static void movr_brm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]]  + REG[r[2]] * 4);
	g_next_index += 4;
}

static void movr_wrm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + REG[r[2]] * 4);
	g_next_index += 4;
}

static void movr_drm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + REG[r[2]] * 4);
	g_next_index += 4;
}

static void movr_qrm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + REG[r[2]] * 4);
	g_next_index += 4;
}

static void movr_brm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint8_t*)(REG[r[1]]  + REG[r[2]] * 8);
	g_next_index += 4;
}

static void movr_wrm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint16_t*)(REG[r[1]]  + REG[r[2]] * 8);
	g_next_index += 4;
}

static void movr_drm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint32_t*)(REG[r[1]]  + REG[r[2]] * 8);
	g_next_index += 4;
}

static void movr_qrm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = *(uint64_t*)(REG[r[1]]  + REG[r[2]] * 8);
	g_next_index += 4;
}

static void pmovb(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)REG[r[0]] = *(uint8_t*)&r[1];
	g_next_index += 3;
}

static void pmovw(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)REG[r[0]] = *(uint16_t*)&r[1];
	g_next_index += 3;
}

static void pmovd(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)REG[r[0]] = *(uint32_t*)&r[1];
	g_next_index += 3;
}

static void pmovq(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)REG[r[0]] = *(uint64_t*)&r[1];
	g_next_index += 3;
}

static void pmovr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)REG[r[0]] = REG[r[1]];
	g_next_index += 3;
}

static void pmovr_bptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)REG[r[0]] = *(uint8_t*)&REG[r[1]];
	g_next_index += 3;
}

static void pmovr_wptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)REG[r[0]] = *(uint16_t*)&REG[r[1]];
	g_next_index += 3;
}

static void pmovr_dptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)REG[r[0]] = *(uint32_t*)&REG[r[1]];
	g_next_index += 3;
}

static void pmovr_qptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)REG[r[0]] = *(uint64_t*)&REG[r[1]];
	g_next_index += 3;
}

static void pmovr_boptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] + *(uint16_t *)(data + 1)) = *(uint8_t*)&REG[r[3]];
	g_next_index += 5;
}

static void pmovr_woptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] + *(uint16_t *)(data + 1)) = *(uint16_t*)&REG[r[3]];
	g_next_index += 5;
}

static void pmovr_doptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] + *(uint16_t *)(data + 1)) = *(uint32_t*)&REG[r[3]];
	g_next_index += 5;
}

static void pmovr_qoptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] + *(uint16_t *)(data + 1)) = *(uint64_t*)&REG[r[3]];
	g_next_index += 5;
}

static void pmovr_brptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] + REG[r[1]]) = *(uint8_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_wrptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]] + REG[r[1]]) = *(uint16_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_drptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] + REG[r[1]]) = *(uint32_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_qrptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] + REG[r[1]]) = REG[r[2]];
	g_next_index += 4;
}

static void pmovr_bm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] * 2)  = *(uint8_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_wm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]] * 2)  = *(uint16_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_dm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] * 2)  = *(uint32_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_qm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] * 2)  = REG[r[1]];
	g_next_index += 3;
}

static void pmovr_bm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] * 4)  = *(uint8_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_wm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]] * 4)  = *(uint16_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_dm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] * 4)  = *(uint32_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_qm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] * 4)  = (REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_bm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] * 8)  = *(uint8_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_wm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]] * 8)  = *(uint16_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_dm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] * 8)  = *(uint32_t*)(&REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_qm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] * 8)  = (REG[r[1]]);
	g_next_index += 3;
}

static void pmovr_broptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]] + REG[r[1]] + *(uint16_t *)(data + 2)) = *(uint8_t*)&REG[r[4]];
	g_next_index += 6;
}

static void pmovr_wroptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]] + REG[r[1]] + *(uint16_t *)(data + 2)) = *(uint16_t*)&REG[r[4]];
	g_next_index += 6;
}

static void pmovr_droptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]] + REG[r[1]] + *(uint16_t *)(data + 2)) = *(uint32_t*)&REG[r[4]];
	g_next_index += 6;
}

static void pmovr_qroptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]] + REG[r[1]] + *(uint16_t *)(data + 2)) = REG[r[4]];
	g_next_index += 6;
}

static void pmovr_brm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]]  + REG[r[1]] * 2) = *(uint8_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_wrm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]]  + REG[r[1]] * 2) = *(uint16_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_drm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]]  + REG[r[1]] * 2) = *(uint32_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_qrm2ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]]  + REG[r[1]] * 2) = REG[r[2]];
	g_next_index += 4;
}

static void pmovr_brm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]]  + REG[r[1]] * 4) = *(uint8_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_wrm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]]  + REG[r[1]] * 4) = *(uint16_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_drm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]]  + REG[r[1]] * 4) = *(uint32_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_qrm4ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]]  + REG[r[1]] * 4) = REG[r[2]];
	g_next_index += 4;
}

static void pmovr_brm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)(REG[r[0]]  + REG[r[1]] * 8) = *(uint8_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_wrm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint16_t*)(REG[r[0]]  + REG[r[1]] * 8) = *(uint16_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_drm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint32_t*)(REG[r[0]]  + REG[r[1]] * 8) = *(uint32_t*)&REG[r[2]];
	g_next_index += 4;
}

static void pmovr_qrm8ptr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint64_t*)(REG[r[0]]  + REG[r[1]] * 8) = REG[r[2]];
	g_next_index += 4;
}

static void lea(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)&REG[r[1]];
	g_next_index += 3;
}

static void lea_o(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] = (uint64_t)(&REG[r[1]] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void lea_m2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] * 2);
	g_next_index += 3;
}

static void lea_m4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] * 4);
	g_next_index += 3;
}

static void lea_m8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] * 8);
	g_next_index += 3;
}

static void lea_ro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] + REG[r[2]]  + *(uint16_t*)(data + 2));
	g_next_index += 6;
}

static void lea_rm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] + REG[r[2]]  * 2);
	g_next_index += 4;
}

static void lea_rm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] + REG[r[2]]  * 4);
	g_next_index += 4;
}

static void lea_rm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]= (uint64_t)((uint64_t)&REG[r[1]] + REG[r[2]]  * 8);
	g_next_index += 4;
}

static void inc(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]++;
	g_next_index += 2;
}

static void dec(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]]--;
	g_next_index += 2;
}

static void add(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] += REG[r[1]];
	g_next_index += 3;
}

static void add_b(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] += r[1];
	g_next_index += 3;
}

static void badd_b(char *data)
{
	uint8_t *r = (uint8_t *)data;
	*(uint8_t*)REG[r[0]] += r[1];
	g_next_index += 3;
}

static void add_w(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(data + 1);
	g_next_index += 4;
}

static void add_d(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(data + 1);
	g_next_index += 6;
}

static void add_q(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(data + 1);
	g_next_index += 10;
}

static void add_br(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1]);
	g_next_index += 3;
}

static void add_brm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void add_brm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void add_brm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void add_bro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void add_brro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[r[1]] + REG[r[2]] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void add_brrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void add_brrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void add_brrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint8_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void add_wr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1]);
	g_next_index += 3;
}

static void add_wrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void add_wrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void add_wrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void add_wro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void add_wrro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void add_wrrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void add_wrrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void add_wrrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint16_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void add_dr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1]);
	g_next_index += 3;
}

static void add_drm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void add_drm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void add_drm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void add_dro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void add_drro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void add_drrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void add_drrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void add_drrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint32_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void add_qr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1]);
	g_next_index += 3;
}

static void add_qrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void add_qrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void add_qrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void add_qro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void add_qrro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void add_qrrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void add_qrrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void add_qrrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] +=  *(uint64_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void sub(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -= REG[r[1]];
	g_next_index += 3;
}

static void sub_b(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -= r[1];
	g_next_index += 3;
}

static void sub_w(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(data + 1);
	g_next_index += 4;
}

static void sub_d(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(data + 1);
	g_next_index += 6;
}

static void sub_q(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(data + 1);
	g_next_index += 10;
}

static void sub_br(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1]);
	g_next_index += 3;
}

static void sub_brm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void sub_brm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void sub_brm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void sub_bro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void sub_brro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void sub_brrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void sub_brrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void sub_brrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint8_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void sub_wr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1]);
	g_next_index += 3;
}

static void sub_wrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void sub_wrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void sub_wrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void sub_wro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void sub_wrro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void sub_wrrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void sub_wrrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void sub_wrrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint16_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void sub_dr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1]);
	g_next_index += 3;
}

static void sub_drm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void sub_drm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void sub_drm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void sub_dro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void sub_drro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void sub_drrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void sub_drrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void sub_drrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint32_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}

static void sub_qr(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1]);
	g_next_index += 3;
}

static void sub_qrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] * 2);
	g_next_index += 3;
}

static void sub_qrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] * 4);
	g_next_index += 3;
}

static void sub_qrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] * 8);
	g_next_index += 3;
}

static void sub_qro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] + *(uint16_t*)(data + 2));
	g_next_index += 5;
}

static void sub_qrro(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] + REG[2] + *(uint16_t*)(data + 3));
	g_next_index += 6;
}

static void sub_qrrm2(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] + REG[2] * 2);
	g_next_index += 4;
}

static void sub_qrrm4(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] + REG[2] * 4);
	g_next_index += 4;
}

static void sub_qrrm8(char *data)
{
	uint8_t *r = (uint8_t *)data;
	REG[r[0]] -=  *(uint64_t*)(REG[1] + REG[2] * 8);
	g_next_index += 4;
}
static void je(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] == REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jz(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] == 0)
		g_next_index = *(uint16_t *)(data + 1);
	else
		g_next_index += 4;
}

static void jne(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] != REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jnz(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] != 0)
		g_next_index = *(uint16_t *)(data + 1);
	else
		g_next_index += 4;
}

static void jg(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] > REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jge(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] >= REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jnle(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] <= REG[r[1]])
		g_next_index += 5;
	else
		g_next_index = *(uint16_t *)(data + 2);
}

static void jnl(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] < REG[r[1]])
		g_next_index += 5;
	else
		g_next_index = *(uint16_t *)(data + 2);
}

static void jl(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] < REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jnge(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] >= REG[r[1]])
		g_next_index += 5;
	else
		g_next_index = *(uint16_t *)(data + 2);
}

static void jle(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] <= REG[r[1]])
		g_next_index = *(uint16_t *)(data + 2);
	else
		g_next_index += 5;
}

static void jng(char *data)
{
	uint8_t *r = (uint8_t *)data;
	if (REG[r[0]] > REG[r[1]])
		g_next_index += 5;
	else
		g_next_index = *(uint16_t *)(data + 2);
}

uint32_t asm_syscall (
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t,
    uint64_t
    );

#define EXEC_SYSCALL(eax, rcx, rdx, r8, r9, rsp) asm_syscall(rcx, rdx, r8, r9, eax, 0, \
	*(uint64_t*)(rsp), *(uint64_t*)(rsp + 8), *(uint64_t*)(rsp + 16), *(uint64_t*)(rsp + 24), *(uint64_t*)(rsp + 32), *(uint64_t*)(rsp + 40), *(uint64_t*)(rsp +48))

static void syscall(char *data)
{
	REG[RAX] = EXEC_SYSCALL((uint32_t)REG[EAX], REG[RCX], REG[RDX], REG[R8Q], REG[R9Q], REG[RSP]);
	g_next_index++;
}

static void call_native(char *data)
{
	REG[RAX] = ((uint64_t (*)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t))
		REG[RAX])(REG[RCX], REG[RDX], REG[R8Q], REG[R9Q], REG[RSP]);
	g_next_index++;
}

static void call(char *data)
{
	asmjit((unsigned char *)REG[RAX]);
	g_next_index++;
}

static void clr_rsp(char *data)
{
	uint8_t *r = (uint8_t *)data;
	for (uint32_t i = 0; i < sizeof(stack_rsp); i++)
		stack_rsp[i] = 0;
	g_next_index += 1;
}

static void clr_rspo(char *data)
{
	uint8_t *r = (uint8_t *)data;
	for (uint32_t i = 0; i < REG[r[0]]; i++)
		stack_rsp[i] = 0;
	g_next_index += 2;
}

static void cpy_rsp(char *data)
{
	uint8_t *r = (uint8_t *)data;

	uint64_t address_0 = REG[r[0]];
	uint64_t address_1 = REG[r[1]];
	for (int i = 0; i < *(uint16_t*)(data + 2); i++)
		*(uint8_t*)(address_0 + i) = *(uint8_t*)(address_1 + i);

	g_next_index += 5;
}

static void *instruction_list[] = {
movb,
movw,
movd,
movq,
movr,
movr_bptr,
movr_wptr,
movr_dptr,
movr_qptr,
movr_boptr,
movr_woptr,
movr_doptr,
movr_qoptr,
movr_brptr,
movr_wrptr,
movr_drptr,
movr_qrptr,
movr_bm2ptr,
movr_wm2ptr,
movr_dm2ptr,
movr_qm2ptr,
movr_bm4ptr,
movr_wm4ptr,
movr_dm4ptr,
movr_qm4ptr,
movr_bm8ptr,
movr_wm8ptr,
movr_dm8ptr,
movr_qm8ptr,
movr_broptr,
movr_wroptr,
movr_droptr,
movr_qroptr,
movr_brm2ptr,
movr_wrm2ptr,
movr_drm2ptr,
movr_qrm2ptr,
movr_brm4ptr,
movr_wrm4ptr,
movr_drm4ptr,
movr_qrm4ptr,
movr_brm8ptr,
movr_wrm8ptr,
movr_drm8ptr,
movr_qrm8ptr,
pmovb,
pmovw,
pmovd,
pmovq,
pmovr,
pmovr_bptr,
pmovr_wptr,
pmovr_dptr,
pmovr_qptr,
pmovr_boptr,
pmovr_woptr,
pmovr_doptr,
pmovr_qoptr,
pmovr_brptr,
pmovr_wrptr,
pmovr_drptr,
pmovr_qrptr,
pmovr_bm2ptr,
pmovr_wm2ptr,
pmovr_dm2ptr,
pmovr_qm2ptr,
pmovr_bm4ptr,
pmovr_wm4ptr,
pmovr_dm4ptr,
pmovr_qm4ptr,
pmovr_bm8ptr,
pmovr_wm8ptr,
pmovr_dm8ptr,
pmovr_qm8ptr,
pmovr_broptr,
pmovr_wroptr,
pmovr_droptr,
pmovr_qroptr,
pmovr_brm2ptr,
pmovr_wrm2ptr,
pmovr_drm2ptr,
pmovr_qrm2ptr,
pmovr_brm4ptr,
pmovr_wrm4ptr,
pmovr_drm4ptr,
pmovr_qrm4ptr,
pmovr_brm8ptr,
pmovr_wrm8ptr,
pmovr_drm8ptr,
pmovr_qrm8ptr,
mov_gs,
lea,
lea_o,
lea_m2,
lea_m4,
lea_m8,
lea_ro,
lea_rm2,
lea_rm4,
lea_rm8,
inc,
dec,
add,
add_b,
add_w,
add_d,
add_q,
add_br,
add_brm2,
add_brm4,
add_brm8,
add_bro,
add_brro, 
add_brrm2,
add_brrm4,
add_brrm8,		
add_wr,
add_wrm2,
add_wrm4,
add_wrm8,
add_wro,
add_wrro, 
add_wrrm2,
add_wrrm4,
add_wrrm8,
add_dr,
add_drm2,
add_drm4,
add_drm8,
add_dro,
add_drro, 
add_drrm2,
add_drrm4,
add_drrm8,
add_qr,
add_qrm2,
add_qrm4,
add_qrm8,
add_qro,
add_qrro, 
add_qrrm2,
add_qrrm4,
add_qrrm8,
sub,
sub_b,
sub_w,
sub_d,
sub_q,
sub_br,
sub_brm2,
sub_brm4,
sub_brm8,
sub_bro,
sub_brro, 
sub_brrm2,
sub_brrm4,
sub_brrm8,		
sub_wr,
sub_wrm2,
sub_wrm4,
sub_wrm8,
sub_wro,
sub_wrro, 
sub_wrrm2,
sub_wrrm4,
sub_wrrm8,
sub_dr,
sub_drm2,
sub_drm4,
sub_drm8,
sub_dro,
sub_drro, 
sub_drrm2,
sub_drrm4,
sub_drrm8,
sub_qr,
sub_qrm2,
sub_qrm4,
sub_qrm8,
sub_qro,
sub_qrro, 
sub_qrrm2,
sub_qrrm4,
sub_qrrm8,
je,
jz,
jne,
jnz,
jg,
jnle,
jge,
jnl,
jl,
jnge,
jle,
jng,
syscall,
call_native,
call,
clr_rsp,
cpy_rsp,
badd_b,
clr_rspo
} ;

void asmjit(unsigned char *payload)
{
	uint16_t size = *(uint16_t*)payload;
	payload += 2;

	g_next_index = 0;
	while (g_next_index < size) {
		((void (*)(void *data))
		instruction_list[payload[g_next_index]])(payload + g_next_index + 1);	
	}
}
