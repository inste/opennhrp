/* nhrp_peer.c - NHRP peer cache implementation
 *
 * Copyright (C) 2007 Timo Teräs <timo.teras@iki.fi>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License version 3 as published
 * by the Free Software Foundation. See http://www.gnu.org/ for details.
 */

#include "nhrp_common.h"
#include "nhrp_peer.h"

static struct nhrp_peer_list peer_cache = CIRCLEQ_HEAD_INITIALIZER(peer_cache);

static void nhrp_peer_register(struct nhrp_task *task)
{
	struct nhrp_peer *peer = container_of(task, struct nhrp_peer, task);
	char dst[64];
	struct nhrp_packet r = {
		.hdr.afnum = peer->afnum,
		.hdr.protocol_type = peer->protocol_type,
		.hdr.version = NHRP_VERSION_RFC2332,
		.hdr.type = NHRP_PACKET_REGISTRATION_REQUEST,
		.hdr.flags = NHRP_FLAG_REGISTRATION_UNIQUE,
		.dst_protocol_address = peer->dst_protocol_address,
	};

	nhrp_info("Sending NHRP-Register to %s",
		nhrp_format_protocol_address(peer->protocol_type,
			&peer->dst_protocol_address, sizeof(dst), dst));

	nhrp_packet_send(&r);
	nhrp_task_schedule(&peer->task, 10000, nhrp_peer_register);
}

void nhrp_peer_insert(struct nhrp_peer *peer)
{
	char dst[64], nbma[64];

	CIRCLEQ_INSERT_HEAD(&peer_cache, peer, peer_list);

	if (peer->type == NHRP_PEER_TYPE_STATIC) {
		/* Schedule registration */
		nhrp_task_schedule(&peer->task, 5000, nhrp_peer_register);
	}

	nhrp_info("Peer %s learned at NBMA address %s",
		nhrp_format_protocol_address(peer->protocol_type,
			&peer->dst_protocol_address, sizeof(dst), dst),
		nhrp_format_nbma_address(peer->afnum, &peer->nbma_address,
			sizeof(nbma), nbma));
}

void nhrp_peer_remove(struct nhrp_peer *peer)
{
	CIRCLEQ_REMOVE(&peer_cache, peer, peer_list);
}

struct nhrp_peer *nhrp_peer_find(uint16_t protocol_type,
				 struct nhrp_protocol_address *dest,
				 int min_prefix)
{
	struct nhrp_peer *p;

	CIRCLEQ_FOREACH(p, &peer_cache, peer_list) {
	}

	return NULL;
}