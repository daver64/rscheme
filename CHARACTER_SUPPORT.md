# Character Type Support in RScheme

## Overview
RScheme now includes full R5RS-compliant character type support, allowing proper distinction between characters and strings.

## Features Implemented

### Core Character Type
- `SCHEME_CHAR` type added to type system
- Character literals: `#\a`, `#\space`, `#\newline`, `#\tab`
- Proper printing format: `#\c`

### Character Predicates
- `char?` - Test if object is a character
- `char=?` - Character equality
- `char<?` - Character less-than comparison
- `char>?` - Character greater-than comparison  
- `char<=?` - Character less-than-or-equal comparison
- `char>=?` - Character greater-than-or-equal comparison
- `char-alphabetic?` - Test if alphabetic character
- `char-numeric?` - Test if numeric character
- `char-whitespace?` - Test if whitespace character

### Character Operations
- `char-upcase` - Convert character to uppercase
- `char-downcase` - Convert character to lowercase
- `char->integer` - Get ASCII value of character
- `integer->char` - Convert ASCII value to character

### String Operations (Updated)
- `string-ref` now returns a character instead of a string (R5RS compliant)

## R5RS Compliance
The character implementation follows R5RS specifications:
- Characters are distinct from one-character strings
- `string-ref` returns characters, not strings
- Character literals use `#\` syntax
- Named characters: `#\space`, `#\newline`, `#\tab`
- Character comparisons work correctly
- Character predicates follow standard behavior

## Example Usage

```scheme
;; Character literals
#\a                           ; => #\a
#\space                       ; => #\space
#\newline                     ; => #\newline

;; Type checking
(char? #\a)                   ; => #t
(char? "a")                   ; => #f

;; String-ref returns characters
(string-ref "Hello" 0)        ; => #\H
(char? (string-ref "Hello" 0)) ; => #t

;; Character comparisons
(char=? #\a #\a)              ; => #t
(char<? #\a #\b)              ; => #t

;; Character predicates
(char-alphabetic? #\a)        ; => #t
(char-numeric? #\5)           ; => #t
(char-whitespace? #\space)    ; => #t

;; Case conversion
(char-upcase #\a)             ; => #\A
(char-downcase #\Z)           ; => #\z

;; Integer conversion
(char->integer #\A)           ; => 65
(integer->char 65)            ; => #\A
```

## Implementation Details

### Lexer Changes
- Special handling for `#\` character literal syntax
- Recognizes named characters (space, newline, tab)
- Parses single character literals like `#\a`

### Parser Changes
- Converts character literal tokens to SCHEME_CHAR objects
- Handles both named and single-character literals

### Interpreter Changes
- Characters are self-evaluating (like numbers and strings)
- Proper equality and comparison handling

### Type System Changes
- Added SCHEME_CHAR to SchemeType enum
- Added char_value to SchemeObject union
- Character objects are properly garbage collected

## Testing
All 20 example files pass, including:
- Example 19: String Processing (demonstrates character usage)
- R5RS compliance test shows proper `string-ref` behavior

## Future Enhancements
Potential additions for even more complete R5RS support:
- `string->list` and `list->string` for character list conversion
- `char-ci=?`, `char-ci<?`, etc. (case-insensitive comparisons)
- More named characters (e.g., `#\return`)
