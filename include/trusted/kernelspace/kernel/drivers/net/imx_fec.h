/**
 * Copyright (c) Andreas Fitzek <andreas[dot]fitzek[at]gmail[dot]com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, 
 * with or without modification, are permitted provided 
 * that the following conditions are met:
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and the following 
 * disclaimer. Redistributions in binary form must reproduce 
 * the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or 
 * other materials provided with the distribution. 
 * Neither the name of the nor the names of its contributors
 * may be used to endorse or promote products derived from 
 * this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/
/*
 * imx_fec.h
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#ifndef IMX_FEC_H_
#define IMX_FEC_H_

#include <common/typedefs.h>
#include <hal.h>

/**
 * IMX Fast Ethernet Controller
 */
struct fec_registers {
	uint32_t res0[1]; 	// +0x000
	uint32_t ievent;  	// +0x004
	uint32_t imask;	 	// +0x008
	uint32_t res1[1]; 	// +0x00C
	uint32_t r_desc_act;	// +0x010
	uint32_t t_desc_act;	// +0x014
	uint32_t res2[3];	// 0x018 - 0x020
	uint32_t ecntrl;		// 0x024
	uint32_t res3[6];	// 0x028 - 0x03c
	uint32_t mii_data;	// 0x040
	uint32_t mii_speed;	// 0x044
	uint32_t res4[7];	// 0x048 - 0x060
	uint32_t mib_control;	// 0x064
	uint32_t res5[7];	// 0x068 - 0x080
	uint32_t r_cntrl;	// 0x084
	uint32_t res6[15];	// 0x088 - 0x0C0
	uint32_t t_cntrl;	// 0x0C4
	uint32_t res7[7];	// 0x0C8 - 0x0E0
	uint32_t paddr1;		// 0x0E4
	uint32_t paddr2;		// 0x0E8
	uint32_t op_pause;	// 0x0EC
	uint32_t res8[10];	// 0x0F0 - 0x114
	uint32_t iaddr1;		// 0x118
	uint32_t iaddr2;		// 0x11C
	uint32_t gaddr1;		// 0x120
	uint32_t gaddr2;		// 0x124
	uint32_t res9[7];	// 0x128 - 0x140
	uint32_t x_wmrk;		// 0x144
	uint32_t res10[1];	// 0x148
	uint32_t r_bound;	// 0x14C
	uint32_t r_fstart;	// 0x150
	uint32_t res11[11];	// 0x154 - 0x17C
	uint32_t erdsr;		// 0x180
	uint32_t etdsr;		// 0x184
	uint32_t emrbr;		// 0x188
	uint32_t res12[29];	// 0x18C - 0x1FC

// MIB Counters

	uint32_t rmon_t_drop; /* MBAR_ETH + 0x200 */
	uint32_t rmon_t_packets; /* MBAR_ETH + 0x204 */
	uint32_t rmon_t_bc_pkt; /* MBAR_ETH + 0x208 */
	uint32_t rmon_t_mc_pkt; /* MBAR_ETH + 0x20C */
	uint32_t rmon_t_crc_align; /* MBAR_ETH + 0x210 */
	uint32_t rmon_t_undersize; /* MBAR_ETH + 0x214 */
	uint32_t rmon_t_oversize; /* MBAR_ETH + 0x218 */
	uint32_t rmon_t_frag; /* MBAR_ETH + 0x21C */
	uint32_t rmon_t_jab; /* MBAR_ETH + 0x220 */
	uint32_t rmon_t_col; /* MBAR_ETH + 0x224 */
	uint32_t rmon_t_p64; /* MBAR_ETH + 0x228 */
	uint32_t rmon_t_p65to127; /* MBAR_ETH + 0x22C */
	uint32_t rmon_t_p128to255; /* MBAR_ETH + 0x230 */
	uint32_t rmon_t_p256to511; /* MBAR_ETH + 0x234 */
	uint32_t rmon_t_p512to1023; /* MBAR_ETH + 0x238 */
	uint32_t rmon_t_p1024to2047; /* MBAR_ETH + 0x23C */
	uint32_t rmon_t_p_gte2048; /* MBAR_ETH + 0x240 */
	uint32_t rmon_t_octets; /* MBAR_ETH + 0x244 */
	uint32_t ieee_t_drop; /* MBAR_ETH + 0x248 */
	uint32_t ieee_t_frame_ok; /* MBAR_ETH + 0x24C */
	uint32_t ieee_t_1col; /* MBAR_ETH + 0x250 */
	uint32_t ieee_t_mcol; /* MBAR_ETH + 0x254 */
	uint32_t ieee_t_def; /* MBAR_ETH + 0x258 */
	uint32_t ieee_t_lcol; /* MBAR_ETH + 0x25C */
	uint32_t ieee_t_excol; /* MBAR_ETH + 0x260 */
	uint32_t ieee_t_macerr; /* MBAR_ETH + 0x264 */
	uint32_t ieee_t_cserr; /* MBAR_ETH + 0x268 */
	uint32_t ieee_t_sqe; /* MBAR_ETH + 0x26C */
	uint32_t t_fdxfc; /* MBAR_ETH + 0x270 */
	uint32_t ieee_t_octets_ok; /* MBAR_ETH + 0x274 */

