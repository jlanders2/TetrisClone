#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define __USE_MISC
#include <math.h>

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
struct TetrominoBlock* _updatedBlocks[FIELD_HEIGHT * FIELD_WIDTH] = { NULL };
int _updatedBlockLength = 0;
bool shouldSpawnTetromino;
struct Tetromino fallingTetromino;
const char kSpawnX = FIELD_WIDTH / 2 - 1;
const struct Offset kShiftDownOffset = { .xOffset = 0, .yOffset = 1 };
const struct Offset kShiftLeftOffset = { .xOffset = -1, .yOffset = 0 };
const struct Offset kShiftRightOffset = { .xOffset = 1, .yOffset = 0 };

bool moveLeft = false;
bool moveRight = false;
bool rotate = false;
bool cannotSpawn = false;

void CleanupManager(void) {
    for(int i = 0; i < sizeof(_blocks) / sizeof(struct TetrominoBlock*); i++) {
        free(_blocks[i]);
    }
}

struct TetrominoBlock** GetUpdatedBlocks(int* numberOfUpdatedBlocks) {
    *numberOfUpdatedBlocks = _updatedBlockLength;

    return &_updatedBlocks[0];
}

void RequestRotate(void) {
    rotate = true;
}

void RequestMoveLeft(void) {
    moveLeft = true;
    moveRight = false;
}

void RequestMoveRight(void) {
    moveRight = true;
    moveLeft = false;
}

struct TetrominoBlock* GetBlockAtPoint(struct Point point) {
    return _blocks[(point.x % FIELD_WIDTH) + (point.y * FIELD_WIDTH)];
}

void RegisterUpdatedBlock(struct TetrominoBlock* pTetrominoBlock) {
    if (_updatedBlockLength < FIELD_HEIGHT * FIELD_WIDTH) {
        _updatedBlocks[_updatedBlockLength] = pTetrominoBlock;
        _updatedBlockLength++;
    }
}

void UpdateBlockType(struct TetrominoBlock* pTetrominoBlock, enum TetrominoType type) {
    pTetrominoBlock->type = type;
    RegisterUpdatedBlock(pTetrominoBlock);
}

void ShiftFallingTetrominoByOffset(struct Offset offset) {
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        UpdateBlockType(fallingTetromino.blocks[i], EMPTY);
        RegisterUpdatedBlock(fallingTetromino.blocks[i]);
    }

    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        struct Point updatedPoint = { .x = fallingTetromino.blocks[i]->x + offset.xOffset, .y = fallingTetromino.blocks[i]->y + offset.yOffset};
        fallingTetromino.blocks[i] = GetBlockAtPoint(updatedPoint);

        UpdateBlockType(fallingTetromino.blocks[i], fallingTetromino.type);
        RegisterUpdatedBlock(fallingTetromino.blocks[i]);
    }
}

