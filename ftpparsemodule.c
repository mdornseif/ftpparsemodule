/* Low Level Interface to Dan Bernsteins ftpparse library
   see http://cr.yp.to/ftpparse.html

   D. R. Tzeck - http://koeln.ccc.de/~drt/
*/

static char rcsid[] = "$Id: ftpparsemodule.c,v 1.1 2001/12/25 07:44:18 drt Exp $";
   
#include <Python.h>
#include "ftpparse.h"

static char module_Doc[] = "
From Dan Bernsteins ftpparse page at http://cr.yp.to/ftpparse.html

ftpparse is a library for parsing FTP LIST responses.

ftpparse currently understands the LIST output from any UNIX server,
Microsoft FTP Service, Windows NT FTP Server, VMS, WFTPD, NetPresenz,
NetWare, and MSDOS. It also supports EPLF, a solution to the
LIST-parsing mess.

ftpparse parses file modification times into time_t, so you can easily
compare them and display them in your favorite format. It lets you
know how precise the time_t is: LOCAL meaning exact with known time
zone (available from EPLF), REMOTEMINUTE meaning unknown time zone and
seconds, or REMOTEDAY meaning unknown time zone and time of day.

To use ftpparse, simply feed each line of LIST output to the
ftpparse() routine, along with a pointer to a struct ftpparse. If
ftpparse() returns 1, you can find a filename and various other useful
information inside the struct ftpparse.

Commercial use of ftpparse is fine, as long as you let me know what
programs you're using it in.
";


/* Interface to dns_ip4 */
static char pyFtpParse_Doc[] = "YYY";

static PyObject *pyFtpParse(self, args)
     PyObject *self;
     PyObject *args;
{
  PyObject *ret;
  struct ftpparse fp;
  unsigned len;
  char *line;

  if(!PyArg_ParseTuple(args, "s#", &line, &len))
    return NULL;
  
  if(ftpparse(&fp, line, len) != 1)
    {
      ret = Py_BuildValue("");
   }
  else
    {
      ret = Py_BuildValue("s#liliiis#i", fp.name, fp.namelen, fp.size, fp.sizetype, fp.mtime, fp.mtimetype, fp.flagtrycwd, fp.flagtryretr, fp.id, fp.idlen, fp.idtype);
    }

  return ret;
}


/*
"Curtis Jensen:
 |I need to return a list of arrays from a CAPI.  I'm using the Numeric
 |module to create some C extensions.  Though, I need to return several
 |arrays to Python.  How can I do this?

You can start with something like this:

    target = PyList_New( verts->length );
    for ( i = 0; i < vertes->length; i++ )
      PyList_SetItem( target, i, 
                      PyFloat_FromDouble( (double) verts->vertices[i] ) );

Add error checking, and modify to taste using the C API reference guide:

    http://www.python.org/doc/current/api/api.html
";
*/

static PyMethodDef ftpparseMethods[] = {
      {"ftpparse",         pyFtpParse,         METH_VARARGS, pyFtpParse_Doc},
      {NULL,      NULL}        /* Sentinel */
};

void initftpparse()
{
  PyObject *m, *d;

  m = Py_InitModule3("ftpparse", ftpparseMethods, module_Doc);
  d = PyModule_GetDict(m);

  PyDict_SetItemString(d, "SIZE_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_SIZE_UNKNOWN ));
  /* size is the number of octets in TYPE I */
  PyDict_SetItemString(d, "SIZE_BINARY", 
		       Py_BuildValue("i", FTPPARSE_SIZE_BINARY ));
  /* size is the number of octets in TYPE A */
  PyDict_SetItemString(d, "SIZE_ASCII", 
		       Py_BuildValue("i", FTPPARSE_SIZE_ASCII )); 
  PyDict_SetItemString(d, "MTIME_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_MTIME_UNKNOWN ));
  /* time is correct */
  PyDict_SetItemString(d, "MTIME_LOCAL", 
		       Py_BuildValue("i", FTPPARSE_MTIME_LOCAL )); 
  /* time zone and secs are unknown */
  PyDict_SetItemString(d, "MTIME_REMOTEMINUTE", 
		       Py_BuildValue("i", FTPPARSE_MTIME_REMOTEMINUTE )); 
  /* time zone and time of day are unknown */
  PyDict_SetItemString(d, "MTIME_REMOTEDAY", 
		       Py_BuildValue("i", FTPPARSE_MTIME_REMOTEDAY )); 
  /*
    When a time zone is unknown, it is assumed to be GMT. You may want
    to use localtime() for LOCAL times, along with an indication that the
    time is correct in the local time zone, and gmtime() for REMOTE* times.
  */
  PyDict_SetItemString(d, "ID_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_ID_UNKNOWN ));
  /* unique identifier for files on this FTP server */
  PyDict_SetItemString(d, "ID_FULL ", 
		       Py_BuildValue("i",FTPPARSE_ID_FULL )); 

    if (PyErr_Occurred())
      Py_FatalError("Can't initialize module ftpparse.\n");
}


