
extern void libxt_CHECKSUM_init(void);
extern void libxt_CLASSIFY_init(void);
extern void libxt_cluster_init(void);
extern void libxt_comment_init(void);
extern void libxt_connbytes_init(void);
extern void libxt_connlimit_init(void);
extern void libxt_connmark_init(void);
extern void libxt_CONNMARK_init(void);
extern void libxt_CONNSECMARK_init(void);
extern void libxt_conntrack_init(void);
extern void libxt_cpu_init(void);
extern void libxt_CT_init(void);
extern void libxt_dccp_init(void);
extern void libxt_dscp_init(void);
extern void libxt_DSCP_init(void);
extern void libxt_esp_init(void);
extern void libxt_hashlimit_init(void);
extern void libxt_helper_init(void);
extern void libxt_IDLETIMER_init(void);
extern void libxt_iprange_init(void);
extern void libxt_ipvs_init(void);
extern void libxt_layer7_init(void);
extern void libxt_LED_init(void);
extern void libxt_length_init(void);
extern void libxt_limit_init(void);
extern void libxt_mac_init(void);
extern void libxt_mark_init(void);
extern void libxt_MARK_init(void);
extern void libxt_multiport_init(void);
extern void libxt_NFLOG_init(void);
extern void libxt_NFQUEUE_init(void);
extern void libxt_NOTRACK_init(void);
extern void libxt_osf_init(void);
extern void libxt_owner_init(void);
extern void libxt_physdev_init(void);
extern void libxt_pkttype_init(void);
extern void libxt_policy_init(void);
extern void libxt_quota_init(void);
extern void libxt_rateest_init(void);
extern void libxt_RATEEST_init(void);
extern void libxt_recent_init(void);
extern void libxt_sctp_init(void);
extern void libxt_SECMARK_init(void);
extern void libxt_set_init(void);
extern void libxt_SET_init(void);
extern void libxt_socket_init(void);
extern void libxt_standard_init(void);
extern void libxt_state_init(void);
extern void libxt_statistic_init(void);
extern void libxt_string_init(void);
extern void libxt_tcp_init(void);
extern void libxt_tcpmss_init(void);
extern void libxt_TCPMSS_init(void);
extern void libxt_TCPOPTSTRIP_init(void);
extern void libxt_TEE_init(void);
extern void libxt_time_init(void);
extern void libxt_tos_init(void);
extern void libxt_TOS_init(void);
extern void libxt_TPROXY_init(void);
extern void libxt_TRACE_init(void);
extern void libxt_u32_init(void);
extern void libxt_udp_init(void);
extern void libipt_addrtype_init(void);
extern void libipt_ah_init(void);
extern void libipt_CLUSTERIP_init(void);
extern void libipt_DNAT_init(void);
extern void libipt_ecn_init(void);
extern void libipt_ECN_init(void);
extern void libipt_icmp_init(void);
extern void libipt_LOG_init(void);
extern void libipt_MASQUERADE_init(void);
extern void libipt_MIRROR_init(void);
extern void libipt_NETMAP_init(void);
extern void libipt_realm_init(void);
extern void libipt_REDIRECT_init(void);
extern void libipt_REJECT_init(void);
extern void libipt_SAME_init(void);
extern void libipt_SNAT_init(void);
extern void libipt_TRIGGER_init(void);
extern void libipt_ttl_init(void);
extern void libipt_TTL_init(void);
extern void libipt_ULOG_init(void);
extern void libipt_unclean_init(void);
void init_extensions(void);
void init_extensions(void)
{
 libxt_CHECKSUM_init();
 libxt_CLASSIFY_init();
 libxt_cluster_init();
 libxt_comment_init();
 libxt_connbytes_init();
 libxt_connlimit_init();
 libxt_connmark_init();
 libxt_CONNMARK_init();
 libxt_CONNSECMARK_init();
 libxt_conntrack_init();
 libxt_cpu_init();
 libxt_CT_init();
 libxt_dccp_init();
 libxt_dscp_init();
 libxt_DSCP_init();
 libxt_esp_init();
 libxt_hashlimit_init();
 libxt_helper_init();
 libxt_IDLETIMER_init();
 libxt_iprange_init();
 libxt_ipvs_init();
 libxt_layer7_init();
 libxt_LED_init();
 libxt_length_init();
 libxt_limit_init();
 libxt_mac_init();
 libxt_mark_init();
 libxt_MARK_init();
 libxt_multiport_init();
 libxt_NFLOG_init();
 libxt_NFQUEUE_init();
 libxt_NOTRACK_init();
 libxt_osf_init();
 libxt_owner_init();
 libxt_physdev_init();
 libxt_pkttype_init();
 libxt_policy_init();
 libxt_quota_init();
 libxt_rateest_init();
 libxt_RATEEST_init();
 libxt_recent_init();
 libxt_sctp_init();
 libxt_SECMARK_init();
 libxt_set_init();
 libxt_SET_init();
 libxt_socket_init();
 libxt_standard_init();
 libxt_state_init();
 libxt_statistic_init();
 libxt_string_init();
 libxt_tcp_init();
 libxt_tcpmss_init();
 libxt_TCPMSS_init();
 libxt_TCPOPTSTRIP_init();
 libxt_TEE_init();
 libxt_time_init();
 libxt_tos_init();
 libxt_TOS_init();
 libxt_TPROXY_init();
 libxt_TRACE_init();
 libxt_u32_init();
 libxt_udp_init();
 libipt_addrtype_init();
 libipt_ah_init();
 libipt_CLUSTERIP_init();
 libipt_DNAT_init();
 libipt_ecn_init();
 libipt_ECN_init();
 libipt_icmp_init();
 libipt_LOG_init();
 libipt_MASQUERADE_init();
 libipt_MIRROR_init();
 libipt_NETMAP_init();
 libipt_realm_init();
 libipt_REDIRECT_init();
 libipt_REJECT_init();
 libipt_SAME_init();
 libipt_SNAT_init();
 libipt_TRIGGER_init();
 libipt_ttl_init();
 libipt_TTL_init();
 libipt_ULOG_init();
 libipt_unclean_init();
}
