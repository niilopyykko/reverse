## Unix-utilities

This repository contains multiple Unix-style utilities written in C.

## Usage

---

## Installation

```bash
make utils
```

---

## Running

### my-cat

```bash
./my-cat file1 "[file2 ...]"
```

- Reads one or more files
- Prints file contents to standard output

---

### my-grep

```bash
./my-grep searchterm
./my-grep searchterm file1
./my-grep searchterm file1 file2 ...

```

- Searches for `searchterm`
- If no file is given, reads from standard input
- Supports multiple files
- Prints matching lines to standard output...WITH COLOR!!! (amazing)

---

## ZIPPING

### One file compress

```bash
./my-zip input.txt > output.z
```

- Reads input from `input.txt`
- Compresses output to a file

---

### One file decompress

```bash
./my-unzip input.z > output.txt
```

- Reads input from `input.z`
- decompresses output to a file

---

### Multiple file compress

```bash
./my-zip input1.txt input2.txt > output.z
```

---

### Multiple file decompress

```bash
./my-unzip input1.z input2.z > output.txt
```

---

## Requirements

### Required files for {my-cat my-grep}

- name does not matter for files
- for decompress, switch .txt and .z around
- File extension may not actually matter

- `input.txt` must exist for input
- `output.z` must exist and should be empty before execution (WARNING! DATA LOSS WILL OCCURR TO DATA PREVIOUSLY IN OUTPUT)
- `input.txt` should contain text (ie. aabbaabb)

### Required files for zip

- name does not matter for files
- for decompress, switch .txt and .z around
- File extension may not actually matter

- `input.txt` must exist for input
- `output.z` must exist and should be empty before execution (WARNING! DATA LOSS WILL OCCURR TO DATA PREVIOUSLY IN OUTPUT)
- `input.txt` should contain text (ie. aabbaabb)
