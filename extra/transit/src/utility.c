#include "utility.h"
#include <string.h>

char *text2char(text *in) {
  char *out = malloc(VARSIZE(in)); // FIXME: Using palloc loses values

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

text *char2text(char *in) {
  int len = strlen(in);
  text *out = (text *) malloc(VARHDRSZ + len); // FIXME: Using palloc loses values
  SET_VARSIZE(out, VARHDRSZ + len);
  memcpy(out->vl_dat, in, len);
  return out;
}

int finish(int code, int ret) {
  code = SPI_finish();
  if (code != SPI_OK_FINISH) {
    elog(ERROR, "couldn't disconnect from SPI");
    return -1;
  }

  return ret;
}
