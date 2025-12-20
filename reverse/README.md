## Reverse

Prints user input in reverse order to screen or file

## Usage

---

## Installation

```bash
make reverse
```

---

## Running

### No arguments

```bash
./reverse
```

- After writing a word, press **Enter** to input the next one
- Press **Ctrl + D** to end the input loop

---

### One argument

```bash
./reverse input.txt
```

- Reads input from `input.txt`
- Prints output to standard output

---

### Two arguments

```bash
./reverse input.txt output.txt
```

---

## Requirements

### Two-argument mode requires files

- `input.txt` must exist
- `output.txt` must exist and should be empty before execution (WARNING! DATA LOSS MAY OCCURR TO DATA IN OUTPUT.TXT)
- `input.txt` must contain words separated by newline characters
