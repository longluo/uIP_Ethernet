/*       ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
         ?   TITLE:  RTL8019AS ethernet routines for 8051 and      ?
         ?           Keil C51 v7.00 port of Adam Dunkels' uIP v0.9 ?
         ?FILENAME:  etherdev.h                                    ?
         ?REVISION:  VER 0.0                                       ?
         ?REV.DATE:  21-01-05                                      ?
         ? ARCHIVE:                                                ?
         ?  AUTHOR:  Copyright (c) 2005, Murray R. Van Luyn.       ?
         ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ        */

/*  ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ?
    ? THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS  ?
    ? OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED  ?
    ? WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ?
    ? ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY    ?
    ? DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL ?
    ? DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  ?
    ? GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS      ?
    ? INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,       ?
    ? WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING          ?
    ? NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS ?
    ? SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.       ?
    ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤ? */


#ifndef ETHERDEV_H
#define ETHERDEV_H

#include "REG52.h"
#include "uip.h"


/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                                                                    ?
    ?                            Public defines.                         ?
    ?                                                                    ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */
// Change ETH_CPU_XTAL to match hardware

//#define ETH_CPU_XTAL     11059200             // 8051 crystal freq in Hz
#define ETH_CPU_XTAL       22118400

/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                                                                    ?
    ?           ISA Expansion slot signal to 8051 port mapping.          ?
    ?                                                                    ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */
#define ETH_DATA_PORT P0     // Adjust this to suit hardware.
#define ETH_ADDR_PORT P2     // Adjust this to suit hardware.
#define ETH_CTRL_PORT P3     // Adjust this to suit hardware.
sbit ETH_RST_PIN = ETH_CTRL_PORT^5;	//rst pin 	
sbit IOW = ETH_CTRL_PORT^6;  // ISA slot pin B13, RTL8019AS pin 30, active low
sbit IOR = ETH_CTRL_PORT^7;  // ISA slot pin B14, RTL8019AS pin 29, active low


/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                                                                    ?
    ?                  Required additional ISA slot wiring.              ?
    ?                                                                    ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */
//      SD0   ETH_DATA_PORT^0  // ISA slot pin A9,  RTL8019AS pin 36
//      SD1   ETH_DATA_PORT^1  // ISA slot pin A8,  RTL8019AS pin 37
//      SD2   ETH_DATA_PORT^2  // ISA slot pin A7,  RTL8019AS pin 38
//      SD3   ETH_DATA_PORT^3  // ISA slot pin A6,  RTL8019AS pin 39
//      SD4   ETH_DATA_PORT^4  // ISA slot pin A5,  RTL8019AS pin 40
//      SD5   ETH_DATA_PORT^5  // ISA slot pin A4,  RTL8019AS pin 41
//      SD6   ETH_DATA_PORT^6  // ISA slot pin A3,  RTL8019AS pin 42
//      SD7   ETH_DATA_PORT^7  // ISA slot pin A2,  RTL8019AS pin 43
//      SA0   ETH_ADDR_PORT^0  // ISA slot pin A31, RTL8019AS pin 5
//      SA1   ETH_ADDR_PORT^1  // ISA slot pin A30, RTL8019AS pin 7
//      SA2   ETH_ADDR_PORT^2  // ISA slot pin A29, RTL8019AS pin 8
//      SA3   ETH_ADDR_PORT^3  // ISA slot pin A28, RTL8019AS pin 9
//      SA4   ETH_ADDR_PORT^4  // ISA slot pin A27, RTL8019AS pin 10
//      SA5   GND              // ISA slot pin A26, RTL8019AS pin 11
//      SA6   GND              // ISA slot pin A25, RTL8019AS pin 12
//      SA7   GND              // ISA slot pin A24, RTL8019AS pin 13
//      SA8   +5V              // ISA slot pin A23, RTL8019AS pin 15
//      SA9   +5V              // ISA slot pin A22, RTL8019AS pin 16
//      SA10  GND              // ISA slot pin A21, RTL8019AS pin 18
//      SA11  GND              // ISA slot pin A20, RTL8019AS pin 19
//      SA12  GND              // ISA slot pin A19, RTL8019AS pin 20
//      SA13  GND              // ISA slot pin A18, RTL8019AS pin 21
//      SA14  GND              // ISA slot pin A17, RTL8019AS pin 22
//      SA15  GND              // ISA slot pin A16, RTL8019AS pin 23
//      SA16  GND              // ISA slot pin A15, RTL8019AS pin 24
//      SA17  GND              // ISA slot pin A14, RTL8019AS pin 25
//      SA18  GND              // ISA slot pin A13, RTL8019AS pin 26
//      SA19  GND              // ISA slot pin A12, RTL8019AS pin 27
//      AEN   GND              // ISA slot pin A11, RTL8019AS pin 34
//      SMEMW +5V              // ISA slot pin B11, RTL8019AS pin 32
//      SMEMR +5V              // ISA slot pin B12, RTL8019AS pin 31
//      GND   GND              // ISA slot pin B1
//     +5VDC  +5V              // ISA slot pin B3
//      GND   GND              // ISA slot pin B31
//     +5VDC  +5V              // ISA slot pin B29
//     RESET   ?               // ISA slot pin B2, active high reset input


