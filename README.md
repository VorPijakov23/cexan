# cexan
=====================================
cexan - is a simple command-line utility designed to display the contents of files in a hexadecimal and ASCII format. It is similar to tools like `hexdump`, but with a focus on simplicity and ease of use.

### Install
Build cexan
```bash
make
```
install cexan
```bash
make install
```
if you also want to install manpage, then make a trace
```bash
make install_all
```

### Uninstall
```bash
make uninstall
```

### Usage
```bash
cexan -b 28 ./example.txt
```

output:
```
00000000    48 65 6c 6c  6f 2c 20 69  27 6d 20 65  78 61 6d 70  6c 65 20 66  69 6c 65 21  0a      Hello, i'm example file!.
```

### License
cexan is licensed under the [GPL 3](LICENSE)
