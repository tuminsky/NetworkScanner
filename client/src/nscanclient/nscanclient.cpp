#include "nscanclient/nscanclient.hpp"
#include "nscanclient/nscandef.hpp"
#include "nscanservice.pb.h"
#include "basedb.hpp"

#include <grpcpp/grpcpp.h>

namespace swatcher
{

NscanClient::NscanClient(QObject* parent) : QObject(parent) { }

void NscanClient::connect(const QString& target)
{
  stub_ = network_scanner::NscanService::NewStub(
    grpc::CreateChannel(target.toStdString(), grpc::InsecureChannelCredentials())
  );

  network_scanner::DbGuestConfig res;
  grpc::ClientContext context;

  const auto status = stub_->connect(&context, google::protobuf::Empty{}, &res);

  if (status.ok()) {
    storage::DbConfig config;
    config.type = QString::fromStdString(res.type());
    config.host = QString::fromStdString(res.host());
    config.port = res.port();
    config.username = QString::fromStdString(res.username());
    config.password = QString::fromStdString(res.password());
    config.db_name = QString::fromStdString(res.db_name());

    emit connected(config);
  } else {
    emit failed("Failed connect to " + target);
  }
}

void NscanClient::start_scan(const QString& target)
{
  network_scanner::StartScanRequest req;
  req.set_target(target.toStdString());

  network_scanner::StartScanResponse res;
  grpc::ClientContext context;
  
  const auto status = stub_->start_scan(&context, req, &res);

  if (status.ok()) {
    StartScanResponse response;
    
    response.success = res.success();
    response.hosts.reserve(res.hosts().size());

    std::copy(res.hosts().begin(), res.hosts().end(),
              std::back_inserter(response.hosts));

    emit scan_finished(response);
  } else {
    emit failed(QString::fromStdString(status.error_code() + ": " + status.error_message()));
  }
}

} // namespace swatcher