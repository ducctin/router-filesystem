/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.17 $ of : mfd-data-access.m2c,v $
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/if-mib/ifTable/ifTable_data_access.h#1 $
 */
#ifndef IFTABLE_DATA_ACCESS_H
#define IFTABLE_DATA_ACCESS_H

#ifdef __cplusplus
extern          "C" {
#endif


    /*
     *********************************************************************
     * function declarations
     */

    /*
     *********************************************************************
     * Table declarations
     */
/**********************************************************************
 **********************************************************************
 ***
 *** Table ifTable
 ***
 **********************************************************************
 **********************************************************************/
    /*
     * IF-MIB::ifTable is subid 2 of interfaces.
     * Its status is Current.
     * OID: .1.3.6.1.2.1.2.2, length: 8
     */


    int             ifTable_init_data(ifTable_registration * ifTable_reg);


    /*
     * TODO:180:o: Review ifTable cache timeout.
     * The number of seconds before the cache times out
     */
    /*
     * A 10 Mbps stream can wrap if*Octets in ~57 minutes.
     * At 100 Mbps it is ~5 minutes, and at 1 Gbps, ~34 seconds.
     */
#define IFTABLE_CACHE_TIMEOUT   30

    void            ifTable_container_init(netsnmp_container
                                           **container_ptr_ptr,
                                           netsnmp_cache * cache);
    void            ifTable_container_shutdown(netsnmp_container
                                               *container_ptr);

    int             ifTable_container_load(netsnmp_container *container);
    void            ifTable_container_free(netsnmp_container *container);

    void            ifTable_container_shutdown(netsnmp_container
                                               *container_ptr);

    int             ifTable_container_load(netsnmp_container *container);
    void            ifTable_container_free(netsnmp_container *container);

    void            ifTable_container_shutdown(netsnmp_container
                                               *container_ptr);

    int             ifTable_container_load(netsnmp_container *container);
    void            ifTable_container_free(netsnmp_container *container);

    int             ifTable_cache_load(netsnmp_container *container);
    void            ifTable_cache_free(netsnmp_container *container);

    int             ifTable_row_prep(ifTable_rowreq_ctx * rowreq_ctx);





#ifdef __cplusplus
}
#endif
#endif                          /* IFTABLE_DATA_ACCESS_H */