/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                                                                    ?
    ?                ISA 16 bit expansion slot edge connector.           ?
    ?                                                                    ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */

//       ?               ฺฤฤฤฤฤ?                                    ?
//       ณC18           C1?    ณA31         Component side         A1?
//       ิออออออออออออออออพ     ิอออออออออออออออออออออออออออออออออออออ?
//        D18           D1       B31          Solder side           B1



/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                                                                    ?
    ?                       RTL8019AS Register defines.                  ?
    ?                                                                    ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */

// Register base address
#define ETH_REG_BASE 0x0000                // Hardwired to 0x0300

// Registers common to all pages.
#define CR           ETH_REG_BASE + 0x00   // Control register
    // Control register bits
    #define PS1      0x80                  // Page select bit 1
    #define PS0      0x40                  // Page select bit 0
    #define RD2      0x20                  // Remote DMA control bit 2
    #define RD1      0x10                  // Remote DMA control bit 1
    #define RD0      0x08                  // Remote DMA control bit 0
    #define TXP      0x04                  // Transmit packet bit
    #define STA      0x02                  // Start bit (a flag only)
    #define STP      0x01                  // Stop bit transceiver ctrl
#define RDMA         0x10                  // Remote DMA port
#define RESET        0x18                  // Reset port

// Page 0 read/write registers.
#define BNRY         ETH_REG_BASE + 0x03   // Boundary register
#define ISR          ETH_REG_BASE + 0x07   // Interrupt status register
    // Interrupt status register bits
    #define RST      0x80                  // Reset state indicator bit
    #define RDC      0x40                  // Remote DMA complete bit
    #define CNT      0x20                  // Network tally counter MSB set
    #define OVW      0x10                  // Receive buffer exhausted
    #define TXE      0x08                  // Transmit abort error bit
    #define RXE      0x04                  // Receive error report bit
    #define PTX      0x02                  // Successful packet transmit
    #define PRX      0x01                  // Successful packet receive

// Page 0 read only registers.
#define CLDA0        ETH_REG_BASE + 0x01
#define CLDA1        ETH_REG_BASE + 0x02
#define TSR          ETH_REG_BASE + 0x04
#define NCR          ETH_REG_BASE + 0x05
#define FIFO         ETH_REG_BASE + 0x06
#define CRDA0        ETH_REG_BASE + 0x08
#define CRDA1        ETH_REG_BASE + 0x09
#define _8019ID0     ETH_REG_BASE + 0x0A
#define _8019ID1     ETH_REG_BASE + 0x0B
#define RSR          ETH_REG_BASE + 0x0C
#define CNTR0        ETH_REG_BASE + 0x0D
#define CNTR1        ETH_REG_BASE + 0x0E
#define CNTR2        ETH_REG_BASE + 0x0F

