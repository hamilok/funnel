/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Alexander Bonar <hamilok@volia.net>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
#define DEMO_USE_SNMP_VERSION_3

#ifdef DEMO_USE_SNMP_VERSION_3
#include "net-snmp/library/transform_oids.h"
const char *our_v3_passphrase = "The Net-SNMP Demo Password";
#endif

int main(int argc, char* argv[])
{
    struct snmp_session session, *ss;
    struct snmp_pdu *pdu;
    struct snmp_pdu *response;

    oid anOID[MAX_OID_LEN];
    size_t anOID_len = MAX_OID_LEN;

    struct variable_list *vars;
    int status;


    /**
     * Initialize the SNMP library
     */
    init_snmp("snmpapp");


    /**
     * Initialize a "session" that defines who we're going to talk to
     */
    snmp_sess_init( &session );                   /* set up defaults */
    session.peername = strdup("sub-6-2.volia.net");

    /* set up the authentication parameters for talking to the server */

    /* set the SNMP version number */
    session.version = SNMP_VERSION_1;

    /* set the SNMPv1 community name used for authentication */
    session.community = (u_char*)strdup("public");
    session.community_len = strlen((char*)session.community);


    /**
     * Open the session
     */
    ss = snmp_open(&session);                     /* establish the session */


    if (!ss) {
        snmp_perror("ack");
        snmp_log(LOG_ERR, "something horrible happened!!!\n");
        exit(2);
    }

    /**
     * Create the PDU for the data for our request.
     *   1) We're going to GET the system.sysDescr.0 node.
     */
    pdu = snmp_pdu_create(SNMP_MSG_GET);

    read_objid(".1.3.6.1.3.83.4.1.5.1.2", anOID, &anOID_len);

    #if OTHER_METHODS
//    get_node("sysDescr.0", anOID, &anOID_len);
//    read_objid("system.sysDescr.0", anOID, &anOID_len);
    #endif

    snmp_add_null_var(pdu, anOID, anOID_len);

    /**
     * Send the Request out.
     */
    status = snmp_synch_response(ss, pdu, &response);


    /**
     * Process the response.
     */
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
      /**
       * SUCCESS: Print the result variables
       */
      for(vars = response->variables; vars; vars = vars->next_variable)
        print_variable(vars->name, vars->name_length, vars);

      /* manipulate the information ourselves */
      for(vars = response->variables; vars; vars = vars->next_variable) {
        int count=1;
        if (vars->type == ASN_OCTET_STR) {
          char *sp = (char*)malloc(1 + vars->val_len);
          memcpy(sp, vars->val.string, vars->val_len);
          sp[vars->val_len] = '\0';
          printf("value #%d is a string: %s\n", count++, sp);
          free(sp);
        }
        else
          printf("value #%d is NOT a string! Ack!\n", count++);
      }

    } else {
      /**
       * FAILURE: print what went wrong!
       */

      if (status == STAT_SUCCESS)
        fprintf(stderr, "Error in packet\nReason: %s\n",
                snmp_errstring(response->errstat));
      else
        snmp_sess_perror("snmpget", ss);

    }

    /**
     * Clean up:
     *  1) free the response.
     *  2) close the session.
     */
    if (response)
      snmp_free_pdu(response);
    snmp_close(ss);

    return 0;
}
