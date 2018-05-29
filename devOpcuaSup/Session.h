/*************************************************************************\
* Copyright (c) 2018 ITER Organization.
* This module is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
\*************************************************************************/

/*
 *  Author: Ralph Lange <ralph.lange@gmx.de>
 *
 *  based on prototype work by Bernhard Kuner <bernhard.kuner@helmholtz-berlin.de>
 */

#ifndef DEVOPCUA_SESSION_H
#define DEVOPCUA_SESSION_H

#include <iostream>
#include <map>

#include <epicsTypes.h>
#include <initHooks.h>

#include "Subscription.h"

namespace DevOpcua {

/**
 * @brief The Session interface to the client side OPC UA session.
 *
 * Main interface for connecting with any OPC UA Server.
 * The implementation manages the connection to an OPC Unified Architecture
 * server and the application session established with it.
 *
 * The connect call establishes and maintains a Session with a Server.
 *
 * The disconnect call disconnects the Session, deleting all Subscriptions
 * and freeing all related resources on both server and client.
 */

class Session
{
public:
    virtual ~Session() {}

    /**
     * @brief Connect the underlying OPC UA Session.
     *
     * Try connecting the session to the OPC UA server.
     *
     * Non-blocking. Connection status changes shall be reported through a
     * callback interface.
     *
     * If the server is not available at the time of calling,
     * the client library shall continue trying to connect.
     *
     * @return long status (0 = OK)
     */
    virtual long connect() = 0;

    /**
     * @brief Disconnect the underlying OPC UA Session.
     *
     * Disconnect the session from the OPC UA server.
     *
     * This shall delete all subscriptions related to the session on both client
     * and server side, and free all connected resources.
     * The disconnect shall complete and the status change to disconnected even
     * if the underlying service fails and a bad status is returned.
     *
     * The call shall block until all outstanding service calls and active
     * client-side callbacks have been completed.
     * Those are not allowed to block, else client deadlocks will appear.
     *
     * Connection status changes shall be reported through a callback
     * interface.
     *
     * @return long status (0 = OK)
     */
    virtual long disconnect() = 0;

    /**
     * @brief Return connection status of the underlying OPC UA Session.
     *
     * @return bool connection status
     */
    virtual bool isConnected() const = 0;

    /**
     * @brief Get session name.
     *
     * @return const std::string & name
     */
    virtual const std::string & getName() const = 0;

    /**
     * @brief Print configuration and status on stdout.
     *
     * The verbosity level controls the amount of information:
     * 0 = one line
     * 1 = session line, then one line per subscription
     *
     * @param level  verbosity level
     */
    virtual void show(int level) const = 0;

    /**
     * @brief Find a session by name.
     *
     * @param name  session name to search for
     *
     * @return Session & session
     */
    static Session & findSession(const std::string &name);

    /**
     * @brief Check if a session with the specified name exists.
     *
     * @param name  session name to search for
     *
     * @return bool
     */
    static bool sessionExists(const std::string &name);

    int debug;  /**< debug verbosity level */

protected:
    Session() {}
};

} // namespace DevOpcua

#endif // DEVOPCUA_SESSION_H
