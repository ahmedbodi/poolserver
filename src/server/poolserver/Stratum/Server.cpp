#include "Server.h"

namespace Stratum
{
    void Server::Start(tcp::endpoint endpoint)
    {
        _acceptor.open(endpoint.protocol());
        _acceptor.set_option(tcp::acceptor::reuse_address(true));
        _acceptor.bind(endpoint);
        _acceptor.listen();
        
        _StartAccept();
        
        sLog.Debug(LOG_STRATUM, "Stratum server started");
    }
    
    void Server::SendToAll(JSON msg)
    {
        std::set<ClientPtr>::iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
            _io_service.post(boost::bind(&Client::SendMessage, (*it), msg));
    }
    
    void Server::SendBlockTmpl(bool resetWork)
    {
        std::set<ClientPtr>::iterator it;
        for (it = _clients.begin(); it != _clients.end(); ++it)
            _io_service.post(boost::bind(&Client::SendJob, (*it), resetWork));
    }
    
    bool Server::SubmitBlock(Bitcoin::Block block)
    {
        return NetworkMgr::Instance()->SubmitBlock(block);
    }
}
