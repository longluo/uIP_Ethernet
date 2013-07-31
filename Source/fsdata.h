/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: fsdata.h,v 1.4.2.1 2003/10/04 22:54:06 adam Exp $
 */

/*       ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
         บ    TITLE:  Keil C51 v7.00 port of Adam Dunkels' uIP v0.9 บ
         บ REVISION:  VER 0.0                                       บ
         บ REV.DATE:  14-01-05                                      บ
         บ  ARCHIVE:                                                บ
         บ   AUTHOR:  Murray R. Van Luyn, 2005.                     บ
         ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ        */

#ifndef __FSDATA_H__
#define __FSDATA_H__

#include "uipopt.h"

struct fsdata_file {
  const struct fsdata_file *next;
  const u8_t *name;
  const u8_t *dat;
  const u16_t len;
};

struct fsdata_file_noconst {
  struct fsdata_file *next;
  u8_t *name;
  u8_t *dat;
  u16_t len;
};

#define FS_ROOT file_tcp_header_html

#define FS_NUMFILES 14

const char code data_cgi_files[];
const char code data_cgi_stats[];
const char code data_cgi_tcp[];
const char code data_img_bg_png[];
const char code data_about_html[];
const char code data_control_html[];
const char code data_404_html[];
const char code data_files_footer_plain[];
const char code data_files_header_html[];
const char code data_index_html[];
const char code data_stats_footer_plain[];
const char code data_stats_header_html[];
const char code data_tcp_footer_plain[];
const char code data_tcp_header_html[];
const struct fsdata_file code file_cgi_files[];
const struct fsdata_file code file_cgi_stats[];
const struct fsdata_file code file_cgi_tcp[];
const struct fsdata_file code file_img_bg_png[];
const struct fsdata_file code file_about_html[];
const struct fsdata_file code file_control_html[];
const struct fsdata_file code file_404_html[];
const struct fsdata_file code file_files_footer_plain[];
const struct fsdata_file code file_files_header_html[];
const struct fsdata_file code file_index_html[];
const struct fsdata_file code file_setok_html[];

const struct fsdata_file code file_stats_footer_plain[];
const struct fsdata_file code file_stats_header_html[];
const struct fsdata_file code file_tcp_footer_plain[];
const struct fsdata_file code file_tcp_header_html[];

#endif /* __FSDATA_H__ */
