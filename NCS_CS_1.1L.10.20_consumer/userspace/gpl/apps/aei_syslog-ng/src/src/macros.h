/***************************************************************************
 *
 * Copyright (c) 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Inspired by nsyslog, originally written by Darren Reed.
 *
 * $Id: macros.h,v 1.2.4.3 2006/05/26 19:21:13 bazsi Exp $
 *
 ***************************************************************************/

#ifndef __MACROS_H
#define __MACROS_H

#define M_FACILITY     0
#define M_FACILITY_NUM 1
#define M_LEVEL        10
#define M_LEVEL_NUM    11
#define M_TAG          20
#define M_PRI          21

#define M_DATE     30
#define M_FULLDATE 40
#define M_ISODATE  50
#define M_YEAR     60
#define M_MONTH    70
#define M_DAY      80
#define M_HOUR     90
#define M_MIN      100
#define M_SEC      110
#define M_WEEKDAY  120
#define M_TZOFFSET 130
#define M_TZ       140
#define M_UNIXTIME 150

#define M_DATE_RECVD     160
#define M_FULLDATE_RECVD 170
#define M_ISODATE_RECVD  180
#define M_YEAR_RECVD     190
#define M_MONTH_RECVD    200
#define M_DAY_RECVD      210
#define M_HOUR_RECVD     220
#define M_MIN_RECVD      230
#define M_SEC_RECVD      240
#define M_WEEKDAY_RECVD  250
#define M_TZOFFSET_RECVD 260
#define M_TZ_RECVD       270
#define M_UNIXTIME_RECVD 280

#define M_DATE_STAMP     300
#define M_FULLDATE_STAMP 310
#define M_ISODATE_STAMP  320
#define M_YEAR_STAMP     330
#define M_MONTH_STAMP    340
#define M_DAY_STAMP      350
#define M_HOUR_STAMP     360
#define M_MIN_STAMP      370
#define M_SEC_STAMP      380
#define M_WEEKDAY_STAMP  390
#define M_TZOFFSET_STAMP 400
#define M_TZ_STAMP       410
#define M_UNIXTIME_STAMP 420

#define M_FULLHOST       430
#define M_HOST           440
#define M_FULLHOST_FROM  450
#define M_HOST_FROM      460
#define M_PROGRAM        470

#define M_MESSAGE        480
#define M_MSGONLY	 490

#define M_SOURCE_IP      500

struct ol_string *
expand_macros(struct syslog_config *cfg, struct ol_string *template, int template_escape, struct log_info *msg);

#endif
