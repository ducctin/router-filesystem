/*
 *	Generic parts
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/llc.h>
#include <net/llc.h>
#include <net/stp.h>

#include "br_private.h"

int (*br_should_route_hook)(struct sk_buff *skb);

static const struct stp_proto br_stp_proto = {
	.rcv	= br_stp_rcv,
};

static struct pernet_operations br_net_ops = {
	.exit	= br_net_exit,
};

static int __init br_init(void)
{
	int err;

	err = stp_proto_register(&br_stp_proto);
	if (err < 0) {
		pr_err("bridge: can't register sap for STP\n");
		return err;
	}

	err = br_fdb_init();
	if (err)
		goto err_out;

	err = register_pernet_subsys(&br_net_ops);
	if (err)
		goto err_out1;

	err = br_netfilter_init();
	if (err)
		goto err_out2;

	err = register_netdevice_notifier(&br_device_notifier);
	if (err)
		goto err_out3;

	err = br_netlink_init();
	if (err)
		goto err_out4;

#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1)
	err = br_pppcheck_init();
	if (err)
		goto err_out5;
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	err = br_dhcpCheck_init();
	if (err)
		goto err_out6;
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
	err = br_dataspeed_limit_init();
	if ( err )
		goto err_out7;
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	err = br_pppbridge_state_init();
	if ( err )
		goto err_out8;
#endif

	brioctl_set(br_ioctl_deviceless_stub);

#if defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE)
	br_fdb_test_addr_hook = br_fdb_test_addr;
#endif

	return 0;
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
err_out8:
	br_pppbridge_state_fini();
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
err_out7:
	br_dataspeed_limit_fini();
#endif
#if defined(TCSUPPORT_CT_PON_SC)
err_out6:
	br_dhcpCheck_fini();
#endif
#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1)
err_out5:
	br_pppcheck_fini();
#endif
err_out4:
	unregister_netdevice_notifier(&br_device_notifier);
err_out3:
	br_netfilter_fini();
err_out2:
	unregister_pernet_subsys(&br_net_ops);
err_out1:
	br_fdb_fini();
err_out:
	stp_proto_unregister(&br_stp_proto);
	return err;
}

static void __exit br_deinit(void)
{
	stp_proto_unregister(&br_stp_proto);

	br_netlink_fini();
	unregister_netdevice_notifier(&br_device_notifier);
	brioctl_set(NULL);

	unregister_pernet_subsys(&br_net_ops);

	rcu_barrier(); /* Wait for completion of call_rcu()'s */

	br_netfilter_fini();
#if defined(TCSUPPORT_CT_PPPINFORM) || defined(TCSUPPORT_CT_AUTOREGISTER) || defined(TCSUPPORT_CT_ADSL_BIND1)
	br_pppcheck_fini();
#endif
#if defined(TCSUPPORT_CT_PON_SC)
	br_dhcpCheck_fini();
#endif
#if defined(TCSUPPORT_CT_DS_LIMIT)
	br_dataspeed_limit_fini();
#endif
#if defined(TCSUPPORT_CT_BRIDGE_PPPSTATUS)
	br_pppbridge_state_fini();
#endif
#if defined(CONFIG_ATM_LANE) || defined(CONFIG_ATM_LANE_MODULE)
	br_fdb_test_addr_hook = NULL;
#endif

	br_fdb_fini();
}

EXPORT_SYMBOL(br_should_route_hook);

module_init(br_init)
module_exit(br_deinit)
MODULE_LICENSE("GPL");
MODULE_VERSION(BR_VERSION);
