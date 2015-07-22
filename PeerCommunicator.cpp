#include "PeerCommunicator.h"

PeerCommunicator::PeerCommunicator()
{
/*    int sockFd = CreateServerSocket(listeningPort);
    CreateEpollSocket(sockFd);
    watchEvent = &SocketWatcher::StartServerWatcher;
    setWatcherWaitTimer(180000);
*/
}

void PeerCommunicator::SetPeerNode(map<string,IPPack*> &peerNodes,string myName)
{
    map<string,IPPack*>::iterator iter = peerNodes.begin();
    map<string,IPPack*>::iterator iterEnd = peerNodes.end();

    int totalCount = peerNodes.size();
    int myPosition = 0;
    for(; iter != iterEnd; ++iter)
    {
        if(iter->first == myName)
        {
            break;
        }
        ++myPosition;
    }

    //iter = peerNodes.find(myName);

    if(iter == iterEnd)
    {
        cerr << "My Name ["<< myName<<"]" <<"not found in Node list\n";
        exit(0);
    }
/*
    for(int i = 0; i < myPosition; ++i)
        ++iter;
*/
    m_thisNode = new PeerNode(iter->first,iter->second);
    ++iter;

    if(iter != iterEnd)
    {
        string nodeName = iter->first;
        IPPack* &ipPack   = iter->second;
        m_peerLoopStart = new PeerNode(nodeName,ipPack);
        ++iter;
        PeerNode* curNode = m_peerLoopStart;
        PeerNode* preNode = m_peerLoopStart;

        for(;iter != iterEnd; ++iter)
        {
            string nodeName = iter->first;
            IPPack* &ipPack   = iter->second;
            curNode->GetRightPeer() = new PeerNode(nodeName,ipPack);
            curNode = curNode->GetRightPeer();
            curNode->GetLeftPeer() = preNode;
            preNode = preNode->GetRightPeer();
        }

        iter = peerNodes.begin();

        for(int i=0; i< totalCount - myPosition; ++i)
            --iterEnd;

        for(;iter != iterEnd; ++iter)
        {
            string nodeName = iter->first;
            IPPack* &ipPack   = iter->second;
            curNode->GetRightPeer() = new PeerNode(nodeName,ipPack);
            curNode = curNode->GetRightPeer();
            curNode->GetLeftPeer() = preNode;
            preNode = preNode->GetRightPeer();
        }

        m_peerLoopStart->GetLeftPeer() = curNode; // close loop
        curNode->GetRightPeer() = m_peerLoopStart;//close loop
        /*
        m_connectedPeers = new PeerNode();
        m_connectedPeers->GetLeftPeer() = NULL;
        m_connectedPeers->GetRightPeer() = NULL;
        */
        m_connectedPeers = m_peerLoopStart;


    }
}

PeerNode*& PeerCommunicator::GetConnectedNode()
{
    return m_connectedPeers;
}

PeerNode*& PeerCommunicator::GetThisNode()
{
    return m_thisNode;
}

void PeerCommunicator::AddPeerToRightOfNode(string node_name, PeerNode* newNode)
{
    if(m_peerLoopStart)
    {
        PeerNode *currNode = m_peerLoopStart;

        if(currNode->GetNodeName() == node_name)
        {
            PeerNode *tmp = currNode->GetRightPeer();
            currNode->GetRightPeer() = newNode;
            newNode->GetRightPeer() = tmp;
        }
    }

//    ShowPeerLoop();
}

void PeerCommunicator::ShowPeerLoop()
{
    if(m_peerLoopStart)
    {
        PeerNode *currNode = m_peerLoopStart;
        //        PeerNode *endNode  = m_peerLoopStart->GetLeftPeer();

        if( m_peerLoopStart->GetLeftPeer() )
        {
            cerr <<currNode->GetNodeName() <<"\n";
            currNode = currNode->GetRightPeer();

            for(;currNode != m_peerLoopStart; currNode = currNode->GetRightPeer())
            {
                cerr << currNode->GetNodeName() << "\n";
            }
        }

    }
}

/*
void PeerCommunicator::EventErrorHandler(struct epoll_event &event)
{
    printf("PeerCommunicator::EventErrorHandler();\n");
    SocketInfo* dh = (SocketInfo*)event.data.ptr;
    if(epoll_ctl(m_epollDescriptor,EPOLL_CTL_DEL,dh->m_socketDescriptor,&event))
    {
        perror ("epoll_ctl");
        return; // exit()
    }
    //MUXER.RemoveSubscriberSocket(dh->m_socketDescriptor);
    close (dh->m_socketDescriptor);

    delete dh;
}

void PeerCommunicator::AcceptNewClientConnection(struct epoll_event &event)
{
    printf("PeerCommunicator()::AcceptNewClientConnection()\n");

    struct sockaddr in_addr;
    socklen_t in_len;
    int infd;

    in_len = sizeof in_addr;
    infd = accept(m_socketDescriptor,&in_addr,&in_len);

    if (infd == -1) //error or Exception
        return;

    if(SetSocketFlags(infd,O_NONBLOCK) < 0)
    {
        perror ("Flag Error:");
        return;// exit()
    }

    event.data.ptr = new SocketInfo(infd,64);
#ifdef EDGETRIGGERED
    event.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
#else
    event.events = EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
#endif
    if(epoll_ctl(m_epollDescriptor,EPOLL_CTL_ADD,infd,&event))
    {
        perror ("epoll_ctl");
        return; // exit()
    }
}

void PeerCommunicator::ProcessClientEvent(struct epoll_event &event)
{

    printf("PeerCommunicator::ProcessClientEvent();\n");
    int done = 0;

    SocketInfo* dh = (SocketInfo*)event.data.ptr;
#ifdef EDGETRIGGERED
    while(1)
    {
#endif

        dh->ReadFromSocket(dh->m_socketDescriptor,done);
        if(done == 1) //socket read Error
        {
            EventErrorHandler(event);
            return;

        }
#ifdef EDGETRIGGERED

        else if(done == 2) //EAGAIN
        {
            return;
        }

    }
#endif
}
*/
