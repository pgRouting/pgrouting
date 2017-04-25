Purpose of this directory
----------------------------------

- Directory to have the files included exclusively by the `*.c` files
- forbidden to be used on c++ code
  - this prohibition is not enforced but will be in the future

Example
----------------------------------

in `dijkstra.c` file

```
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
```

