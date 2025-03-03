/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.67 $ of : mfd-interface.m2c,v $
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/ip-mib/ipSystemStatsTable/ipSystemStatsTable_interface.h#1 $
 */
/** @ingroup interface Routines to interface to Net-SNMP
 *
 * \warning This code should not be modified, called directly,
 *          or used to interpret functionality. It is subject to
 *          change at any time.
 * 
 * @{
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */
#ifndef IPSYSTEMSTATSTABLE_INTERFACE_H
#define IPSYSTEMSTATSTABLE_INTERFACE_H

#ifdef __cplusplus
extern          "C" {
#endif


#include "ipSystemStatsTable.h"


    /*
     ********************************************************************
     * Table declarations
     */

    /*
     * PUBLIC interface initialization routine 
     */
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        _ipSystemStatsTable_initialize_interface
        (ipSystemStatsTable_registration * user_ctx, u_long flags);
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        _ipSystemStatsTable_shutdown_interface
        (ipSystemStatsTable_registration * user_ctx);

         
         
         
         
         
         
        ipSystemStatsTable_registration
        * ipSystemStatsTable_registration_get(void);

         
         
         
         
         
         
        ipSystemStatsTable_registration
        * ipSystemStatsTable_registration_set
        (ipSystemStatsTable_registration * newreg);

    netsnmp_container *ipSystemStatsTable_container_get(void);
    int             ipSystemStatsTable_container_size(void);

         
         
         
         
         
         
        ipSystemStatsTable_rowreq_ctx
        * ipSystemStatsTable_allocate_rowreq_ctx(ipSystemStatsTable_data *,
                                                 void *);
    void
     
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        ipSystemStatsTable_release_rowreq_ctx(ipSystemStatsTable_rowreq_ctx
                                              * rowreq_ctx);

    int             ipSystemStatsTable_index_to_oid(netsnmp_index *
                                                    oid_idx,
                                                    ipSystemStatsTable_mib_index
                                                    * mib_idx);
    int             ipSystemStatsTable_index_from_oid(netsnmp_index *
                                                      oid_idx,
                                                      ipSystemStatsTable_mib_index
                                                      * mib_idx);

    /*
     * access to certain internals. use with caution!
     */
    void
     
        
        
        
        
        
        
        
        
        
             ipSystemStatsTable_valid_columns_set(netsnmp_column_info *vc);


#ifdef __cplusplus
}
#endif
#endif                          /* IPSYSTEMSTATSTABLE_INTERFACE_H */
/**  @} */

