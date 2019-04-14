#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_service.hpp>
#include "utils/util.h"
#include "utils/stream_queue.h"
#include "utils/idle_service.h"
#include "server/rtsp_service.h"

int main(int argc, char **argv) {
    Log    *log = Log::getInstance();
    std::shared_ptr<RTSPParameter> param = std::make_shared<RTSPParameter>();
    std::shared_ptr<StreamQueue> streamQueue = std::make_shared<StreamQueue>();
    std::shared_ptr<IdleService> idleService = std::make_shared<IdleService>();
    std::shared_ptr<RTSPService> rtspService = nullptr;
    boost::asio::io_service io_service;
    boost::asio::signal_set signals(io_service, SIGINT, SIGALRM);

    log->init();

    Util::enable_core_dump();

    param->payload_type_ = AV_CODEC_ID_H265;
    param->stream_name_ = "stream";
    param->port_ = 554u;
    param->bitrate_ = 4096;
    param->account.insert(std::make_pair<std::string, std::string>("admin", "admin"));

    /* init encoder, get frame from encoder and then send to streamQueue
     * ......
    */

    rtspService = std::make_shared<RTSPService>(param, streamQueue);
    rtspService->init();

    idleService->init();

    log->info("start nap");
    signals.async_wait(
        [&io_service](const boost::system::error_code& /*error*/, int signal_number){
            switch (signal_number) {
                case SIGINT: {
                    io_service.stop();
                    break;
                };

                case SIGALRM: {
                    io_service.stop();
                    break;
                };

                default: {
                    break;
                }
            }
        });

    log->info("nap alarm setup done");
    io_service.run();
    log->info("io_service done");
    idleService->destroy();
    log->info("nap done");

    rtspService->destroy();
    log->destroy();

    return 0;
}

