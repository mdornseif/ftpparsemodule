/* Low Level Interface to Dan Bernsteins ftpparse library
   see http://cr.yp.to/ftpparse.html

   D. R. Tzeck - http://koeln.ccc.de/~drt/
     --drt@un.bewaff.net
*/

static char rcsid[] = "$Id: ftpparsemodule.c,v 1.5 2002/01/18 10:15:51 drt Exp $";
   
#include <Python.h>
#include "ftpparse.h"

static char module_Doc[] = "Provides access to ftpparse function and assosiated constants.

From Dan Bernstein's ftpparse page at http://cr.yp.to/ftpparse.html

---8<---------------------------------------------------------------

ftpparse is a library for parsing FTP LIST responses.

ftpparse currently understands the LIST output from any UNIX server,
Microsoft FTP Service, Windows NT FTP Server, VMS, WFTPD, NetPresenz,
NetWare, and MS-DOS. It also supports EPLF, a solution to the
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

---8<---------------------------------------------------------------

This Python module is based on Dan Bernstein's ftpparse
library at http://cr.yp.to/ftpparse.html Mr. Bernstein requests you to
inform him if you use his library in an commercial application.

It extends Bernstein's ftpparse code by providing a way to detect
symbolic links on some UNIX Servers.

You can get the version of the actual module you are using by 
querying '__version__' and '__rcsid__'.

The Python wrapper was hacked by drt@un.bewaff.net - http://c0re.jp/

You might be able to find more Information and updates at
http://c0re.jp/c0de/ftpparse/
";


/* Interface to dns_ip4 */
static char pyFtpParse_Doc[] = "Parsing of FTP Server responses.

ftpparse should be called with a list of lines obtained from an
FTP-Server. It returns a list of parsed filenames. If ftpparse can't
find a filename it will fill the corospondending entry in the output
list with None. If it can find a filename it returns a 10-element tupe
with Information about the entry.

(name, size, sizetype, mtime, mtimetype, cwd, retr, id, idtype, islink)

'name' is the name of the file.

'size' is the number of octets, if 'sizetype' is SIZE_BINARY this can
 be interpreted asw binary size, if sizetype is SIZE_ANSII it can be 
interpreted as ASCII size. If ftpparse can't decide sizetype is 
SIZE_UNKNOWN.

'mtime' is the modification time of the file. If mtimetype is MTIME_LOCAL it
can be assumed that the time is correct. If mtimetype is
MTIME_REMOTEMINUTE time zone and secs are unknown, if mtimetype is
MTIME_REMOTEDAY time zone and time of day are unknown.
When a time zone is unknown, it is assumed to be GMT. You may want
to use localtime() for LOCAL times, along with an indication that the
time is correct in the local time zone, and gmtime() for REMOTE* times.

If cwd is '1' it can be assumed that the file is a directory, if it is '0' 
the FTP CWD command is definitely pointless. 

If retr is '1' it can be assumed that the file can be retrived, if it is 
'0' the FTP RETR command is definitely pointless.

If 'id' is set the file can be accessed by an unique ID. This feature is 
to my knowledge only supported by Dan Bernstein's FTP servers. If idtype 
is ID_FULL there is really an unique identifier for files on this FTP server.
If idtype is ID_UNKNOWN nothing is known about the ID.

If 'islink' is set to one it can be assumed that the filename is a symbolic 
link pointing to another file. 

The module defines the constants NAME, SIZE, SIZETYPE, MTIME, MTIMETYPE, CWD, 
RETR, ID, IDTYPE and ISLINK to allow acces to the individual return valuse.

Example:
>>> import ftpparse
>>> ls = ['-rw-r--r--   1 root     other        531 Jan 29 03:26 README',
...       'dr-xr-xr-x   2 root     other        512 Apr  8  1994 etc', 
...       'Total of 11 Files, 10966 Blocks.']
>>> ftpparse.ftpparse(ls)
[('README', 531, 1, 980738760, 2, 0, 1, None, 0), ('etc', 512, 1, 765763200, 3, 1, 0, None, 0), None]
>>> for x in ftpparse.ftpparse(ls):
...   if x and x[ftpparse.MTIMETYPE] == ftpparse.MTIME_REMOTEMINUTE:
...     print x
... 
('README', 531, 1, 980738760, 2, 0, 1, None, 0)

";