// Page 0 write only registers.
#define PSTART       ETH_REG_BASE + 0x01   // Receive page start register
#define PSTOP        ETH_REG_BASE + 0x02   // Receive page stop register
#define TPSR         ETH_REG_BASE + 0x04   // Transmit page start register
#define TBCR0        ETH_REG_BASE + 0x05   // Transmit byte count register 0
#define TBCR1        ETH_REG_BASE + 0x06   // Transmit byte count register 1
#define RSAR0        ETH_REG_BASE + 0x08   // Remote start address register 0
#define RSAR1        ETH_REG_BASE + 0x09   // Remote start address register 0
#define RBCR0        ETH_REG_BASE + 0x0A   // Remote byte count register 0
#define RBCR1        ETH_REG_BASE + 0x0B   // Remote byte count register 1
#define RCR          ETH_REG_BASE + 0x0C   // Receive configuration register
    // Receive configuration register bits (write in page 0, read in page 2)
    #define MON      0x20                  // Monitor mode select bit
    #define PRO      0x10                  // Promiscuous mode select bit
    #define AM       0x08                  // Multicast packet accept bit
    #define AB       0x04                  // Broadcast packet accept bit
    #define AR       0x02                  // Runt packet accept bit
    #define SEP      0x01                  // Error packet accept bit
#define TCR          ETH_REG_BASE + 0x0D   // Transmit configuration register
    // Transmit configuration register bits
    #define OFST     0x10                  // Collision offset enable bit
    #define ATD      0x08                  // Auto transmit disable select bit
    #define LB1      0x04                  // Loopback mode select bit 1
    #define LB0      0x02                  // Loopback mode select bit 0
    #define CRC      0x01                  // CRC generation inhibit bit
#define DCR          ETH_REG_BASE + 0x0E   // Data configuration register
    // Data configuration register bits (write in page 0, read in page 2)
    #define FT1      0x40                  // FIFO threshold select bit 1
    #define FT0      0x20                  // FIFO threshold select bit 0
    #define ARM      0x10                  // Auto-initialise remote
    #define LS       0x08                  // Loopback select bit
    #define LAS      0x04                  // Set to 0 (pwrup = 1)
    #define BOS      0x02                  // Byte order select bit
    #define WTS      0x01                  // Word transfer select bit
#define IMR          ETH_REG_BASE + 0x0F   // Interrupt mask register
    // Interrupt mask register bits
    // Each enable bit correspons with an interrupt flag in ISR

// Page 1 read/write registers.
#define PAR0         ETH_REG_BASE + 0x01   // Physical address register 0
#define PAR1         ETH_REG_BASE + 0x02   // Physical address register 1
#define PAR2         ETH_REG_BASE + 0x03   // Physical address register 2
#define PAR3         ETH_REG_BASE + 0x04   // Physical address register 3
#define PAR4         ETH_REG_BASE + 0x05   // Physical address register 4
#define PAR5         ETH_REG_BASE + 0x06   // Physical address register 5
#define CURR         ETH_REG_BASE + 0x07   // Current receive buffer page
#define MAR0         ETH_REG_BASE + 0x08
#define MAR1         ETH_REG_BASE + 0x09
#define MAR2         ETH_REG_BASE + 0x0A
#define MAR3         ETH_REG_BASE + 0x0B
#define MAR4         ETH_REG_BASE + 0x0C
#define MAR5         ETH_REG_BASE + 0x0D
#define MAR6         ETH_REG_BASE + 0x0E
#define MAR7         ETH_REG_BASE + 0x0F

// Page 2 read only registers.
// Each previously defined in page 0 write only.
//#define PSTART     ETH_REG_BASE + 0x01
//#define PSTOP      ETH_REG_BASE + 0x02
//#define TPSR       ETH_REG_BASE + 0x04
//#define RCR        ETH_REG_BASE + 0x0C
//#define TCR        ETH_REG_BASE + 0x0D
//#define DCR        ETH_REG_BASE + 0x0E
//#define IMR        ETH_REG_BASE + 0x0F

