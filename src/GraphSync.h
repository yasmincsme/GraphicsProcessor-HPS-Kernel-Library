#include <stdio.h>

#include "utils/types.h"
#include "utils/ui.h"

typedef enum sprite_type_t;

typedef enum { SPRITE_NORMAL, SPRITE_FIXED } sprite_type_t;


typedef struct sprite_t sprite_t;

struct sprite_t {
  int coord_x;       /**< X-coordinate of the sprite. */
  int coord_y;       /**< Y-coordinate of the sprite. */
  int direction;     /**< Direction of movement. */
  int offset;        /**< Offset used for bitmap selection. */
  int data_register; /**< Register where sprite information is stored. */
  int step_x;        /**< Number of pixels to move in the X direction. */
  int step_y;        /**< Number of pixels to move in the Y direction. */
  int ativo;         /**< Activation status of the sprite. */
  int collision;     /**< Collision status of the sprite. */
};

typedef struct sprite_fixed_t sprite_fixed_t;

struct sprite_fixed_t {
  int coord_x;       /**< X-coordinate of the fixed sprite. */
  int coord_y;       /**< Y-coordinate of the fixed sprite. */
  int offset;        /**< Offset used for bitmap selection. */
  int data_register; /**< Register where sprite information is stored. */
  int ativo;         /**< Activation status of the fixed sprite. */
};

void write_data(u64_t fd, u64_t data);

u64_t read_data(u64_t fd);

//uint64_t set_background_block(int column, int line, int R, int G, int B);

u64_t set_background_color(u8_t R, u8_t G, u8_t B);

void increase_coordinate(sprite_t *sp, int mirror);

int collision(sprite_t *sp1, sprite_t *sp2);

//void write_DP(u8_t type, u8_t R, u8_t G, u8_t B, u8_t lenght, u16_t x, u16_t y, u16_t adress);

//void write_WSM(u8_t R, u8_t G, u8_t B, u32_t adress);

//void write_WBR_sprite(u8_t ativo, u16_t x, u16_t y, u16_t offset, u8_t reg);

//void write_WBR_background(u8_t R, u8_t G, u8_t B);

u64_t set_polygon(int reg, int format, int R, int G, int B, int size, int ref_point_y, int ref_point_x);

u64_t set_sprite(u8_t reg, u8_t sp, u16_t x, u16_t y, u16_t offset);