// I HATE THIS NAME... but it's verbose for now TODO - RENAME
bool DoesPointIntersectNonFallingBlock(struct Point point) {
    bool result = false;

    struct TetrominoBlock* blockAtPoint = GetBlockAtPoint(point);

    if (blockAtPoint->type != EMPTY) {
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

bool CanBlockFall(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x, .y = block.y + 1};
    if (newBlockPoint.y >= FIELD_HEIGHT) {
        return false;
    } else if (DoesPointIntersectNonFallingBlock(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}

bool FallingTetrominoCanFall(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!CanBlockFall(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

void UpdateFallingTetromino(void) {
    if(FallingTetrominoCanFall()) {
        ShiftFallingTetrominoByOffset(kShiftDownOffset);
    } else {
        shouldSpawnTetromino = true;
    }
}

bool CanBlockMoveLeft(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x - 1, .y = block.y };
    if (newBlockPoint.x < 0) {
        return false;
    } else if (DoesPointIntersectNonFallingBlock(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}

bool FallingTetrominoCanMoveLeft(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!CanBlockMoveLeft(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

bool CanBlockMoveRight(struct TetrominoBlock block) {
    struct Point newBlockPoint = { .x = block.x + 1, .y = block.y };
    if (newBlockPoint.x >= FIELD_WIDTH) {
        return false;
    }  else if (DoesPointIntersectNonFallingBlock(newBlockPoint)) {
        return false;
    } else {
        return true;
    }
}

bool FallingTetrominoCanMoveRight(void) {
    bool result = true;
    for(int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        if (!CanBlockMoveRight(*(fallingTetromino.blocks[i]))) {
            result = false;
            break;
        }
    }

    return result;
}

struct Point GetRotatedPointForBlock(struct Point origin, struct TetrominoBlock block) {
    // Only doing counter clockwise for now
    struct Point newPoint;
    newPoint.x = (block.x - origin.x) * cos(M_PI_2) - (block.y - origin.y) * sin(M_PI_2);
    newPoint.y = (block.x - origin.x) * sin(M_PI_2) + (block.y - origin.y) * cos(M_PI_2);

    return newPoint;
}

void RotateFallingTetromino(void) {
    // Will be a bit buggy because i'm not checking for intersections
    struct Point origin = { .x = fallingTetromino.blocks[0]->x, .y = fallingTetromino.blocks[0]->y };

    for (int i = 0; i < BLOCKS_WITHIN_A_TETROMINO; i++) {
        // Empty the old
        UpdateBlockType(fallingTetromino.blocks[i], EMPTY);
        RegisterUpdatedBlock(fallingTetromino.blocks[i]);

        // Update the new
        struct Point rotatedPointAdjustedForOrigin = GetRotatedPointForBlock(origin, *fallingTetromino.blocks[i]);
        struct Point newRotatedPoint = { .x = rotatedPointAdjustedForOrigin.x + origin.x, .y = rotatedPointAdjustedForOrigin.y + origin.y };
        fallingTetromino.blocks[i] = GetBlockAtPoint(newRotatedPoint);

        UpdateBlockType(fallingTetromino.blocks[i], fallingTetromino.type);
        RegisterUpdatedBlock(fallingTetromino.blocks[i]);
    }
}

void TryRotateFallingTetromino(void) {
    // TODO IMPLEMENT
    if (fallingTetromino.type == O_TYPE) {
        return;
    } else {
        RotateFallingTetromino();
    }
}

void SpawnBlocksAtPoints(enum TetrominoType type, struct Offset* offsets) {
    fallingTetromino.type = type;
    int index = 0;
    while(index < BLOCKS_WITHIN_A_TETROMINO) {
        struct Point spawnPoint;
        spawnPoint.x = kSpawnX + offsets->xOffset;
        spawnPoint.y = offsets->yOffset;

        struct TetrominoBlock* spawnBlock = GetBlockAtPoint(spawnPoint);
        if (spawnBlock->type != EMPTY) {
            cannotSpawn = true;
            return;
        }
        UpdateBlockType(spawnBlock, type);

        // I'm not sure if this makes more sense to be here or return a list of spawnedBlocks to be handled in Parent function
        fallingTetromino.blocks[index] = spawnBlock;

        index++;
        offsets++;
    } 
}

enum TetrominoType GenerateRandomTetrominoType() {
    srand(time(NULL));
    return (enum TetrominoType) rand() % 6;
}

void SpawnTetromino() {
    enum TetrominoType type = GenerateRandomTetrominoType();

    struct Offset offsets[BLOCKS_WITHIN_A_TETROMINO];

    if (type == O_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 0;

        offsets[2].xOffset = 0;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 1;
        offsets[3].yOffset = 1;
    } else if (type == J_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    } else if (type == L_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 1;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 2;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 0;
    } else if (type == I_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 0;
        offsets[2].yOffset = 2;

        offsets[3].xOffset = 0;
        offsets[3].yOffset = 3;
    } else if (type == S_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 1;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 0;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 0;
    } else if (type == Z_TYPE) {
        offsets[0].xOffset = 0;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 1;
        offsets[1].yOffset = 0;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    } else if (type == T_TYPE) {
        offsets[0].xOffset = 1;
        offsets[0].yOffset = 0;

        offsets[1].xOffset = 0;
        offsets[1].yOffset = 1;

        offsets[2].xOffset = 1;
        offsets[2].yOffset = 1;

        offsets[3].xOffset = 2;
        offsets[3].yOffset = 1;
    }

    SpawnBlocksAtPoints(type, offsets);
}

void DeregisterUpdatedBlocks() {
    _updatedBlockLength = 0;
}

void Update(void) {
    DeregisterUpdatedBlocks();
    if (shouldSpawnTetromino) {
        moveLeft = false;
        moveRight = false;
        SpawnTetromino();
        shouldSpawnTetromino = false;
    } else {
        // TODO move left and right refactor naming 

        if(rotate) {
            TryRotateFallingTetromino();
            rotate = false;
        }
        
        if (moveRight && FallingTetrominoCanMoveRight()) {
            ShiftFallingTetrominoByOffset(kShiftRightOffset);
            moveRight = false;
        }

        if (moveLeft && FallingTetrominoCanMoveLeft()) {
            ShiftFallingTetrominoByOffset(kShiftLeftOffset);
            moveLeft = false;
        }

        UpdateFallingTetromino();
    }
}

bool CantSpawnBlock(void) {
    return cannotSpawn;
}

void EmptyAllBlocks(void) {
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
        UpdateBlockType(_blocks[i], EMPTY);
    }
}

void AllocBlocks(void) {
    for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
        struct TetrominoBlock* block = calloc(1, sizeof(struct TetrominoBlock));
        block->x = i % FIELD_WIDTH;
        block->y = i / FIELD_WIDTH;

        _blocks[i] = block;
    }
}

void InitManager(void) {
    AllocBlocks();
    shouldSpawnTetromino = true;
}