"$Id: test_ftpparse.py,v 1.2 2002/01/02 23:01:00 drt Exp $"

# Tests for ftpparse module

# drt@un.bewaff.net - http://c0re.jp/c0de/ftpparse/

import sys
sys.path.append('..')

import time
import unittest
import ftpparse
import gc

ftplines = [ \
            # ELPF - see http://pobox.com/~djb/proto/eplf.txt
            ('+i8388621.29609,m824255902,/,\tdev',
             [('dev', 0, 0, 824255902, 1, 1, 0, '8388621.29609', 1)]),
            ('+i8388621.44468,m839956783,r,s10376,\tRFCEPLF',
             [('RFCEPLF', 10376, 1, 839956783, 1, 0, 1, '8388621.44468', 1)]),
            # UNIX-style listing, without inum and without blocks
            # UNIX ls does not show the year for dates in the last six months.
            # So ftpparse has to guess the year. This leds to non-constant
            # results so I exclude this tests for now. 
            #('-rw-r--r--   1 root     other        531 Jan 29 03:26 README',
            # [('README', 531, 1, 980738760, 2, 0, 1, None, 0)]),
            ('dr-xr-xr-x   2 root     other        512 Apr  8  1994 etc',
             [('etc', 512, 1, 765763200, 3, 1, 0, None, 0)]),
            ('dr-xr-xr-x   2 root     512 Apr  8  1994 etc',
             [('etc', 512, 1, 765763200, 3, 1, 0, None, 0)]),
            #('lrwxrwxrwx   1 root     other          7 Jan 25 00:17 bin -> usr/bin',
            # [('bin', 7, 1, 980381820, 2, 1, 1, None, 0)]),
            #('-rw-r-----  1 md  staff  13500 Dec 25 08:44 ftpparse.c',
            # [('ftpparse.c', 13500, 1, 1009269840, 2, 0, 1, None, 0)]),
            # Also produced by Microsoft's FTP servers for Windows:
            #('----------   1 owner    group         1803128 Jul 10 10:18 ls-lR.Z',
            # [('ls-lR.Z', 1803128, 1, 994760280, 2, 0, 1, None, 0)]),
            #('d---------   1 owner    group               0 May  9 19:45 Softlib',
            # [('Softlib', 0, 1, 989437500, 2, 1, 0, None, 0)]),
            # Also WFTPD for MSDOS:
            ('-rwxrwxrwx   1 noone    nogroup      322 Aug 19  1996 message.ftp',
             [('message.ftp', 322, 1, 840412800, 3, 0, 1, None, 0)]),
            # Also NetWare:
            #('d [R----F--] supervisor            512       Jan 16 18:53    login',
            # [('login', 512, 1, 979671180, 2, 1, 0, None, 0)]),
            #('- [R----F--] rhesus             214059       Oct 20 15:27    cx.exe',
            # [('cx.exe', 214059, 1, 1003591620, 2, 0, 1, None, 0)]),
            # Also NetPresenz for the Mac:
            ('-------r--         326  1391972  1392298 Nov 22  1995 MegaPhone.sit',
             [('MegaPhone.sit', 1392298, 1, 816998400, 3, 0, 1, None, 0)]),
            ('drwxrwxr-x               folder        2 May 10  1996 network',
             [('network', 2, 1, 831686400, 3, 1, 0, None, 0)]),
            # MultiNet (some spaces removed from examples)
            ('00README.TXT;1      2 30-DEC-1996 17:44 [SYSTEM] (RWED,RWED,RE,RE)',
             [('00README.TXT', 0, 0, 851967840, 2, 0, 1, None, 0)]),
            ('CORE.DIR;1          1  8-SEP-1996 16:09 [SYSTEM] (RWE,RWE,RE,RE)',
             [('CORE', 0, 0, 842198940, 2, 1, 0, None, 0)]),
            # and non-MutliNet VMS:
            ('CII-MANUAL.TEX;1  213/216  29-JAN-1996 03:33:12  [ANONYMOU,ANONYMOUS]   (RWED,RWED,,)',
             [('CII-MANUAL.TEX', 0, 0, 822886380, 2, 0, 1, None, 0)]),
            # MSDOS format
            ('04-27-00  09:09PM       <DIR>          licensed',
             [('licensed', 0, 0, 956869740, 2, 1, 0, None, 0)]),
            ('07-18-00  10:16AM       <DIR>          pub',
             [('pub', 0, 0, 963915360, 2, 1, 0, None, 0)]),
            ('04-14-00  03:47PM                  589 readme.htm',
             [('readme.htm', 589, 1, 955727220, 2, 0, 1, None, 0)]),
            # Some useless lines, safely ignored:
            ('Total of 11 Files, 10966 Blocks.',
             [None]),
            ('total 14786',
             [None]),
            ('DISK$ANONFTP:[ANONYMOUS]',
             [None]),
            ('Directory DISK$PCSA:[ANONYM]',
             [None])]

class SimpleCheck(unittest.TestCase):
    def testKnownValues(self):
        """Known Questions shoulsd result in known aswers."""
        for (question, answer) in ftplines:
            result = ftpparse.ftpparse([question])
            self.assertEqual(answer, result)

    def testTypeErrors(self):
         self.assertRaises(TypeError, ftpparse.ftpparse, 'string')
         self.assertRaises(TypeError, ftpparse.ftpparse, 1)
         self.assertRaises(TypeError, ftpparse.ftpparse, [1])

    def testMemLeak(self):
        question = ["-rw-r--r--  1 md  staff  13500 Dec 25 08:44 ftpparse.c",
                    "-rw-r--r--  1 md  staff   1719 Dec 25 08:44 ftpparse.h"]
        gc.set_debug(gc.DEBUG_LEAK)
        for i in range(2**16):
            answer = ftpparse.ftpparse(question) 
        # this once produced a huge output
        gc.collect()
        
if __name__ == "__main__":
    unittest.main()
