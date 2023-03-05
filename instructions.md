# Instructions

## Constants

**Basically:** `0`: push 0 — `n`: push 2<sup>n−1</sup>

- `0`: push 0
- `1`: push 1
- `2`: push 2
- `3`: push 4
- `4`: push 8
- `5`: push 16
- `6`: push 32
- `7`: push 64
- `8`: push 128
- `9`: push 255

## Cursor control

- `@`: program starts here in the direction of the adjacent instruction; otherwise, starts at the top left, going right
if there are multiple `@`s or there are multiple adjacent instructions, the program crashes

### Conditionals

- `-`: if coming from left or right, do nothing; otherwise, if there's no operation to its left or right, halt; if there's only one, go in that direction; if there's two, pop stack, go cw if 0, otherwise ccw
- `|`: `-` but up and down
- `*`: pop stack; if 255, go ccw; if 0, go cw; otherwise go forward

### Mirrors

- `/`: mirror, sends left if coming from above, sends up if coming from left; sends right if coming from below, sends down if coming from right
- `\`: same but flipped

## Actions

### Stack manipulation

- `#`: pop stack — *n*, do next non-flow command (so excluding `-`, `|`, `*`, `/`, `\`, ` `) *n* many times
- `?`: push random byte to stack
- `!`: duplicate top of stack
- `%`: swap top two elements of stack
- `_`: pop stack, do nothing

### Operations

- `^`: pop top two, bitwise xor, push result
- `&`: `^` but and
- `+`: `^` but or
- `[`: pop stack, logical bit shift left with no cycling, push result
- `]`: `[` but right
- `~`: `[` but bitwise not

### Piles

- `$`: pop stack — *n*, go to *n*-th pile
- `<`: switch to pile on left, circular
- `>`: `>` but right
- `,`: pop from pile, push to stack
- `.`: pop from stack, push to pile

### Input / output

- `;`: take one byte as input, push to stack
- `:`: pop stack, print
