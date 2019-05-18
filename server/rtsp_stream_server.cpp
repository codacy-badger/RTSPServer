#include <algorithm>
#include <ctype.h>
#include "HashTable.hh"
#include "GroupsockHelper.hh"
#include "server/rtsp_stream_server.h"
#include "server/rtsp_server_media_subsession.h"

RTSPStreamServer *RTSPStreamServer::createNew(UsageEnvironment &env,
                                              int32_t payload_type,
                                              int32_t bitrate,
                                              std::shared_ptr<StreamQueue> queue,
                                              Port ourPort,
                                              UserAuthenticationDatabase *authDatabase,
                                              unsigned int reclamationTestSeconds) {
    int ourSocket = setUpOurSocket(env, ourPort);
    if (-1 == ourSocket) {
        return nullptr;
    }
    return new RTSPStreamServer(env, payload_type, bitrate, ourSocket, queue,
                                ourPort, authDatabase, reclamationTestSeconds);
}

RTSPStreamServer::RTSPStreamServer(UsageEnvironment &env,
                                   int32_t payload_type,
                                   int32_t bitrate,
                                   int32_t ourSocket,
                                   std::shared_ptr<StreamQueue> queue,
                                   Port ourPort,
                                   UserAuthenticationDatabase *authDatabase,
                                   unsigned int reclamationTestSeconds)
    : RTSPServer(env, ourSocket, ourPort, authDatabase, reclamationTestSeconds),
      fStreamTable(HashTable::create(STRING_HASH_KEYS)),
      payload_type_(payload_type),
      streamQueue_(queue),
      bitrate_(bitrate) {
    log_ = Log::getInstance();
}

RTSPStreamServer::~RTSPStreamServer() {
    while (nullptr != fStreamTable->getFirst()) {}
    delete (fStreamTable);
}

void RTSPStreamServer::addStream(RTSPStreamInput *streamInput) {
    auto streamName = streamInput->streamName();
    removeStream(streamName);
    fStreamTable->Add(streamName.c_str(), streamInput);
}

void RTSPStreamServer::removeStream(RTSPStreamInput *streamInput) {
    if (nullptr == streamInput) {
        return;
    }
    fStreamTable->Remove(streamInput->streamName().c_str());
    Medium::close(streamInput);
}

void RTSPStreamServer::removeStream(std::string streamName) {
    removeStream(reinterpret_cast<RTSPStreamInput *>(fStreamTable->Lookup(streamName.c_str())));
}

ServerMediaSession *RTSPStreamServer::lookupServerMediaSession(char const *streamName,
                                                               Boolean isFirstLookupInSession) {
    ServerMediaSession *serverMediaSession = nullptr;
    RTSPStreamInput *streamInput = nullptr;
    const char *key = nullptr;
    std::string strName = streamName;
    StreamChannel *streamChannel = nullptr;
    std::transform(strName.begin(), strName.end(), strName.begin(), ::tolower);

    HashTable::Iterator *iterator = HashTable::Iterator::create(*fStreamTable);
    while (nullptr != (streamInput = reinterpret_cast<RTSPStreamInput *>(iterator->next(key)))) {
        if (strName == streamInput->streamName()) {
            serverMediaSession = RTSPServer::lookupServerMediaSession(strName.c_str(), isFirstLookupInSession);
            if (nullptr == serverMediaSession) {
                char const *descriptionString = "Session streamed by \"RTSPServer\"";
                serverMediaSession =
                    ServerMediaSession::createNew(envir(), strName.c_str(), strName.c_str(), descriptionString);
                if (nullptr != serverMediaSession) {
                    streamChannel = StreamChannel::createNew(StreamType::kMaster);
                    /* FIXME: add substream, request IDR frame */
                    serverMediaSession->addSubsession(RTSPServerMediaSubsession::createNew(payload_type_,
                                                                                           bitrate_,
                                                                                           streamQueue_,
                                                                                           streamInput,
                                                                                           envir()));
                    addServerMediaSession(serverMediaSession);
                }
            }
            break;
        }
    }

    if (nullptr != streamChannel) {
        delete (streamChannel);
    }
    delete (iterator);
    return serverMediaSession;
}

int32_t RTSPStreamServer::setUpOurSocket(UsageEnvironment &env, Port &port) {
#define LISTEN_BACKLOG_SIZE 20
    int ourSocket = -1;

    do {
        // The following statement is enabled by default.
        // Don't disable it (by defining ALLOW_SERVER_PORT_REUSE) unless you know what you're doing.
#if !defined(ALLOW_SERVER_PORT_REUSE) && !defined(ALLOW_RTSP_SERVER_PORT_REUSE)
        // ALLOW_RTSP_SERVER_PORT_REUSE is for backwards-compatibility #####
        NoReuse dummy(env); // Don't use this socket if there's already a local server using it
#endif

        ourSocket = setupStreamSocket(env, port);
        if (ourSocket < 0) {
            break;
        }

        // Make sure we have a big send buffer:
        if (!increaseSendBufferTo(env, ourSocket, 50*1024)) {
            break;
        }

        // Allow multiple simultaneous connections:
        if (listen(ourSocket, LISTEN_BACKLOG_SIZE) < 0) {
            env.setResultErrMsg("listen() failed: ");
            break;
        }

        if (port.num() == 0) {
            // bind() will have chosen a port for us; return it also:
            if (!getSourcePort(env, ourSocket, port)) break;
        }

        return ourSocket;
    } while (0);

    if (ourSocket != -1) {
        ::closeSocket(ourSocket);
    }
    return -1;
}
