## Parallel Zip / Unzip

Parallel version of zip and unzip utilities written in C.  
The programs support compressing and decompressing one or multiple files.

## Usage

---

## Installation

```bash
make parallel
```

---

## Running

### One file compress

```bash
./pzip input.txt > output.z
```

- Reads input from `input.txt`
- Compresses output to a file

---

### One file decompress

```bash
./punzip input.z > output.txt
```

- Reads input from `input.z`
- decompresses output to a file

---

### Multiple file compress

```bash
./pzip input1.txt input2.txt > output.z
```

---

### Multiple file decompress

```bash
./punzip input1.z input2.z > output.txt
```

---

## Requirements

### Required files

- name does not matter for files
- for decompress, switch .txt and .z around
- File extension may not actually matter

- `input.txt` must exist for input
- `output.z` must exist and should be empty before execution (WARNING! DATA LOSS WILL OCCURR TO DATA PREVIOUSLY IN OUTPUT)
- `input.txt` should contain text (ie. aabbaabb)
