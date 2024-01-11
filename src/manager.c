#include <stdlib.h> //srand,rand,
#include <time.h> //time
#define __USE_MISC
#include <math.h> //cos,sin

#include "manager.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define BLOCKS_WITHIN_A_TETROMINO 4

// should probably call these offsets
struct Offset {
    int xOffset;
    int yOffset;
};

struct Point {
    int x;
    int y;
};

struct Tetromino {
    enum TetrominoType type;
    struct TetrominoBlock* blocks[BLOCKS_WITHIN_A_TETROMINO];
};

// To find block (x,y) index = (x % FIELD_WIDTH) + (y * FIELD_WIDTH)
//    Height doesn't matter but height is just the factor of how tall the board is. 
//    And in the end each row is just the next representation of width * units, height just determines row count.
//    Doesn't factor into determining X,Y
struct TetrominoBlock* _blocks[FIELD_HEIGHT * FIELD_WIDTH];
struct TetrominoBlock* _blocks_updated[FIELD_HEIGHT * FIELD_WIDTH] = { NULL };
int _blocks_updated_length = 0;
bool _should_spawn_tetromino;
struct Tetromino fallingTetromino;
const char kSpawnX = FIELD_WIDTH / 2 - 1;
const struct Offset kShiftDownOffset = { .xOffset = 0, .yOffset = 1 };
const struct Offset kShiftLeftOffset = { .xOffset = -1, .yOffset = 0 };
const struct Offset kShiftRightOffset = { .xOffset = 1, .yOffset = 0 };

bool _should_tetromino_move_left = false;
bool _should_tetromino_move_right = false;
bool _should_tetromino_rotate = false;
bool _tetromino_can_spawn = true;

void m_initialize(void) {
    _m_blocks_alloc();
    _should_spawn_tetromino = true;
}

void m_update(void) {
    _m_blocks_updated_erase();
    if (_should_spawn_tetromino) {
        _should_tetromino_move_left = false;
        _should_tetromino_move_right = false;
        _m_tetromino_spawn();
        _should_spawn_tetromino = false;
    } else {
        // TODO move left and right refactor naming 

        if(_should_tetromino_rotate) {
            _m_falling_tetromino_rotate();
            _should_tetromino_rotate = false;
        }
        
        if (_should_tetromino_move_right && _m_falling_tetromino_can_move_right()) {
            _m_falling_tetromino_translate(kShiftRightOffset);
            _should_tetromino_move_right = false;
        }

        if (_should_tetromino_move_left && _m_falling_tetromino_can_move_left()) {
            _m_falling_tetromino_translate(kShiftLeftOffset);
            _should_tetromino_move_left = false;
        }

        _m_falling_tetromino_fall();
    }
}

void m_blocks_set_empty(void) {
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
        _m_block_set_type(_blocks[i], TT_EMPTY);
    }
}

bool m_tetromino_can_spawn(void) {
    return _tetromino_can_spawn;
}

void m_request_falling_tetromino_rotate(void) {
    _should_tetromino_rotate = true;
}

void m_request_falling_tetromino_move_left(void) {
    _should_tetromino_move_left = true;
    _should_tetromino_move_right = false;
}

void m_request_falling_tetromino_move_right(void) {
    _should_tetromino_move_right = true;
    _should_tetromino_move_left = false;
}

struct TetrominoBlock** m_blocks_get_updated(int* numberOfUpdatedBlocks) {
    *numberOfUpdatedBlocks = _blocks_updated_length;

    return &_blocks_updated[0];
}

void m_deactivate(void) {
    for(int i = 0; i < sizeof(_blocks) / sizeof(struct TetrominoBlock*); i++) {
        free(_blocks[i]);
    }
}

void _m_blocks_alloc(void) {
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
        struct TetrominoBlock* block = calloc(1, sizeof(struct TetrominoBlock));
        block->x = i % FIELD_WIDTH;
        block->y = i / FIELD_WIDTH;

        _blocks[i] = block;
    }
}

void _m_blocks_updated_erase() {
    _blocks_updated_length = 0;
}

void _m_tetromino_spawn() {
    enum TetrominoType type = _m_tetromino_type_get_random();

    struct Offset offsets[BLOCKS_WITHIN_A_TETROMINO];

    if (type == TT_O) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 0;

        offsets[2].xOffset = 0;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 1;
        offsets[3].yOffset = 1;
    } else if (type == TT_J) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    } else if (type == TT_L) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 1;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 2;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 0;
    } else if (type == TT_I) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 0;
        offsets[2].yOffset = 2;

        offsets[3].xOffset = 0;
        offsets[3].yOffset = 3;
    } else if (type == TT_S) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 1;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 0;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 0;
    } else if (type == TT_Z) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 0;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    } else if (type == TT_T) {
        offsets[0].xOffset = 1;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    }

    _m_blocks_spawn_at_offset_from_spawn_point(type, offsets);
}

