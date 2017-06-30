/*
 * hostapd / ubus support
 * Copyright (c) 2013, Felix Fietkau <nbd@nbd.name>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
#ifndef __HOSTAPD_UBUS_H
#define __HOSTAPD_UBUS_H

enum hostapd_ubus_event_type {
	HOSTAPD_UBUS_PROBE_REQ,
	HOSTAPD_UBUS_AUTH_REQ,
	HOSTAPD_UBUS_ASSOC_REQ,
	HOSTAPD_UBUS_TYPE_MAX
};

struct hostapd_ubus_request {
	enum hostapd_ubus_event_type type;
	const struct ieee80211_mgmt *mgmt_frame;
	const struct hostapd_frame_info *frame_info;
	const u8 *addr;
};

struct hostapd_iface;
struct hostapd_data;

#ifdef UBUS_SUPPORT

#include <libubox/avl.h>
#include <libubus.h>

struct hostapd_ubus_iface {
	struct ubus_object obj;
};

struct hostapd_ubus_bss { /* per-BSS parameters */
	struct ubus_object obj;
	struct avl_tree banned; /* List of banned MACs */
	int	signal_auth_min;    /* Minimum signal a STA needs to authenticate */
	int signal_stay_min;    /* Minimum signal needed to stay connected. */
	int signal_poll_time;   /* Time in seconds between checks of connected STAs */
	int signal_strikes;     /* Number of consecutive times signal can be low
								before dropping the STA.  */
	int signal_drop_reason; /* IEEE reason transmitted when dropping a STA.  */
};

void hostapd_ubus_add_iface(struct hostapd_iface *iface);
void hostapd_ubus_free_iface(struct hostapd_iface *iface);
void hostapd_ubus_add_bss(struct hostapd_data *hapd);
void hostapd_ubus_free_bss(struct hostapd_data *hapd);

int hostapd_ubus_handle_event(struct hostapd_data *hapd, struct hostapd_ubus_request *req);

#else

struct hostapd_ubus_iface {};

struct hostapd_ubus_bss {};

static inline void hostapd_ubus_add_iface(struct hostapd_iface *iface)
{
}

static inline void hostapd_ubus_free_iface(struct hostapd_iface *iface)
{
}

static inline void hostapd_ubus_add_bss(struct hostapd_data *hapd)
{
}

static inline void hostapd_ubus_free_bss(struct hostapd_data *hapd)
{
}

static inline int hostapd_ubus_handle_event(struct hostapd_data *hapd, struct hostapd_ubus_request *req)
{
	return 0;
}

#endif /* UBUS_SUPPORT */

#endif
