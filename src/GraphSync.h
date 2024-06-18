#include <stdio.h>

#include "utils/types.h"
#include "utils/ui.h"

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

typedef struct polygon_t polygon_t;

struct polygon_t {
  u16_t ref_point_x;  /**< X-coordinate of the fixed sprite. */
  u16_t ref_point_y;  /**< Y-coordinate of the fixed sprite. */
  u8_t size;          /**< Offset used for bitmap selection. */
  u8_t data_register; /**< Register where sprite information is stored. */
  u8_t type;
  u8_t R;
  u8_t G;
  u8_t B; /**< Activation status of the fixed sprite. */
};

typedef struct ground_block_t ground_block_t;

struct ground_block_t {
  u64_t address;
  u8_t R;
  u8_t G;
  u8_t B;
};

void write_data(u64_t data);

u64_t read_data();

u64_t set_background_block(ground_block_t block);

u64_t set_fixed_sprite(sprite_fixed_t sprite);

u64_t set_dynamic_sprite(sprite_t sprite);

u64_t set_background_color(u8_t R, u8_t G, u8_t B);

u64_t set_polygon(polygon_t polygon);

// u8_t increase_coordinate_sprite(sprite_t *sprite, int mirror);
