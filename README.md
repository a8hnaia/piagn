# piagn
A 2D stack-based esolang.

# Building and Usage
Run `cate` or `./build.sh` to build. The executable is `piagn`. It takes a file to interpret as its argument.

# Language
This language features functions which allow you to bind a program to a letter. Functions all share the stack (the one where operations happens) but have their own piles (each of the 256 memory stacks) and it gets reset between calls. Functions are defined by putting a letter and the code between braces, like so:
```
A {
-.!,^%,&![|
\        [-_|
}
```
The brace must be in the same line as the letter. Any text outside function definitions is ignored. The main function is written without a letter before the opening brace. Alternatively, in a program without any functions other than main, braces may be omitted entirely.

A description of all the built-in operations can be found in `instructions.md`.


# License
This project is licensed under the MIT license. Please link to the repository in derivatives.
