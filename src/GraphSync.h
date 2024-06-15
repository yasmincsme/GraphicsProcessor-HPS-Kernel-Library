#include <stdio.h>

#include "ui.h"

/**
 * @brief Structure representing a movable sprite.
 *
 * This structure represents a movable sprite in a game.
 * It contains information about its position, direction,
 * movement steps, and activation status.
 */
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

/**
 * @brief Structure representing a fixed sprite.
 *
 * This structure represents a fixed sprite in a game.
 * It contains information about its position, offset,
 * data register, and activation status.
 */
typedef struct sprite_fixed_t sprite_fixed_t;

struct sprite_fixed_t {
  int coord_x;       /**< X-coordinate of the fixed sprite. */
  int coord_y;       /**< Y-coordinate of the fixed sprite. */
  int offset;        /**< Offset used for bitmap selection. */
  int data_register; /**< Register where sprite information is stored. */
  int ativo;         /**< Activation status of the fixed sprite. */
};

/**
 * @brief Sets the properties of a movable sprite.
 *
 * This function sets the properties of a movable sprite
 * on the screen using the WBR instruction of the Graphics
 * Processor. The parameters correspond to the fields of
 * the instruction.
 *
 * @param reg Register to use.
 * @param x X-coordinate of the sprite.
 * @param y Y-coordinate of the sprite.
 * @param offset Offset used for bitmap selection.
 * @param activation_bit Activation status of the sprite.
 * @return 1 if the operation was successful, 0 otherwise.
 */
uint32_t set_sprite(int reg, int x, int y, int offset, int activation_bit);

/**
 * @brief Sets a background block with a specific color.
 *
 * This function sets a background block with a specific
 * color using the provided RGB values. It fills a block
 * of 8x8 pixels at the specified column and line.
 *
 * @param column Column of the background block.
 * @param line Line of the background block.
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 */
int set_background_block(int column, int line, int R, int G, int B);

/**
 * @brief Sets the base color of the background.
 *
 * This function sets the base color of the background
 * using the provided RGB values. It uses the WBR
 * instruction of the Graphics Processor.
 *
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 * @return 1 if the operation was successful, 0 otherwise.
 */
long long int set_background_color(int R, int G, int B);

/**
 * @brief Increases the coordinates of a movable sprite.
 *
 * This function updates the X and Y coordinates of a movable
 * sprite based on its movement angle and displacement value.
 *
 * @param sp Pointer to the sprite to update.
 * @param mirror Indicates if sprite coordinates should be mirrored.
 */
void increase_coordinate(sprite_t *sp, int mirror);

/**
 * @brief Checks collision between two sprites.
 *
 * This function checks for collision between two sprites
 * using the technique of Rectangle Overlap. It compares
 * the printing area of one sprite with the printing area
 * of the other sprite.
 *
 * @param sp1 Pointer to the first sprite.
 * @param sp2 Pointer to the second sprite.
 * @return 1 if collision is detected, 0 otherwise.
 */
int collision(sprite_t *sp1, sprite_t *sp2);
