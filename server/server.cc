// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "gen-cpp/ImageServer.h"
#include "Algorithm.h"

#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>

#include <iostream>

class ImageServerHandler : virtual public ImageServerIf {
public:
  ImageServerHandler() : ImageServerIf() {
  }

  void query(std::vector<std::string> &_return, const std::string &filename) {
    std::cout << "query " << filename << std::endl;

    auto result = algorithm->query(filename);


    for (auto it = result.begin(); it != result.end(); ++it) {
      


      _return.push_back(it->second);
    }
  }

  void reload_func(const std::string& func_name) {
    algorithm->reload_func(func_name);
  }

  void query_result(std::vector<Pair> & _return, const std::string& filename) {
    std::cout << "query_result " << filename << std::endl;

    auto result = algorithm->query(filename);
    _return.resize(result.size());

    for (size_t i = 0; i < result.size(); ++i) {
      _return[i].result = result[i].first;
      _return[i].filename = result[i].second;
    }
  }

  Algorithm *algorithm;
};

int main(int argc, char **argv) {
  using namespace ::apache::thrift;
  using namespace ::apache::thrift::protocol;
  using namespace ::apache::thrift::transport;
  using namespace ::apache::thrift::server;
  using namespace ::apache::thrift::concurrency;

  using boost::shared_ptr;

  if (argc != 2) return 0;

  // argv[1] is the database filename
  std::cout << "loading database..." << std::endl;
  DefaultAlgorithm algorithm;
  algorithm.load_database(argv[1]);

  shared_ptr<ImageServerHandler> handler(new ImageServerHandler());
  handler->algorithm = &algorithm;

  shared_ptr<TProcessor> processor(new ImageServerProcessor(handler));
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  shared_ptr<TServerTransport> serverTransport(new TServerSocket(9090));
  shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());

  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(15);
  shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();

  TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory, threadManager);

  std::cout << "init done!" << std::endl;
  server.serve();
}
