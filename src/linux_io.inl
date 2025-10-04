//
// I/O wrapper functions for Linux
//
static void delay(uint32_t u32)
{
        usleep(u32*1000);
} /* delay() */

// Initialize the I2C bus on Linux
static void I2CInit(BBI2C *pI2C, uint32_t iClock)
{
char filename[32];
int iChannel = pI2C->iSDA;

// Only try to initialize the handle if it hasn't already been done
    if (pI2C->file_i2c == -1) {
        sprintf(filename, "/dev/i2c-%d", iChannel);
        if ((pI2C->file_i2c = open(filename, O_RDWR)) < 0)
        {
                fprintf(stderr, "Failed to open the i2c bus\n");
        }
    }
} /* I2CInit() */
//
// Test if an I2C address responds
// returns 1 for success, 0 for failure
//
static uint8_t I2CTest(BBI2C *pI2C, uint8_t addr)
{
uint8_t response = 0;
    if (ioctl(pI2C->file_i2c, I2C_SLAVE, addr) >= 0) {
            // probe this address
        uint8_t ucTemp;
        if (read(pI2C->file_i2c, &ucTemp, 1) >= 0)
            response = 1;
    }
    return response;
} /* I2CTest() */
//
// Read n bytes from the given I2C address
//
static int I2CRead(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = read(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CRead() */
#ifdef FUTURE
//
// Read n bytes from the given address, after setting the register number
//
static int I2CReadRegister(BBI2C *pI2C, uint8_t iAddr, uint8_t u8Register, uint8_t *pData, int iLen)
{
int rc;
        // Reading from an I2C device involves first writing the 8-bit register
        // followed by reading the data
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = write(pI2C->file_i2c, &u8Register, 1); // write the register value
        if (rc == 1)
        {
                rc = read(pI2C->file_i2c, pData, iLen);
        }
        return rc;

} /* I2CReadRegister() */
#endif // FUTURE
//
// Write n bytes to the given address
//
static int I2CWrite(BBI2C *pI2C, uint8_t iAddr, uint8_t *pData, int iLen)
{
int rc;
        ioctl(pI2C->file_i2c, I2C_SLAVE, iAddr);
        rc = write(pI2C->file_i2c, pData, iLen);
        return rc;
} /* I2CWrite() */

