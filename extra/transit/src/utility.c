#include "utility.h"

char *text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

int finish(int code, int ret)
{
  code = SPI_finish();
  if (code != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }
                        
  return ret;
}