	uint32_t res13[2]; /* MBAR_ETH + 0x278-27C */
	uint32_t rmon_r_drop; /* MBAR_ETH + 0x280 */
	uint32_t rmon_r_packets; /* MBAR_ETH + 0x284 */
	uint32_t rmon_r_bc_pkt; /* MBAR_ETH + 0x288 */
	uint32_t rmon_r_mc_pkt; /* MBAR_ETH + 0x28C */
	uint32_t rmon_r_crc_align; /* MBAR_ETH + 0x290 */
	uint32_t rmon_r_undersize; /* MBAR_ETH + 0x294 */
	uint32_t rmon_r_oversize; /* MBAR_ETH + 0x298 */
	uint32_t rmon_r_frag; /* MBAR_ETH + 0x29C */
	uint32_t rmon_r_jab; /* MBAR_ETH + 0x2A0 */

	uint32_t rmon_r_resvd_0; /* MBAR_ETH + 0x2A4 */

	uint32_t rmon_r_p64; /* MBAR_ETH + 0x2A8 */
	uint32_t rmon_r_p65to127; /* MBAR_ETH + 0x2AC */
	uint32_t rmon_r_p128to255; /* MBAR_ETH + 0x2B0 */
	uint32_t rmon_r_p256to511; /* MBAR_ETH + 0x2B4 */
	uint32_t rmon_r_p512to1023; /* MBAR_ETH + 0x2B8 */
	uint32_t rmon_r_p1024to2047; /* MBAR_ETH + 0x2BC */
	uint32_t rmon_r_p_gte2048; /* MBAR_ETH + 0x2C0 */
	uint32_t rmon_r_octets; /* MBAR_ETH + 0x2C4 */
	uint32_t ieee_r_drop; /* MBAR_ETH + 0x2C8 */
	uint32_t ieee_r_frame_ok; /* MBAR_ETH + 0x2CC */
	uint32_t ieee_r_crc; /* MBAR_ETH + 0x2D0 */
	uint32_t ieee_r_align; /* MBAR_ETH + 0x2D4 */
	uint32_t r_macerr; /* MBAR_ETH + 0x2D8 */
	uint32_t r_fdxfc; /* MBAR_ETH + 0x2DC */
	uint32_t ieee_r_octets_ok; /* MBAR_ETH + 0x2E0 */

	uint32_t res14[7]; /* MBAR_ETH + 0x2E4-2FC */

	uint16_t miigsk_cfgr; /* MBAR_ETH + 0x300 */
	uint16_t res15[3]; /* MBAR_ETH + 0x302-306 */
	uint16_t miigsk_enr; /* MBAR_ETH + 0x308 */
	uint16_t res16[3]; /* MBAR_ETH + 0x30a-30e */
	uint32_t res17[60]; /* MBAR_ETH + 0x300-3FF */
};

typedef struct fec_registers fec_regs_t;

struct fec_bd {
	uint16_t data_length; /* payload's length in bytes */
	uint16_t status; /* BD's staus (see datasheet) */
	uint32_t data_pointer; /* payload's buffer address */
};

typedef struct fec_bd fec_bd_t;

struct fec_priv {
	fec_regs_t* dev;
	fec_bd_t* rx_bd;
	int32_t rx_index;
	fec_bd_t* tx_bd;
	int32_t tx_index;
	uint8_t phy_id;
};

typedef struct fec_priv fec_priv_t;

#define FEC_IEVENT_HBERR		0x80000000
#define FEC_IEVENT_BABR			0x40000000
#define FEC_IEVENT_BABT			0x20000000
#define FEC_IEVENT_GRA			0x10000000
#define FEC_IEVENT_TXF			0x08000000
#define FEC_IEVENT_TXB			0x04000000
#define FEC_IEVENT_RXF			0x02000000
#define FEC_IEVENT_RXB			0x01000000
#define FEC_IEVENT_MII			0x00800000
#define FEC_IEVENT_EBERR		0x00400000
#define FEC_IEVENT_LC			0x00200000
#define FEC_IEVENT_RL			0x00100000
#define FEC_IEVENT_UN			0x00080000

