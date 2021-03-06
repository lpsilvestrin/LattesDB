/* This file was generated by scm2c from source file "del.scm" */
/*  "del.scm" WB-tree File Based Associative String Data Base System. */
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


int del_defer_block_deletes_P = 0;

/*  fixes: */
/*  1. 1/22 blk-delete should not be called if END-OF-CHAIN */
/*  2.      IND-REM-V&K needed to return B-POS */
/*  3.      CHAIN-KEY-REM also neede to check for being already at root level */
/*  4. 1/23 fixed BLK-DELETE? to set accmode to ACCNONE while calling PREV-BLK-ENT! */
/*  5.      fixed CHAIN-KEY-REM to give error message if key not found in index */

/*  BLK-DELETE assumes caller has ACCWRITE to blk and will */
/*  release if after blk-delete returns */

/*  sorry, waiting on parent-update is losing since */
/*  deletes that  lock the entire path to the root will almost certainly */
/*  NEVER succeed! */


int del_delete_bck(ENTRY *ent)
{
  unsigned char *blk = ent_blk(ent);
  int win_P = !(del_defer_block_deletes_P);
  if (win_P)
    {
      ents_ent_update_access_P(ent, accwrite, accnone);
      {
	ENTRY *prent = prev_blk_ent(ent, blk_level(blk));
	win_P = ents_ent_update_access_P(ent, accnone, accwrite);
	if ((win_P)
	    && (prent))
	  win_P = ents_ent_update_access_P(prent, accread, accwrite);
	win_P = (win_P)
        && (1==(ent_ref(ent)));
	if (win_P)
	  {
	    if (!(at_root_level_P(ent_seg(ent), blk)))
	      {
		int skey_pos = split_key_pos(blk);
		if (0 < (skey_pos))
		  {
		    long top_num = blk_top_id(blk);
		    SEGD *seg = ent_seg(ent);
		    int level = blk_level(blk);
		    unsigned char key_str[0x100];
		    int k_len = recon_this_key(blk, skey_pos, key_str, 0, 0x100);
		    win_P = parent_delete_update_P(seg, top_num, level, ent_id(ent), key_str, k_len);
		  }
	      }
	    win_P = (win_P)
	    && (1==(ent_ref(ent)));
	    if (win_P)
	      {
		if (prent)
		  {
		    blk_set_nxt_id(ent_blk(prent), blk_nxt_id(blk));
		    ent_set_dty(prent, !0);
		    ents_ent_write(prent);
		  }
		win_P = blk_free(ent);
		if (!(win_P))
		  dprintf((diagout,
	 ">>>>ERROR<<<< %s: could not free %ld:%ld\n",
	 "blk_delete",
	 seg_id(ent_seg(ent)),
	 ent_id(ent)));
	      }
	  }
	if (prent)
	  release_ent(prent, ent_acc(prent));
      }
    }
  if (win_P)
    {
      block_deletes = (block_deletes)+1;
    }
  else {
    deferred_deletes = 1+(deferred_deletes);
    dprintf((diagout, "Can't delete block %lu\n", ent_id(ent)));
  }
  return win_P;
}

/*  return #t if operation was succsessful; #f if not */
/*  Note the deletion of blk OLD-ID by removing its KEY+ID from parent. */
/*  Note this routine does not check if the key has already been */
/*  (perhaps by another process) deleted from the parent. */


int parent_delete_update_P(SEGD *seg,long top_id,int level,long old_id,unsigned char *key_str,int k_len)
{
  int pkt[pkt_size];
  int ans =  -1;
  unsigned char ans_str[4];
  {
    ENTRY *ent = find_ent(get_ent(seg, top_id, accnone), 1+(level),  -1, key_str, k_len);
    if (!(ent))
      ;
    else if (ents_ent_update_access_P(ent, accread, accwrite))
      {
	ent = chain_find(ent, accwrite, key_str, k_len, pkt);
      }
    else {
      release_ent(ent, accread);
      ent = 0;
    }
    if (ent)
      {
	ans = del_chain_rem(ent, key_str, k_len, ans_str, pkt, wcb_sar);
	if ((ans)>=0)
	  if ((old_id)!=(str2long(ans_str, 0)))
	    dprintf((diagout,
	 "\
>>>>ERROR<<<< %s: bad value %lu in deleted down pointer %lu told\n",
	 "parent_delete_update_P",
	 str2long(ans_str, 0),
	 old_id));
	release_ent(ent, accwrite);
      }
    if ((ent)
        || ((ans)>=0))
      return !0;
    else {
      dprintf((diagout,
	 "WARNING: %s blk=%ld:%ld, level=%d, key=%.*s\n",
	 "parent_delete_update_P",
	 seg_id(seg),
	 old_id,
	 level,
	 k_len,
	 key_str));
      return 0;
    }
  }
}

/*  called with ACCREAD on ENT, releases ent before returning */
/*  DEL:CHAIN-REM can call BLK-DELETE */
/*    BLK-DELETE calls BLK-FREE */
/*      BLK-FREE calls AMNESIA-ENT! which sets the segment number to -1 */
/*  DEL:CHAIN-REM calls RELEASE-ENT! */
/*  Chad Gadya! */


int del_chain_rem(ENTRY *ent,unsigned char *key_str,int k_len,unsigned char *ans_str,int *pkt,int wcb)
{
  if ((pkt_match_type(pkt))==(match))
    {
      int alen = success;
      if (ans_str)
	alen = get_this_val(ent_blk(ent), pkt_match_pos(pkt), ans_str);
      blk_remove_key_and_val(ent_blk(ent), pkt_match_pos(pkt), seg_bsiz(ent_seg(ent)));
      ent_set_dty(ent, !0);
      if ((blk_empty_P(ent_blk(ent)))
	  && (!(end_of_chain_P(ent_blk(ent)))))
	{
	  del_delete_bck(ent);
	}
      else if (((wcb_sar)&(wcb))
	  || ((blk_level(ent_blk(ent)))>(leaf)))
	ents_ent_write(ent);
      return alen;
    }
  else return notpres;
}
