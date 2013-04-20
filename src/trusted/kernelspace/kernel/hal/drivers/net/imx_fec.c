/*
 * imx_fec.c
 *
 *  Created on: Nov 12, 2012
 *      Author: Andreas Fitzek 
 *       Email: andreas.fitzek@gmail.com
 */

#include <drivers/net/imx_fec.h>
#include <mm/mm.h>

int imx_fec_probe(platform_device_t *dev) {
	hal_map_io_mem(dev);

	fec_priv_t* fec = (fec_priv_t*)kmalloc(sizeof(fec_priv_t));
	fec->dev = (fec_regs_t*) dev->vbase;
	dev->device_data = (uintptr_t) fec;
	return (__imx_fec_init(fec));
}

void imx_fec_release(platform_device_t *dev) {

}

uint32_t imx_fec_write(platform_device_t *dev, void* data, uint32_t size) {

	//fec_priv_t* fec = (fec_priv_t*) dev->device_data;

	if (size > 1500) {
		hal_debug("invalid payload");
		return HAL_E_INVALID_DEV;
	}

	return HAL_SUCCESS;
}

uint32_t imx_fec_read(platform_device_t *dev, void* data, uint32_t nsize) {
	//uint32_t ievent;
	uint32_t addr;//, size;
	if (dev == NULL) {
		return HAL_E_INVALID_DEV;
	}

	fec_priv_t* fec = (fec_priv_t*) dev->device_data;
	fec_bd_t* rbd = &fec->rx_bd[fec->rx_index];

	if (fec == NULL) {
		return HAL_E_INVALID_DEV;
	}

	/*
	 * TODO: Check if any critical events have happened
	 */

	addr = (uint32_t) rbd;
	addr &= ~(64 - 1);
	//size = roundup(sizeof(fec_bd_t), 64);

	return HAL_SUCCESS;
}

uint32_t imx_fec_ioctl(platform_device_t *dev, uint32_t request,
		uintptr_t param, uint32_t psize) {
	return (HAL_E_IOCTL_REQ_NOT_AVAIL);
}

static const platform_driver_t imx_fec_driver = { .probe = imx_fec_probe,
		.release = imx_fec_release, .write = imx_fec_write,
		.read = imx_fec_read, .ioctl = imx_fec_ioctl, .driver = { .name =
				IMX53_FEC_DRIVER_NAME, .driver_data = NULL } };

HAL_DRIVER_REGISTER(imx_fec_driver)

