/*
 Copyright (c) 2013, Tom van der Woerdt
 */

#pragma once
#include <stdio.h>
#include <event2/event.h>
#include "btcblock.h"
#include "config.h"


struct bp_blockstorage_hashnode_t {
	int num;
	unsigned int checksum;
	struct bp_blockstorage_hashnode_t *next;
};
typedef struct bp_blockstorage_hashnode_t bp_blockstorage_hashnode_s;


struct bp_blockstorage_hashmap_t {
	unsigned int size;
	bp_blockstorage_hashnode_s **nodes;
};
typedef struct bp_blockstorage_hashmap_t bp_blockstorage_hashmap_s;


struct bp_blockstorage_fd_t {
#ifdef HAVE_FILE_SEGMENTS
	struct evbuffer_file_segment *fd;
#else
	int fd;
#endif
	unsigned int offset;
	unsigned int size;
	unsigned int checksum;
};
typedef struct bp_blockstorage_fd_t bp_blockstorage_fd_s;


struct bp_blockstorage_t {
	void *program;
	
	char *mainchain;
	char *orphans;
	bp_blockstorage_hashmap_s *mainindex;
	bp_blockstorage_hashmap_s *orphanindex;
	
	FILE* mainchain_fd;
	FILE* orphans_fd;
	unsigned int mainchain_allocsize;
	unsigned int mainchain_writepos;
	unsigned int orphans_allocsize;
	unsigned int orphans_writepos;
	
	FILE *currentblock_fd;
	unsigned int currentblock_num;
	unsigned int currentblock_offset;
	unsigned do_rechain: 1;

#ifdef HAVE_FILE_SEGMENTS
	struct evbuffer_file_segment **block_fds;
#endif
};
typedef struct bp_blockstorage_t bp_blockstorage_s;


int bp_blockstorage_init(bp_blockstorage_s *storage, void *program);
void bp_blockstorage_deinit(bp_blockstorage_s *storage);

int bp_blockstorage_store(bp_blockstorage_s *storage, char *blockhash, bp_btcblock_header_s *header, char *txs, size_t txs_len);
int bp_blockstorage_getfd(bp_blockstorage_s *storage, char *blockhash, bp_blockstorage_fd_s *fd);
int bp_blockstorage_index(bp_blockstorage_s *storage, char *blockhash, char *prevblock, unsigned int blockfile, unsigned int offset, unsigned int length, unsigned int checksum);
int bp_blockstorage_hasblock(bp_blockstorage_s *storage, char *blockhash);
int bp_blockstorage_reindex(bp_blockstorage_s *storage);
int bp_blockstorage_rehash(bp_blockstorage_s *storage);
char *bp_blockstorage_gettop(bp_blockstorage_s *storage);
unsigned int bp_blockstorage_getheight(bp_blockstorage_s *storage);
char *bp_blockstorage_getatindex(bp_blockstorage_s *storage, unsigned int num);
int bp_blockstorage_getnum(bp_blockstorage_s *storage, char *blockhash);
int bp_blockstorage_nextblock(bp_blockstorage_s *storage);
int bp_blockstorage_store_genesis(bp_blockstorage_s *storage);

bp_blockstorage_hashmap_s *bp_blockstorage_hashmap_new(unsigned int size);
int bp_blockstorage_hashmap_insert(bp_blockstorage_hashmap_s *map, char *blockhash, unsigned int num);
bp_blockstorage_hashnode_s *bp_blockstorage_hashmap_getnode(bp_blockstorage_hashmap_s *map, char *blockhash);
void bp_blockstorage_hashmap_free(bp_blockstorage_hashmap_s *map);

// Rechaining
int bp_blockstorage_rechain_main(bp_blockstorage_s *storage);
int bp_blockstorage_rechain_orphan(bp_blockstorage_s *storage);
int bp_blockstorage_deindex(bp_blockstorage_s *storage, int chain, int num);
