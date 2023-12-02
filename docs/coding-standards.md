# Coding Standards

## Generated Configuration for Linter and Formatter

For C language ...

- **Formatter:** `clang-format`

    ```console
    clang-format 
    --style="{BasedOnStyle: LLVM, IndentWidth: 4}" 
    --dump-config > .clang-format
    ```

- **Linter:** `clang-tidy`

    ```console
    clang-tidy 
    -checks='bugprone*,clang-analyzer*,concurrency*,
      cppcoreguidelines*,google*,hicpp*,llvm-*,misc*,
      modernize*,performance*,readability*,' 
    -format-style='{BasedOnStyle: llvm, IndentWidth:4}'
    --dump-config > .clang-tidy
    ```

## File Naming

If possible, never use spaces or underscores. Spaces are
converted to %20 in URLs or can break a URL when shared.
Underscores are difficult to see when the file name is
displayed as an underlined link.

### Folders

Lowercase without separators. e.g.,

```console
foldername/
```

### C and Header Files

Lowercase filenames maximum of 15 characters. Preferably
no separators else use underscores (_). e.g.,

```console
sourcefile.c
source_file.c
headerfile.h
header_file.h
```

### Markdown and Text Files

Lowercase with dash (-) as separators. e.g.,

```console
filename-text.txt
filename-markdown.md
```

## Coding Conventions

Inconsistent naming, formatting, and style on the project
generates confusion. Having a standardized convention
might limit this problem. In that case, we strictly
implement these listed conventions sourced from forums
and other projects like LLVM. References listed below.

**Formatting Tool:** [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

**Linter:** [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)

**Based on Style:** Variation of LLVM style with 4 space
indentation

### Indentation

Use 4 spaces for indentation instead of tabs to ensure
consistency.

```c
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
    // statements
}
```

### Space Before Parenthesis

Should only be applicable in control flow statements.

```c
if (x = 1) { ... }
for (x = 0; x < 100; x++) { ... }
while (1) { ... }

doFunction(x);
printf("...");
```

### Source Code Width of 80

Code width maximum of 80 characters. Wrap long lines.
Here are several (googled) reasons behind this:

- It forces writing a more succinct code
- Humans are better at processing information in smaller
quantity portions
- It helps users of vi/vim (and potentially other editors)
who use vertical splits

### Comment formatting

Comments should only appear when necessary, especially
for complex functionalities that might require
clarification. Stick only to `// in-line comments`.

Use multiple in-line comments to represent a block. e.g.,

```c
// in-line comment
// in-line comment
// in-line comment
```

### Language and Compiler Issues

Treat warnings as errors and write portable code. Adhere to
[ANSI C](https://www.csse.uwa.edu.au/programming/ansic-library.html#stdlib)
or C89 library standards for compatibility across platforms.

For compatibility issues, visit our [Issues page on GitHub](https://github.com/steguiosaur/renaisscript/issues)

### Writing Error and Warning Messages

Use clear diagnosis for error messages. Start with
lowercase letter, and finish last sentence without
period.

```console
[ERROR] file.rens: accessing undeclared variable name 'h' on line 12
```

### Name Types, Functions, Variables, and Enumerators

Modified copy from [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html#source-code-formatting)
with little changes.

#### Type Names

Including classes, structs, enums, typedefs, etc. should be
nouns and in `PascalCase`. e.g.,

```c
enum TokenType {
    // ENUM_MEMBERS
};
```

#### Variable and Parameter Names

Should be nouns (as they represent state). The name should
be in `snake_case`. e.g.,

```c
char *input_file;
void getSquare(int number); // getSquare function declaration
```

#### Function names

Should be verb phrases (as they represent actions), and
command-like function should be imperative. The name
should be `camelCase`, which starts with a lower case
letter. e.g.,

```c
void getSquare(int number) {
    return number * number;
}
```

#### Enum declarations

(e.g. `enum Foo {...}`) are types, so they should follow
the naming conventions for types. A common use for enums
is as a discriminator for a union, or an indicator of a
subclass. When an enum is used for something like this, it
should have a Kind suffix (e.g. `NodeKind`).

```c
// Type of AST Node
enum NodeKind {
    NODE_ROOT,
    NODE_FUNCTION,
    NODE_DECLARATION,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_CALL,
    NODE_CAST,
    // ...,
};
```

#### Enumerators and Public Member Variables

This (e.g. `enum { FOO, BAR }` for Enumerator values)
should be in `UPPERCASE_SNAKE_CASE`. Unless the enumerators are
defined in their own small namespace or inside a class,
enumerators should have a prefix corresponding to the
enum declaration name. For example, `enum TokenType { ... };`
may contain enumerators like `TK_IF`, `TK_NUMBER`, etc.
Enumerators that are just convenience constants are exempt
from the requirement for a prefix. For instance:

## References

- [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html#source-code-formatting)
- [File Name Conventions](https://github.com/bartvandebiezen/file-name-conventions)
- [Recommended C Style and Coding Standards](https://www.doc.ic.ac.uk/lab/cplus/cstyle.html)
- [What are the most common naming conventions in C?](https://stackoverflow.com/questions/1722112/what-are-the-most-common-naming-conventions-in-c)
- [The ANSI C standard library](https://www.csse.uwa.edu.au/programming/ansic-library.html#stdlib)

