/* This file was generated by scm2c from source file "stats.scm" */
/*  "stats.scm" WB-tree File Based Associative String Data Base System. */
/*  Copyright (C) 1991, 1992, 1993, 2000 Free Software Foundation, Inc. */
/*  */
/*  This program is free software: you can redistribute it and/or modify */
/*  it under the terms of the GNU Lesser General Public License as */
/*  published by the Free Software Foundation, either version 3 of the */
/*  License, or (at your option) any later version. */
/*  */
/*  This program is distributed in the hope that it will be useful, but */
/*  WITHOUT ANY WARRANTY; without even the implied warranty of */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/*  Lesser General Public License for more details. */
/*  */
/*  You should have received a copy of the GNU Lesser General Public */
/*  License along with this program.  If not, see */
/*  <http://www.gnu.org/licenses/>. */


#include "wbsys.h"

/*  PERFORMANCE STATISTICS */


long next_ct = 0x0L;

long next_fct = 0x0L;

long prev_ct = 0x0L;

long prev_fct = 0x0L;

long get_ct = 0x0L;

long get_fct = 0x0L;

long put_ct = 0x0L;

long put_fct = 0x0L;

long rem_ct = 0x0L;

long rem_fct = 0x0L;

long ge_ct = 0x0L;

long ge_fct = 0x0L;

long tge_ct = 0x0L;

long tge_fct = 0x0L;

long tce_ct = 0x0L;

long tce_fct = 0x0L;


int chains_to_next = 0;

int deferred_inserts = 0;

int split_index_inserts = 0;

int index_screw_case = 0;

int block_splits = 0;

int block_deletes = 0;

int deferred_deletes = 0;

long dir_dty_ct = 0x0L;


long read_ct = 0x0L;

long write_ct = 0x0L;

long read_fl_ct = 0x0L;

long write_fl_ct = 0x0L;

long flush_ct = 0x0L;


int clear_stats(void)
{
  next_ct = 0x0L;
  next_fct = 0x0L;
  prev_ct = 0x0L;
  prev_fct = 0x0L;
  get_ct = 0x0L;
  get_fct = 0x0L;
  put_ct = 0x0L;
  put_fct = 0x0L;
  rem_ct = 0x0L;
  rem_fct = 0x0L;
  ge_ct = 0x0L;
  ge_fct = 0x0L;
  tge_ct = 0x0L;
  tge_fct = 0x0L;
  tce_ct = 0x0L;
  tce_fct = 0x0L;
  chains_to_next = 0;
  deferred_inserts = 0;
  split_index_inserts = 0;
  index_screw_case = 0;
  block_splits = 0;
  block_deletes = 0;
  deferred_deletes = 0;
  dir_dty_ct = 0x0L;
  read_ct = 0x0L;
  write_ct = 0x0L;
  read_fl_ct = 0x0L;
  write_fl_ct = 0x0L;
  flush_ct = 0x0L;
  return success;
}


int cstats(void)
{
  stats();
  return clear_stats();
}


