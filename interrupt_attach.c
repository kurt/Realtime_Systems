/*
 * part of int2.c
*/

#include <stdio.h>
#include <sys/neutrino.h>

#define HW_SERIAL_IRQ      3
#define REG_RX             0
#define REG_II             2
#define REG_LS             5
#define REG_MS             6
#define IIR_MASK           0x07
#define IIR_MSR            0x00
#define IIR_THE            0x02
#define IIR_RX             0x04
#define IIR_LSR            0x06
#define IIR_MASK           0x07

static int base_reg = 0x2f8;

int
main (int argc, char **argv)
{
    int  intId;         // interrupt id
    int  iir;           // interrupt identification register
    int  serial_msr;    // saved contents of Modem Status Reg
    int  serial_rx;     // saved contents of RX register
    int  serial_lsr;    // saved contents of Line Status Reg
    struct sigevent event;

    // usual main() setup stuff...

    // set up the event
    intId = InterruptAttachEvent (HW_SERIAL_IRQ, &event, 0);

    for (;;) {

        // wait for an interrupt event (could use MsgReceive instead)
        InterruptWait (0, NULL);

        /*
         * determine the source of the interrupt (and clear it)
         * by reading the Interrupt Identification Register
        */

        iir = in8 (base_reg + REG_II) & IIR_MASK;

        // unmask the interrupt, so we can get the next event
        InterruptUnmask (HW_SERIAL_IRQ, intId);

        /* no interrupt? */
        if (iir & 1) {
            /* then wait again for next */
            continue;
        }

        /*
         * figure out which interrupt source caused the interrupt,
         * and determine if we need to do something about it
        */

        switch (iir) {
        case    IIR_MSR:
            serial_msr = in8 (base_reg + REG_MS);

            /*
             * perform whatever processing you would've done in
             * the other example...
            */
            break;

        case    IIR_THE:
            /* do nothing */
            break;

        case    IIR_RX:
            /* note the character */
            serial_rx = in8 (base_reg + REG_RX);
            break;

        case    IIR_LSR:
            /* note the line status reg. */
            serial_lsr = in8 (base_reg + REG_LS);
            break;
        }
    }

    /* You won't get here. */
    return (0);
}