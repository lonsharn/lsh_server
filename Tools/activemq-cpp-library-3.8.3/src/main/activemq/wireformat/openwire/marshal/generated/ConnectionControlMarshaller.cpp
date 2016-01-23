/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/wireformat/openwire/marshal/generated/ConnectionControlMarshaller.h>

#include <activemq/commands/ConnectionControl.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <decaf/lang/Pointer.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace activemq::wireformat::openwire::utils;
using namespace activemq::wireformat::openwire::marshal::generated;
using namespace decaf;
using namespace decaf::io;
using namespace decaf::lang;

///////////////////////////////////////////////////////////////////////////////
DataStructure* ConnectionControlMarshaller::createObject() const {
    return new ConnectionControl();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char ConnectionControlMarshaller::getDataStructureType() const {
    return ConnectionControl::ID_CONNECTIONCONTROL;
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionControlMarshaller::tightUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs) {

    try {

        BaseCommandMarshaller::tightUnmarshal(wireFormat, dataStructure, dataIn, bs);

        ConnectionControl* info =
            dynamic_cast<ConnectionControl*>(dataStructure);

        int wireVersion = wireFormat->getVersion();

        info->setClose(bs->readBoolean());
        info->setExit(bs->readBoolean());
        info->setFaultTolerant(bs->readBoolean());
        info->setResume(bs->readBoolean());
        info->setSuspend(bs->readBoolean());
        if (wireVersion >= 6) {
            info->setConnectedBrokers(tightUnmarshalString(dataIn, bs));
        }
        if (wireVersion >= 6) {
            info->setReconnectTo(tightUnmarshalString(dataIn, bs));
        }
        if (wireVersion >= 6) {
            info->setRebalanceConnection(bs->readBoolean());
        }
        if (wireVersion >= 8) {
            info->setToken(tightUnmarshalByteArray(dataIn, bs));
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
int ConnectionControlMarshaller::tightMarshal1(OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs) {

    try {

        ConnectionControl* info =
            dynamic_cast<ConnectionControl*>(dataStructure);

        int rc = BaseCommandMarshaller::tightMarshal1(wireFormat, dataStructure, bs);

        int wireVersion = wireFormat->getVersion();

        bs->writeBoolean(info->isClose());
        bs->writeBoolean(info->isExit());
        bs->writeBoolean(info->isFaultTolerant());
        bs->writeBoolean(info->isResume());
        bs->writeBoolean(info->isSuspend());
        if (wireVersion >= 6) {
            rc += tightMarshalString1(info->getConnectedBrokers(), bs);
        }
        if (wireVersion >= 6) {
            rc += tightMarshalString1(info->getReconnectTo(), bs);
        }
        if (wireVersion >= 6) {
            bs->writeBoolean(info->isRebalanceConnection());
        }
        if (wireVersion >= 8) {
            bs->writeBoolean(info->getToken().size() != 0);
            rc += info->getToken().size() == 0 ? 0 : (int)info->getToken().size() + 4;
        }

        return rc + 0;
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionControlMarshaller::tightMarshal2(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs) {

    try {

        BaseCommandMarshaller::tightMarshal2(wireFormat, dataStructure, dataOut, bs );

        ConnectionControl* info =
            dynamic_cast<ConnectionControl*>(dataStructure);

        int wireVersion = wireFormat->getVersion();

        bs->readBoolean();
        bs->readBoolean();
        bs->readBoolean();
        bs->readBoolean();
        bs->readBoolean();
        if (wireVersion >= 6) {
            tightMarshalString2(info->getConnectedBrokers(), dataOut, bs);
        }
        if (wireVersion >= 6) {
            tightMarshalString2(info->getReconnectTo(), dataOut, bs);
        }
        if (wireVersion >= 6) {
            bs->readBoolean();
        }
        if (wireVersion >= 8) {
            if (bs->readBoolean()) {
                dataOut->writeInt((int)info->getToken().size() );
                dataOut->write((const unsigned char*)(&info->getToken()[0]), (int)info->getToken().size(), 0, (int)info->getToken().size());
            }
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionControlMarshaller::looseUnmarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn) {

    try {

        BaseCommandMarshaller::looseUnmarshal(wireFormat, dataStructure, dataIn);
        ConnectionControl* info =
            dynamic_cast<ConnectionControl*>(dataStructure);

        int wireVersion = wireFormat->getVersion();

        info->setClose(dataIn->readBoolean());
        info->setExit(dataIn->readBoolean());
        info->setFaultTolerant(dataIn->readBoolean());
        info->setResume(dataIn->readBoolean());
        info->setSuspend(dataIn->readBoolean());
        if (wireVersion >= 6) {
            info->setConnectedBrokers(looseUnmarshalString(dataIn));
        }
        if (wireVersion >= 6) {
            info->setReconnectTo(looseUnmarshalString(dataIn));
        }
        if (wireVersion >= 6) {
            info->setRebalanceConnection(dataIn->readBoolean());
        }
        if (wireVersion >= 8) {
            info->setToken(looseUnmarshalByteArray(dataIn));
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}

///////////////////////////////////////////////////////////////////////////////
void ConnectionControlMarshaller::looseMarshal(OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut) {

    try {

        ConnectionControl* info =
            dynamic_cast<ConnectionControl*>(dataStructure);
        BaseCommandMarshaller::looseMarshal(wireFormat, dataStructure, dataOut);

        int wireVersion = wireFormat->getVersion();

        dataOut->writeBoolean(info->isClose());
        dataOut->writeBoolean(info->isExit());
        dataOut->writeBoolean(info->isFaultTolerant());
        dataOut->writeBoolean(info->isResume());
        dataOut->writeBoolean(info->isSuspend());
        if (wireVersion >= 6) {
            looseMarshalString(info->getConnectedBrokers(), dataOut);
        }
        if (wireVersion >= 6) {
            looseMarshalString(info->getReconnectTo(), dataOut);
        }
        if (wireVersion >= 6) {
            dataOut->writeBoolean(info->isRebalanceConnection());
        }
        if (wireVersion >= 8) {
            dataOut->write( info->getToken().size() != 0 );
            if( info->getToken().size() != 0 ) {
                dataOut->writeInt( (int)info->getToken().size() );
                dataOut->write((const unsigned char*)(&info->getToken()[0]), (int)info->getToken().size(), 0, (int)info->getToken().size());
            }
        }
    }
    AMQ_CATCH_RETHROW(decaf::io::IOException)
    AMQ_CATCH_EXCEPTION_CONVERT(exceptions::ActiveMQException, decaf::io::IOException)
    AMQ_CATCHALL_THROW(decaf::io::IOException)
}