int __imx_fec_init(fec_priv_t* fec) {
	int i = 0;
	uint32_t mib_ptr = (uint32_t) &fec->dev->rmon_t_drop;
	uint32_t r_ctrl = (FEC_MAX_FRAME_LEN << FEC_RCNTRL_MAX_FL_SHIFT) | 0x12;
	uint32_t size = 0;
	uint8_t mac[] = { 0x01, 0x02, 0x06, 0x08, 0xAA, 0xCD };
	fec->phy_id = 0x1f;
	// set mac address
	__raw_writel(0, &fec->dev->iaddr1);
	__raw_writel(0, &fec->dev->iaddr2);
	__raw_writel(0, &fec->dev->gaddr1);
	__raw_writel(0, &fec->dev->gaddr2);

	/*
	 * Set physical address
	 */
	__raw_writel((mac[0] << 24) + (mac[1] << 16) + (mac[2] << 8) + mac[3],
			&fec->dev->paddr1);
	__raw_writel((mac[4] << 24) + (mac[5] << 16) + 0x8808, &fec->dev->paddr2);

	// add transmit descriptor
	size = roundup(2*sizeof(fec_bd_t), 64);

	fec->tx_bd = (fec_bd_t*)kmalloc_align(size, 64);
	memset(fec->tx_bd, 0, size);
	__raw_writew(0x0000, &fec->tx_bd[0].status);
	__raw_writew(0x2000, &fec->tx_bd[1].status);
	fec->tx_index = 0;

	// add receive descriptor
	size = roundup(64*sizeof(fec_bd_t), 64);
	fec->rx_bd = (fec_bd_t*) kmalloc_align(size, 64);
	memset(fec->rx_bd, 0, size);
	size = roundup(FEC_MAX_FRAME_LEN, 64);
	for (i = 0; i < 64; i++) {
		uint32_t data_ptr = __raw_readl(&fec->rx_bd[i].data_pointer);
		if (data_ptr == 0) {
			uint8_t *data = (uint8_t *)kmalloc_align(size, 64);
			__raw_writel((uint32_t) data, &fec->rx_bd[i].data_pointer);
		} /* needs allocation */
		__raw_writew(0x8000, &fec->rx_bd[i].status);
		__raw_writew(0, &fec->rx_bd[i].data_length);
	}

	/* Mark the last RBD to close the ring. */
	__raw_writew(0x2000 | 0x8000, &fec->rx_bd[i - 1].status);
	fec->rx_index = 0;

	__raw_writel(r_ctrl, &fec->dev->r_cntrl);

	// TODO: set mii speed (calculate speed value ...)
	__raw_writel(0x00000016, &fec->dev->mii_speed);

	/*
	 * Set Opcode/Pause Duration Register
	 */
	__raw_writel(0x00010020, &fec->dev->op_pause); /* FIXME 0xffff0020; */
	__raw_writel(0x2, &fec->dev->x_wmrk);
	/*
	 * Set multicast address filter
	 */
	__raw_writel(0x00000000, &fec->dev->gaddr1);
	__raw_writel(0x00000000, &fec->dev->gaddr2);

	/* clear MIB RAM */
	for (i = mib_ptr; i <= mib_ptr + 0xfc; i += 4)
		__raw_writel(0, (void*)i);

	// reset phy
	__fec_mdio_write(fec->dev, fec->phy_id, MII_BMCR, BMCR_RESET);

	__fec_mdio_write(fec->dev, fec->phy_id, MII_ADVERTISE,
			LPA_100FULL | LPA_100HALF | LPA_10FULL | LPA_10HALF
					| PHY_ANLPAR_PSB_802_3);
	__fec_mdio_write(fec->dev, fec->phy_id, MII_BMCR,
			BMCR_ANENABLE | BMCR_ANRESTART);

	// TODO: implement fec_open code ...
	__raw_writel(1 << 2, &fec->dev->t_cntrl);

	/*
	 * Enable FEC-Lite controller
	 */
	__raw_writel(__raw_readl(&fec->dev->ecntrl) | FEC_ECNTRL_ETHER_EN,
			&fec->dev->ecntrl);

	__raw_writel(1 << 24, &fec->dev->r_desc_act);

	return HAL_SUCCESS;
}

int __fec_mdio_write(fec_regs_t *eth, uint8_t phyAddr, uint8_t regAddr,
		uint16_t data) {
	uint32_t reg; /* convenient holder for the PHY register */
	uint32_t phy; /* convenient holder for the PHY */
	//uint32_t start;

	reg = regAddr << FEC_MII_DATA_RA_SHIFT;
	phy = phyAddr << FEC_MII_DATA_PA_SHIFT;

	__raw_writel(
			FEC_MII_DATA_ST | FEC_MII_DATA_OP_WR | FEC_MII_DATA_TA | phy | reg
					| data, &eth->mii_data);

	/*
	 * wait for the MII interrupt
	 */
	while (!(__raw_readl(&eth->ievent) & FEC_IEVENT_MII)) {
		NOP;
	}

	/*
	 * clear MII interrupt bit
	 */
	__raw_writel(FEC_IEVENT_MII, &eth->ievent);
	hal_debug(
			"%s: phy: 0x%x reg: 0x%x val: 0x%x", __func__, phyAddr, regAddr, data);

	return (0);
}

int __fec_mdio_read(fec_regs_t *eth, uint8_t phyAddr, uint8_t regAddr) {
	uint32_t reg; /* convenient holder for the PHY register */
	uint32_t phy; /* convenient holder for the PHY */
	//uint32_t start;
	int val;

	/*
	 * reading from any PHY's register is done by properly
	 * programming the FEC's MII data register.
	 */
	__raw_writel(FEC_IEVENT_MII, &eth->ievent);
	reg = regAddr << FEC_MII_DATA_RA_SHIFT;
	phy = phyAddr << FEC_MII_DATA_PA_SHIFT;

	__raw_writel(
			FEC_MII_DATA_ST | FEC_MII_DATA_OP_RD | FEC_MII_DATA_TA | phy | reg,
			&eth->mii_data);

	/*
	 * wait for the related interrupt
	 */
	while (!(__raw_readl(&eth->ievent) & FEC_IEVENT_MII)) {
	}

	/*
	 * clear mii interrupt bit
	 */
	__raw_writel(FEC_IEVENT_MII, &eth->ievent);

	/*
	 * it's now safe to read the PHY's register
	 */
	val = (unsigned short) __raw_readl(&eth->mii_data);
	hal_debug(
			"%s: phy: 0x%x reg: 0x%x val: 0x%x", __func__, phyAddr, regAddr, val);
	return (val);
}
