# piagn
A 2D stack-based esolang.

## Building and Usage
Run `cate` or `./build.sh` to build. The executable is `piagn`. It takes a file to interpret as its argument.

## Language
Piagn allows you to bind a function to a letter. Functions all share the stack (the one where operations happen) but have their own piles (each of the 256 memory stacks) and the stack gets reset between calls.

Functions are defined by putting a letter and the code inside braces, like this:
```
A {
-.!,^%,&![|
\        [-_|
}
```
The brace (`{`) must be in the same line as the letter. Any text outside function definitions is ignored. The main function is written without a letter before the opening brace. Braces can also not be used in a program that only has main.

Descriptions of all the built-in operations can be found in [`instructions.md`](instructions.md).


## License
This project is licensed under the MIT license. Please link to this repository in derivatives.
