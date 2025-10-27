# C Publish-Subscribe System

This project is a simple implementation of a Publish-Subscribe system in the C programming language. It allows for the creation of information, groups, and subscribers. Subscribers can subscribe to groups and consume the information published to those groups.

## Compilation

To compile the project, you can use the following GCC command:

```bash
gcc -ansi main.c pss.c -o run
```

This will create an executable file named `run`.

## Usage

The program takes a single command-line argument: the path to an input file.

```bash
./run <input_file>
```

### Input File Format

The input file should contain a series of commands, one per line. The available commands are:

*   **Insert Info:** `I <itm> <iId> <gId1> <gId2> ... <gIdk> -1`
    *   `itm`: Timestamp of arrival.
    *   `iId`: Identifier of the information.
    *   `gId1`, `gId2`, ...: A space-separated list of group IDs to publish the information to. The list is terminated by `-1`.

*   **Subscriber Registration:** `S <sΤΜ> <sId> <gId1> <gId2> ... <gIdm> -1`
    *   `sTM`: Timestamp of arrival.
    *   `sId`: Identifier of the subscriber.
    *   `gId1`, `gId2`, ...: A space-separated list of group IDs to subscribe to. The list is terminated by `-1`.

*   **Consume:** `C <sId>`
    *   `sId`: The ID of the subscriber that will consume the information.

*   **Delete Subscriber:** `D <sId>`
    *   `sId`: The ID of the subscriber to delete.

*   **Print:** `P`
    *   Prints the current state of all data structures in the system.

*   **Comments:** Lines starting with `#` are ignored.

### Example Input File

```
# This is a comment
I 1 101 1 2 -1
I 2 102 2 3 -1
S 1 201 1 3 -1
C 201
D 201
P
```
