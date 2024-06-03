#include <asm/io.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <src\utils\ui.h>

#include "/home/aluno/TEC499/LabExemples/auxFiles/address_map_arm.h"

void *LW_virtual;                                  /*Lightweight bridge base address*/
volatile int *WRREG_ptr, *DATA_A_ptr, *DATA_B_ptr; /*Start pulse, Data A bus and Data B bus base address*/

/*Default function for initialization of the module*/
static int __init initialize_module(void) {
  /*generate a virtual address for the FPGA lightweight bridge*/
  LW_virtual = ioremap_nocache(LW_BRIDGE_BASE, LW_BRIDGE_SPAN);

  WRREG_ptr = LW_virtual + WRREG;   /*Virtual address for Start pulse port*/
  DATA_A_ptr = LW_virtual + DATA_A; /*Virtual address for Data A bus port*/
  DATA_B_ptr = LW_virtual + DATA_B; /*Virtual address for Data B bus port*/

  return 0;
}

/*Default function for cleanup and unload the module */
static void __exit cleanup_module(void) {
  iounmap(LW_virtual);
}

module_init(initialize_module);
module_exit(cleanup_module);