enum TetrominoType _m_tetromino_type_random(void) {
    srand(time(NULL));
    return (enum TetrominoType) rand() % 6;
}

void _m_blocks_spawn_at_offset_from_spawn_point(enum TetrominoType type, struct Offset *offsets) {
    fallingTetromino.type = type;
    int index = 0;
    while(index < BLOCKS_WITHIN_A_TETROMINO) {
        struct Point spawnPoint;
        spawnPoint.x = kSpawnX + offsets->xOffset;
        spawnPoint.y = offsets->yOffset;

        struct TetrominoBlock* spawnBlock = _m_block_at_point(spawnPoint);
        if (spawnBlock->type != TT_EMPTY) {
            _tetromino_can_spawn = false;
            return;
        }
        _m_block_set_type(spawnBlock, type);

        // I'm not sure if this makes more sense to be here or return a list of spawnedBlocks to be handled in Parent function
        fallingTetromino.blocks[index] = spawnBlock;

        index++;
        offsets++;
    } 
}

struct TetrominoBlock *_m_block_at_point(struct Point point) {
    return _blocks[(point.x % FIELD_WIDTH) + (point.y * FIELD_WIDTH)];
}

void _m_block_set_type(struct TetrominoBlock *pTetrominoBlock, enum TetrominoType type) {
    pTetrominoBlock->type = type;
    _m_blocks_updated_register_block(pTetrominoBlock);
}

void _m_blocks_updated_register_block(struct TetrominoBlock *pTetrominoBlock) {
    if (_blocks_updated_length < FIELD_HEIGHT * FIELD_WIDTH) {
        _blocks_updated[_blocks_updated_length] = pTetrominoBlock;
        _blocks_updated_length++;
    }
}

void _m_falling_tetromino_rotate(void) {
    if (fallingTetromino.type != TT_O) {
        // i'm not a fan of this implementation
        // check this out instead and re-write
        // https://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array?page=1&tab=scoredesc#tab-top

    }
}

bool _m_falling_tetromino_can_move_right(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!_m_block_can_move_right(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

void _m_falling_tetromino_translate(struct Offset offset) {
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        _m_block_set_type(fallingTetromino.blocks[i], TT_EMPTY);
        _m_blocks_updated_register_block(fallingTetromino.blocks[i]);
    }

    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        struct Point updatedPoint = { .x = fallingTetromino.blocks[i]->x + offset.xOffset, .y = fallingTetromino.blocks[i]->y + offset.yOffset};
        fallingTetromino.blocks[i] = _m_block_at_point(updatedPoint);

        _m_block_set_type(fallingTetromino.blocks[i], fallingTetromino.type);
        _m_blocks_updated_register_block(fallingTetromino.blocks[i]);
    }
}

bool _m_point_intersects_static_block(struct Point point) {
    bool result = false;

    struct TetrominoBlock* blockAtPoint = _m_block_at_point(point);

    if (blockAtPoint->type != TT_EMPTY) {
        result = true;

        for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
            if (fallingTetromino.blocks[i] == blockAtPoint) {
                result = false;
                break;
            }
        }
    }

    return result;
}

bool _m_block_can_fall(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x, .y = block.y + 1};
    if (newBlockPoint.y >= FIELD_HEIGHT) {
        return false;
    } else if (_m_point_intersects_static_block(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}

bool _m_falling_tetromino_can_fall(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!_m_block_can_fall(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

void _m_falling_tetromino_fall(void) {
    if(_m_falling_tetromino_can_fall()) {
        _m_falling_tetromino_translate(kShiftDownOffset);
    } else {
        _should_spawn_tetromino = true;
    }
}

bool _m_block_can_move_left(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x - 1, .y = block.y };
    if (newBlockPoint.x < 0) {
        return false;
    } else if (_m_point_intersects_static_block(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}

bool _m_falling_tetromino_can_move_left(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!_m_block_can_move_left(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

bool _m_block_can_move_right(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x + 1, .y = block.y };
    if (newBlockPoint.x >= FIELD_WIDTH) {
        return false;
    }  else if (_m_point_intersects_static_block(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}