static PyObject *pyFtpParse(self, args)
     PyObject *self;
     PyObject *args;
{
  PyObject *ret;
  PyObject *list;
  PyObject *pyline;
  PyObject *target;
  struct ftpparse fp;
  unsigned len;
  char *line;
  int i, islink;

  if(!PyArg_ParseTuple(args, "O!:ftpparse", &PyList_Type, &list))
    return NULL;
  
  target = PyList_New(0);
  
  for(i = 0; i < PyList_Size(list); i++)
    {
      if(!(pyline = PyList_GetItem(list, i)))
	return NULL;
      
      if(PyString_AsStringAndSize(pyline, &line, &len))
	return NULL; 

      if(line[len] == '\n')
	len--;
      if(line[len] == '\r')
	len--;

      islink = 0;
      if((len > 11) && (strncmp(line, "lrwxrwxrwx", 10) == 0))
	islink = 1;
      else
	islink = 0;
       
      if(ftpparse(&fp, line, len) != 1)
	  ret = Py_BuildValue("");
      else
	  ret = Py_BuildValue("(s#liliiis#ii)", fp.name, fp.namelen, fp.size, fp.sizetype, 
			      fp.mtime, fp.mtimetype, fp.flagtrycwd, fp.flagtryretr, 
			      fp.id, fp.idlen, fp.idtype, islink);

      if(PyList_Append(target, ret) != 0)
	return NULL; 
      Py_DECREF(ret);
    } 

  return target;
}


static PyMethodDef ftpparseMethods[] = {
      {"ftpparse", pyFtpParse, METH_VARARGS, pyFtpParse_Doc},
      {NULL, NULL}        /* Sentinel */
};

void initftpparse()
{
  PyObject *m, *d;

  m = Py_InitModule3("ftpparse", ftpparseMethods, module_Doc);
  d = PyModule_GetDict(m);

  PyDict_SetItemString(d, "SIZE_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_SIZE_UNKNOWN));
  /* size is the number of octets in TYPE I */
  PyDict_SetItemString(d, "SIZE_BINARY", 
		       Py_BuildValue("i", FTPPARSE_SIZE_BINARY));
  /* size is the number of octets in TYPE A */
  PyDict_SetItemString(d, "SIZE_ASCII", 
		       Py_BuildValue("i", FTPPARSE_SIZE_ASCII)); 
  PyDict_SetItemString(d, "MTIME_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_MTIME_UNKNOWN));
  /* time is correct */
  PyDict_SetItemString(d, "MTIME_LOCAL", 
		       Py_BuildValue("i", FTPPARSE_MTIME_LOCAL)); 
  /* time zone and secs are unknown */
  PyDict_SetItemString(d, "MTIME_REMOTEMINUTE", 
		       Py_BuildValue("i", FTPPARSE_MTIME_REMOTEMINUTE)); 
  /* time zone and time of day are unknown */
  PyDict_SetItemString(d, "MTIME_REMOTEDAY", 
		       Py_BuildValue("i", FTPPARSE_MTIME_REMOTEDAY)); 
  /*
    When a time zone is unknown, it is assumed to be GMT. You may want
    to use localtime() for LOCAL times, along with an indication that the
    time is correct in the local time zone, and gmtime() for REMOTE* times.
  */
  PyDict_SetItemString(d, "ID_UNKNOWN", 
		       Py_BuildValue("i", FTPPARSE_ID_UNKNOWN));
  /* unique identifier for files on this FTP server */
  PyDict_SetItemString(d, "ID_FULL", 
		       Py_BuildValue("i", FTPPARSE_ID_FULL)); 

  /* position of the tuple elements */
  PyDict_SetItemString(d, "NAME", Py_BuildValue("i", 0)); 
  PyDict_SetItemString(d, "SIZE", Py_BuildValue("i", 1)); 
  PyDict_SetItemString(d, "SIZETYPE", Py_BuildValue("i", 2)); 
  PyDict_SetItemString(d, "MTIME", Py_BuildValue("i", 3)); 
  PyDict_SetItemString(d, "MTIMETYPE", Py_BuildValue("i", 4)); 
  PyDict_SetItemString(d, "CWD", Py_BuildValue("i", 5)); 
  PyDict_SetItemString(d, "RETR", Py_BuildValue("i", 6)); 
  PyDict_SetItemString(d, "ID", Py_BuildValue("i", 7)); 
  PyDict_SetItemString(d, "IDTYPE", Py_BuildValue("i", 8)); 
  PyDict_SetItemString(d, "ISLINK", Py_BuildValue("i", 9)); 

  PyDict_SetItemString(d, "__rcsid__", Py_BuildValue("s", rcsid)); 
  PyDict_SetItemString(d, "__version__", Py_BuildValue("s", VERSION)); 
  
  if (PyErr_Occurred())
    Py_FatalError("Can't initialize module ftpparse.\n");
}


