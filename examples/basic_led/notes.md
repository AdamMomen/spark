### Noets

Before uploading we need to specify the port to find the port run the following command

```bash
ls /dev/cu.*
```


# Install libraries

The following command will check program size also download packages along the way

```bash
pio run --target checkprogsize
```


### Fix linting errors
Step 1: run this command
```bash
pio run --target checkprogsize
```
Step 2: Pres cmd+shift+p then choose `Restart Window` command from the list