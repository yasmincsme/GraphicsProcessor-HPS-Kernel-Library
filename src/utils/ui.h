#define WRREG 0xc0
#define DATA_B 0x70
#define DATA_A 0x80

#define LEFT 0
#define RIGHT 4
#define UP 2
#define DOWN 6
#define UPPER_RIGHT 1
#define UPPER_LEFT 3
#define BOTTOM_LEFT 5
#define BOTTOM_RIGHT 7

/*OPCODES*/
#define WBR 0000
#define WSM 0001
#define WBM 0010
#define DP 0011

/*START CLOCK PULSE GENERATOR*/
#define LOW_LOGIC_LEVEL 0x00000000
#define HIGHT_LOGIC_LEVEL 0x00000001

#define DEVICE_PATH "/dev/gpp_data_bus"