#include <boost/asio/signal_set.hpp>
#include <boost/asio/io_service.hpp>
#include "utils/idle_service.h"
#include "utils/log.h"
#include "utils/util.h"
#include "client/transcode_service.h"
#include "client/record_service.h"

int main(int argc, char **argv) {
    const std::string url = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8"; // CCTV1
    Log *log = Log::getInstance();
    TranscodeService *transcodeService = new TranscodeService();
    RecordService *recordService = new RecordService();
    IdleService *idleService = new IdleService();
    std::shared_ptr<StreamQueue> streamQueue = std::make_shared<StreamQueue>();
    boost::asio::io_service io_service;
    boost::asio::signal_set signals(io_service, SIGINT, SIGALRM);

    Util::enable_core_dump();

    log->init();

    transcodeService->setStreamQueue(streamQueue);
    transcodeService->init(url);
    auto param = transcodeService->getCodecParameter();

    recordService->setStreamQueue(streamQueue);
    recordService->init(param);

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

    recordService->destory();
    delete(recordService);

    transcodeService->destory();
    delete(transcodeService);

    log->destroy();

    return 0;
}