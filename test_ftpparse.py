ftplines = [ "+i8388621.29609,m824255902,/,\tdev",
             # ELPF - see http://pobox.com/~djb/proto/eplf.txt
             "+i8388621.44468,m839956783,r,s10376,\tRFCEPLF",
             # UNIX-style listing, without inum and without blocks
             "-rw-r--r--   1 root     other        531 Jan 29 03:26 README",
             "dr-xr-xr-x   2 root     other        512 Apr  8  1994 etc",
             "dr-xr-xr-x   2 root     512 Apr  8  1994 etc",
             "lrwxrwxrwx   1 root     other          7 Jan 25 00:17 bin -> usr/bin",
             # Also produced by Microsoft's FTP servers for Windows:
             "----------   1 owner    group         1803128 Jul 10 10:18 ls-lR.Z",
             "d---------   1 owner    group               0 May  9 19:45 Softlib",
             # Also WFTPD for MSDOS:
             "-rwxrwxrwx   1 noone    nogroup      322 Aug 19  1996 message.ftp",
             # Also NetWare:
             "d [R----F--] supervisor            512       Jan 16 18:53    login",
             "- [R----F--] rhesus             214059       Oct 20 15:27    cx.exe",
             # Also NetPresenz for the Mac:
             "-------r--         326  1391972  1392298 Nov 22  1995 MegaPhone.sit",
             "drwxrwxr-x               folder        2 May 10  1996 network",
             # MultiNet (some spaces removed from examples)
             "00README.TXT;1      2 30-DEC-1996 17:44 [SYSTEM] (RWED,RWED,RE,RE)",
             "CORE.DIR;1          1  8-SEP-1996 16:09 [SYSTEM] (RWE,RWE,RE,RE)",
             # and non-MutliNet VMS:
             "CII-MANUAL.TEX;1  213/216  29-JAN-1996 03:33:12  [ANONYMOU,ANONYMOUS]   (RWED,RWED,,)",
             # MSDOS format
             "04-27-00  09:09PM       <DIR>          licensed",
             "07-18-00  10:16AM       <DIR>          pub",
             "04-14-00  03:47PM                  589 readme.htm",
             # Some useless lines, safely ignored:
             "Total of 11 Files, 10966 Blocks.",
             "total 14786",
             "DISK$ANONFTP:[ANONYMOUS]",
             "Directory DISK$PCSA:[ANONYM]"]

import ftpparse

for x in ftplines:
    print ftpparse.ftpparse(x)
