Purpose of this directory
----------------------------------

- Directory having files included exclusively by the `*.c` files
- forbidden to be used on c++ code
  - this prohibition is not enforced but will be in the future

- Convention:
  - files must end with `_input`


Example
----------------------------------

in `foo.c` file

    #include "c_common/edges_input.h"
    #include "c_common/arrays_input.h"