// Page 3 read/write registers.
#define _9346CR      ETH_REG_BASE + 0x01   // 9346 EEPROM command register
    // 9346 EEPROM command register bits
    #define EEM1     0x80                  // RTL8019AS operating mode bit 1
    #define EEM0     0x40                  // RTL8019AS operating mode bit 0
    #define EECS     0x08                  // 9346 EEPROM chip select bit
    #define EESK     0x04                  // 9346 EEPROM serial clock bit
    #define EEDI     0x02                  // 9346 EEPROM data input bit
    #define EEDO     0x01                  // 9346 EEPROM data output bit
#define BPAGE        ETH_REG_BASE + 0x02
#define CONFIG1      ETH_REG_BASE + 0x04   // RTL9019AS config register 1
    // RTL9019AS config register 1 bits
    #define IRQEN    0x80                  // IRQ enable bit (WR protected)
    #define IRQS2    0x40                  // IRQ line select bit 2
    #define IRQS1    0x20                  // IRQ line select bit 1
    #define IRQS0    0x10                  // IRQ line select bit 0
    #define IOS3     0x08                  // I/O base address select bit 3
    #define IOS2     0x04                  // I/O base address select bit 2
    #define IOS1     0x02                  // I/O base address select bit 1
    #define IOS0     0x01                  // I/O base address select bit 0
#define CONFIG2      ETH_REG_BASE + 0x05   // 
    // RTL9019AS config register 2 bits
    #define PL1      0x80                  // Network medium type select bit 1
    #define PL0      0x40                  // Network medium type select bit 0
    #define BSELB    0x20                  // Boot ROM disable (WR protected)
    #define BS4      0x10                  // Boot ROM configuration bit 4
    #define BS3      0x08                  // Boot ROM configuration bit 3
    #define BS2      0x04                  // Boot ROM configuration bit 2
    #define BS1      0x02                  // Boot ROM configuration bit 1
    #define BS0      0x01                  // Boot ROM configuration bit 0
#define CONFIG3      ETH_REG_BASE + 0x06   // RTL9019AS config register 3
    // RTL9019AS config register 3 bits
    #define PNP      0x80                  // Plug & play mode indicator bit
    #define FUDUP    0x40                  // Full duplex mode select bit
    #define LEDS1    0x20                  // LED output select bit 1
    #define LEDS0    0x10                  // LED output select bit 0
    #define SLEEP    0x04                  // Sleep mode select bit
    #define PWRDN    0x02                  // Power down mode select bit
    #define ACTIVEB  0x01                  // Inverse of bit 0, PNP active reg

// Page 3 read only registers.
#define CONFIG0      ETH_REG_BASE + 0x03   // RTL9019AS config register 0
    // RTL9019AS config register 0 bits
    #define VERID1   0x80                  // RTL9019AS version ID bit 1 (R/W)
    #define VERID0   0x40                  // RTL9019AS version ID bit 0 (R/W)
    #define AUI      0x20                  // AUI input pin state bit
    #define PNPJP    0x10                  // PNP jumper pin state bit
    #define JP       0x08                  // JP input pin state bit
    #define BNC      0x04                  // Thinnet mode indication bit
#define CSNSAV       ETH_REG_BASE + 0x08
#define INTR         ETH_REG_BASE + 0x0B
#define CONFIG4      ETH_REG_BASE + 0x0D

// Page 3 write only registers.
#define TEST         ETH_REG_BASE + 0x07
#define HLTCLK       ETH_REG_BASE + 0x09
#define FMWP         ETH_REG_BASE + 0x0C




/*  ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ?
    ?                       Public Function Prototypes                   ?
    ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ? */
void etherdev_init(void);
void etherdev_send(void);
unsigned int etherdev_read(void);

#endif