int stats(void)
{
  dprintf((diagout, "\n"));
  dprintf((diagout, "OPERATIONS SUMMARY:\n"));
  dprintf((diagout,
	 "\
            NEXT     PREV      GET      PUT      REM   GETENT   CACHE    CACHE\n"));
  dprintf((diagout,
	 "\
                                                                AVAIL?   VALID?\n"));
  dprintf((diagout,
	 "   succ %8lu %8lu %8lu %8lu %8lu %8lu %8lu %8lu\n",
	 next_ct,
	 prev_ct,
	 get_ct,
	 put_ct,
	 rem_ct,
	 ge_ct,
	 tge_ct,
	 tce_ct));
  dprintf((diagout,
	 "   fail %8lu %8lu %8lu %8lu %8lu %8lu %8lu %8lu\n\n",
	 next_fct,
	 prev_fct,
	 get_fct,
	 put_fct,
	 rem_fct,
	 ge_fct,
	 tge_fct,
	 tce_fct));
  dprintf((diagout, "INTERNAL OPERATIONS SUMMARY:\n"));
  dprintf((diagout, "   chains-to-next = %d\n", chains_to_next));
  dprintf((diagout,
	 "   %d block splits, %d deferred parent updates, %d undone\n",
	 block_splits,
	 deferred_inserts,
	 deferred_inserts));
  dprintf((diagout,
	 "   %d split index inserts; %d index-insert screw cases \n",
	 split_index_inserts,
	 index_screw_case));
  dprintf((diagout,
	 "   %d block deletes; %d deferred block deletes\n\n",
	 block_deletes,
	 deferred_deletes));
  dprintf((diagout,
	 "\
I/O SUMMARY: %lu READS, %lu WRITES, %lu FLUSH (of dirty bufs) calls.\n",
	 read_ct,
	 write_ct,
	 flush_ct));
  dprintf((diagout,
	 "             %lu FREELIST READS, %lu FREELIST WRITES.\n",
	 read_fl_ct,
	 write_fl_ct));
  dprintf((diagout, "             %lu DIRS LEFT DTY.\n", dir_dty_ct));
  {
    int ops = max((next_fct)+(prev_fct)+(get_fct)+(put_fct)+(rem_fct)+(next_ct)+(prev_ct)+(get_ct)
     +(put_ct)+(rem_ct), 1);
    dprintf((diagout,
	 "   AVG BLKS REFERENCED PER OPERATION: %7lu%%\n",
	 (0x64*((ge_ct)+(tce_ct)))/(ops)));
    dprintf((diagout, "   AVG DISK I/Os PER OPERATION: %7lu%%; ", (0x64*((read_ct)+(write_ct)+(read_fl_ct)+(write_fl_ct)))/(ops)));
    dprintf((diagout, "   WRITE/READ RATIO: %7lu%%\n", (0x64*((write_ct)+(write_fl_ct)))/(max(1, (read_ct)+(read_fl_ct)))));
    if ((put_ct)>0)
      dprintf((diagout, "   AVG DISK WRITES PER PUT: %7lu%%\n", (0x64*(write_ct))/(put_ct)));
  }
  dprintf((diagout, "\n"));
  dprintf((diagout, "MODES IN EFFECT:\n"));
  dprintf((diagout,
	 "\
%lu buffers [hashed over %d buckets]; blksize (max)=%d; FLC-LEN=%d\n",
	 num_ents_ct,
	 num_buks,
	 blk_size,
	 flc_len));
  dprintf((diagout,
	 "defer-insert-updates?= %2s; defer-block-deletes=%2s\n",
	 
	   (defer_insert_updates_P
	     ?"#t"
	     :"#f"),
	 
	   (del_defer_block_deletes_P
	     ?"#t"
	     :"#f")));
  dprintf((diagout,
	 "cache-ent-enable?=%2s; clever-cache-enable?=%2s\n",
	 
	   (cache_ent_enable_P
	     ?"#t"
	     :"#f"),
	 
	   (clever_cache_enable_P
	     ?"#t"
	     :"#f")));
  dprintf((diagout, "\n"));
  return success;
}


void show_buffer(ENTRY *ent)
{
  dprintf((diagout,
	 "SEG = %ld ID= %lu ACC= %d REF= %d DTY= %d AGE= %d\n",
	 seg_id(ent_seg(ent)),
	 ent_id(ent),
	 ent_acc(ent),
	 ent_ref(ent),
	 ent_dty_P(ent),
	 ent_age(ent)));
  return;
}


int buf_verbose_P = !0;


int show_buffer_1(ENTRY *ent)
{
  if ((buf_verbose_P)
      || ((seg_id(ent_seg(ent)))> -1))
    {
      dprintf((diagout, " %3ld:%-6lu %6lu %8d %6u %7d %3d %4d", seg_id(ent_seg(ent)), ent_id(ent), hash2int(seg_id(ent_seg(ent)), ent_id(ent)), ent_acc(ent), ent_ref(ent), 0, ent_dty_P(ent), ent_age(ent)));
      if ((seg_id(ent_seg(ent)))> -1)
	{
	  dprintf((diagout, " %5d %4c\n", (blk_level(ent_blk(ent)))-(leaf), blk_typ(ent_blk(ent))));
	}
      else dprintf((diagout, "\n"));
    }
  return success;
}


int show_buffers(void)
{
  dprintf((diagout, "\n"));
  dprintf((diagout,
	 "\
 SEG:ID        BUK      ACC    REF READERS DTY  AGE LEVEL TYPE\n"));
  return do_seg_buffers(0, show_buffer_1);
}


int sb(void)
{
  return show_buffers();
}