#define FEC_IMASK_HBERR			0x80000000
#define FEC_IMASK_BABR			0x40000000
#define FEC_IMASKT_BABT			0x20000000
#define FEC_IMASK_GRA			0x10000000
#define FEC_IMASKT_TXF			0x08000000
#define FEC_IMASK_TXB			0x04000000
#define FEC_IMASKT_RXF			0x02000000
#define FEC_IMASK_RXB			0x01000000
#define FEC_IMASK_MII			0x00800000
#define FEC_IMASK_EBERR			0x00400000
#define FEC_IMASK_LC			0x00200000
#define FEC_IMASKT_RL			0x00100000
#define FEC_IMASK_UN			0x00080000

#define FEC_RCNTRL_MAX_FL_SHIFT		16
#define FEC_RCNTRL_LOOP			0x00000001
#define FEC_RCNTRL_DRT			0x00000002
#define FEC_RCNTRL_MII_MODE		0x00000004
#define FEC_RCNTRL_PROM			0x00000008
#define FEC_RCNTRL_BC_REJ		0x00000010
#define FEC_RCNTRL_FCE			0x00000020
#define FEC_RCNTRL_RGMII		0x00000040
#define FEC_RCNTRL_RMII			0x00000100
#define FEC_RCNTRL_RMII_10T		0x00000200

#define FEC_TCNTRL_GTS			0x00000001
#define FEC_TCNTRL_HBC			0x00000002
#define FEC_TCNTRL_FDEN			0x00000004
#define FEC_TCNTRL_TFC_PAUSE		0x00000008
#define FEC_TCNTRL_RFC_PAUSE		0x00000010

#define FEC_ECNTRL_RESET		0x00000001	/* reset the FEC */
#define FEC_ECNTRL_ETHER_EN		0x00000002	/* enable the FEC */
#define FEC_ECNTRL_SPEED		0x00000020
#define FEC_ECNTRL_DBSWAP		0x00000100

#define FEC_X_WMRK_STRFWD		0x00000100

#define FEC_MAX_FRAME_LEN		1518

#define FEC_MII_DATA_RA_SHIFT	18
#define FEC_MII_DATA_PA_SHIFT	23

#define FEC_MII_DATA_ST			0x40000000
#define FEC_MII_DATA_OP_RD		0x20000000
#define FEC_MII_DATA_OP_WR		0x10000000
#define FEC_MII_DATA_TA			0x00020000

#define MII_BMCR				0x00
#define BMCR_RESET				0x8000
#define MII_ADVERTISE			0x04
#define LPA_100FULL				0x0100
#define LPA_100HALF				0x0080
#define LPA_10FULL				0x0040
#define LPA_10HALF				0x0020
#define PHY_ANLPAR_PSB_802_3	0x0001

#define BMCR_ANENABLE			0x1000
#define BMCR_ANRESTART			0x0200

/* defines for MIIGSK */
/* RMII frequency control: 0=50MHz, 1=5MHz */
#define MIIGSK_CFGR_FRCONT		(1 << 6)
/* loopback mode */
#define MIIGSK_CFGR_LBMODE		(1 << 4)
/* echo mode */
#define MIIGSK_CFGR_EMODE		(1 << 3)
/* MII gasket mode field */
#define MIIGSK_CFGR_IF_MODE_MASK	(3 << 0)
/* MMI/7-Wire mode */
#define MIIGSK_CFGR_IF_MODE_MII		(0 << 0)
/* RMII mode */
#define MIIGSK_CFGR_IF_MODE_RMII	(1 << 0)
/* reflects MIIGSK Enable bit (RO) */
#define MIIGSK_ENR_READY		(1 << 2)
/* enable MIGSK (set by default) */
#define MIIGSK_ENR_EN			(1 << 1)

#define IMX53_FEC_DRIVER_NAME	"IMX53_FEC"

int __imx_fec_init(fec_priv_t* fec);
int __fec_mdio_write(fec_regs_t *eth, uint8_t phyAddr, uint8_t regAddr,
		uint16_t data);
int __fec_mdio_read(fec_regs_t *eth, uint8_t phyAddr, uint8_t regAddr);
int imx_fec_probe(platform_device_t *dev);
void imx_fec_release(platform_device_t *dev);
uint32_t imx_fec_write(platform_device_t *dev, void* data, uint32_t size);
uint32_t imx_fec_read(platform_device_t *dev, void* data, uint32_t nsize);

uint32_t imx_fec_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize);

#endif /* IMX_FEC_H_ */
