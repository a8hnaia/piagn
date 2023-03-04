`0`: push 0
`1`: push 1
`2`: push 2
`3`: push 4
`4`: push 8
`5`: push 16
`6`: push 32
`7`: push 64
`8`: push 128
`9`: push 255

`-`: if coming from left or right, do nothing; otherwise, if there's no operation to its left or right, halt; if there's only one, go in that direction; if there's two, pop stack, go cw if 0, otherwise ccw
`|`: same thing but up and down
`*`: pop stack, if 255, go ccw; if 0, go cw; otherwise go forward

`/`: mirror, sends left if coming from above, sends up if coming from left; sends right if coming from below, sends down if coming from right
`\`: same but flipped

`@`: program starts here in the direction of the adjacent instruction; otherwise, starts at the top left, going right
if there are multiple `@`s or there are multiple adjacent instructions, the program crashes

`#`: pop stack, do next non-flow command (so excluding `-`, `|`, `*`, `/`, `\`, ` `) that many times

`?`: push random byte to stack

`!`: duplicate top of stack
`%`: swap top two elements of stack
`_`: pop stack, do nothing

`^`: pop top two, bitwise xor, push result
`&`: same but and
`+`: same but or
`[`: pop stack, logical bit shift left with no cycling, push result
`]`: same but right
`~`: same but bitwise not

`$`: pop stack, go to that-th pile
`<`: switch to pile on left, circular
`>`: same but right
`,`: pop from pile, push to stack
`.`: pop from stack, push to pile

`;`: take one byte as input, push to stack
`:`: pop stack, print
